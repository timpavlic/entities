/*! Example usage of entities. */

#include <iostream>
#include <fstream>

#include "entityfactory.hpp"
#include "property.hpp"

using namespace std;

struct Integer {
	int val;
};

struct TestEnt : public Entity
{
	TestEnt() : Entity("testent"),
		anInt("anint", this),
		wrapped("wrapped", this)
	{}
	
	Property<int> anInt;
	Property<Integer> wrapped;
};

struct FlatFileSaveVisitor : public PropertyVisitorBase, public PropertyVisitor<int>,
	public PropertyVisitor<Integer>
{
	FlatFileSaveVisitor(ofstream& f) : file(f) {}

	virtual bool visit(int& i) {
		file << i;
		return true;
	}
	
	virtual bool visit(Integer& i) {
		file << "Integer(" << i.val << ")";
		return true;
	}
	
	ofstream& file;
};


struct FlatFilePersistenceApi : public PersistenceApi
{
	FlatFilePersistenceApi(const char* filename) : file(filename) {}

	virtual bool save(const Entity& ent) throw(Entception&) {
	
		const Entity::PropertyDeque& props = ent.properties();
	
		FlatFileSaveVisitor ffsv(file);
	
		file << "{";
		for ( size_t i = 0; i < props.size(); ++i ) {
			file << props[i]->name() << " = ";
			props[i]->accept(ffsv);
			
			if ( i < props.size() - 1 ) {
				file << ", ";
			}
		}
		file << "},\n";
	
		return true;
	}
	virtual bool update(const Entity& old, const AbstractPropertyCollection& updates) throw(Entception&)
	{ throw Entception("Update not implemented."); }
	
	virtual bool load(Entity& e, const AbstractPropertyCollection& criteria) throw(Entception&)
	{ throw Entception("Load not implemented."); }
	
	virtual bool del(const Entity& e) throw(Entception&)
	{ throw Entception("Del not implemented."); }
	
	ofstream file;
};

struct FlatFileEntityFactory : public EntityFactory
{
	FlatFileEntityFactory(const char* filename) : pers(filename) {}

private:
	virtual void installPersistenceApi(Entity* e) {
		e->setPersistence(&pers);
	}
	
	FlatFilePersistenceApi pers;
};


int main(void)
{
	FlatFileEntityFactory ffef("test.txt");
	
	TestEnt* ent = ffef.create<TestEnt>();
	
	ent->anInt.set(9001);
	ent->wrapped.set({1337});
	
	ent->save();
	
		
	return 0;
}
