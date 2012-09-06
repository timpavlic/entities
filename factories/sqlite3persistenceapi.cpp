/*! \file	sqlite3persistenceapi.cpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */
#include <sstream>

#include <iostream>

#include <sqlite3.h>

#include "sqlite3persistenceapi.hpp"

using namespace std;
using namespace tdk::ent;

namespace {

class Sqlite3ReadVisitor : public ReadVisitor
{
public:
	Sqlite3ReadVisitor(stringstream& s) : ss(s) {}

	virtual bool visit(const bool& b) {
		ss << (b ? "true" : "false");
		return true;
	}
	
	virtual bool visit(const char& c) {
		return false;
	}

	virtual bool visit(const int& i) {
		ss << i;
		return true;
	}

	virtual bool visit(const unsigned int& i) {
		ss << i;
		return true;
	}

	virtual bool visit(const double& d) {
		ss << d;
		return true;
	}

	virtual bool visit(const StringPrimitive& str)
	{
		ss << "'";
		for ( unsigned int i = 0; i < str.len(); ++i ) {
			if ( str.data()[i] == '\'' ) {
				ss << "''";
			} else {
				ss << str.data()[i];
			}
		}
		ss << "'";
		return true;
	}


	virtual ~Sqlite3ReadVisitor() {}
	
private:
	stringstream& ss;
};


class Sqlite3WriteVisitor : public WriteVisitor
{
public:
	const char* valStr;	// Value to assign, but as a string from sqlite3 callback.

	virtual void visit(bool& b) {

	}

	virtual void visit(char& c) {

	}

	virtual void visit(int& i) {

	}

	virtual void visit(unsigned int& ui) {

	}

	virtual void visit(StringPrimitive& str) {

	}
};

/** Sqlite3 callback implementation for entity saving operations.
 *
 * \param	propDeque	The context should be a pointer to a deque of AbstractProperty pointers.
 * \param	colCount	Number of columns retreived.
 * \param	colNames	Names of the columns. Should match properties in an Entity.
 * \param	colData		Data retreived for each column.
 */
int save_callback(void* propDeque, int colCount, char** colNames, char** colData)
{
	const deque<AbstractProperty*>& props = *reinterpret_cast< deque<AbstractProperty*>* >(propDeque);

	cout << "We have " << colCount << " columns" << endl;

	Sqlite3WriteVisitor vis;


	return 0;
}

}	// End anon namespace


namespace tdk {
namespace ent {

bool Sqlite3PersistenceApi::save(const Entity& e) throw(Entception&)
{
	stringstream ss;
	
	const deque<AbstractProperty*>& props = e.properties();
	
	ss << "INSERT INTO " << e.entitytype() << '(';
	// Get all the property names
	for ( unsigned int i=0; i<props.size(); ++i ) {
		ss << props[i]->propertyName();
		cout << props[i]->propertyName() << endl;
		if ( i < (props.size() - 1) ) {
			ss << ',';
		}
	}
	Sqlite3ReadVisitor reader(ss);
	ss << ") VALUES(";
	for ( unsigned int i=0; i<props.size(); ++i ) {
		// Attempt to visit the property.
		if ( !props[i]->acceptReader(reader) ) {
			string msg("Could not visit property '");
			msg += props[i]->propertyName();
			msg += '\'';
			throw SaveEntception(&e, msg.c_str());
		}
		if ( i < props.size() - 1 ) {	// Add comma between all but last value in query
			ss << ',';
		}
	}
	ss << ");";	// Terminate the query
	
	// TODO: Call sqlite3_exec or w/e the func is.
	char* sqliteErr = NULL;
	cout << "Would like to execute this query: " << ss.str() << endl;
	int res = sqlite3_exec(db_, ss.str().c_str(), save_callback, const_cast<void*>(reinterpret_cast<const void*>(&props)), &sqliteErr);
	
	if ( res != SQLITE_OK ) {
		cout << "ERROR. result = " << res << endl;
	} else {
		cout << "Query executed ok" << endl;
	}
	if ( sqliteErr ) {
		cout << "Sqlite3 error: " << sqliteErr << endl;
		sqlite3_free(sqliteErr);
	}

	return false;
}

bool Sqlite3PersistenceApi::update(const Entity& ent, const AbstractPropertyCollection& updates) throw(Entception&)
{
	std::stringstream sql;
	
	// Validate collection against entity.
	if ( !ent.isSubset(updates) ) {
		throw UpdateEntception(&ent, "Updates are not a valid subset.");
	}
	
	const Entity::PropertyDeque& newVals = updates.props();
	
	sql << "UPDATE " << ent.entitytype() << "(";
	
	// Get all the property names for the update
	for ( unsigned int i = 0; i < newVals.size(); ++i ) {
		sql << newVals[i]->propertyName();
		if ( i < newVals.size() -2 ) {
			sql << ',';
		}
	}
	// Specify the update values.
	Sqlite3ReadVisitor reader(sql);
	sql << ") VALUES(";
	for ( unsigned int i = 0; i < newVals.size(); ++i ) {
		// Attempt to visit the value.
		if ( !newVals[i]->acceptReader(reader) ) {
			string msg("Could not visit update property '");
			msg += newVals[i]->propertyName();
			msg += '\'';
			throw UpdateEntception(&ent, msg);
		}
	}
	
	// Specify the where clause, requiring the entity to be a currently saved on.
	sql << ") WHERE(";
	const Entity::PropertyDeque& props = ent.properties();
	for ( unsigned int i = 0; i < props.size(); ++i ) {
		// Add the property names first.
		sql << props[i]->propertyName() << '=';
		
		// Now attempt to visit the value.
		if ( !props[i]->acceptReader(reader) ) {
			string msg("Could not visit match property '");
			msg += props[i]->propertyName();
			msg += '\'';
			throw UpdateEntception(&ent, msg);
		}
		
		if ( i < newVals.size() -2 ) {	// Add comma for all but last item.
			sql << ',';
		}
	}
	sql << ");";
	/* update ent.name values(updates.values) where (match each ent.properties); */
	
	// Attempt to execute the query.
	char* msg;
	//int res = sqlite3_exec(db_, sql.str().c_str(), sqlite3_callback, TODO_UPDATE_CONTEXT_OBJ, &msg);
	
	
	return false;
}

bool Sqlite3PersistenceApi::load(Entity& ent, const AbstractPropertyCollection& criteria) throw(Entception&)
{
	std::stringstream sql;
	
	// Validate collection against entity.
	if ( !ent.isSubset(criteria) ) {
		throw LoadEntception(&ent, "Load criteria are not a valid subset.");
	}
	
	const Entity::PropertyDeque& props = ent.properties();
	
	// Start creating the select statement. We need to explicitly list all props
	sql << "SELECT FROM " << ent.entitytype() << '(';
	for ( size_t i = 0; i < props.size(); ++i ) {
		sql << props[i]->propertyName();
		
		if ( i < props.size() - 2 ) {
			sql << ',';
		}
	}
	sql << ") WHERE(";
	
	// Now we need to build the where clause equalities.
	Sqlite3ReadVisitor reader(sql);
	const Entity::PropertyDeque& loadVals = criteria.props();
	for ( size_t i = 0; i < loadVals.size(); ++i ) {
		sql << loadVals[i]->propertyName() << '=';
		
		// Attempt to visit the property so it's value is appended to the query.
		if ( !loadVals[i]->acceptReader(reader) ) {
			string msg("Could not visit match property '");
			msg += loadVals[i]->propertyName();
			msg += '\'';
			throw LoadEntception(&ent, msg.c_str());
		}
		
		if ( i < loadVals.size() - 2 ) {	// Add comma for all but last val.
			sql << ',';
		}
	}
	sql << ");";	// Terminate query.
	
	// Perform query
	//if ( sqlite3_exec(...) != SQLITE_OK ) {
	//
	//}
	
	// TODO: Will need a visitor that will perform assignment. Does Sqlite give us strings, ints, blobs?
	//Sqlite3ReadVisitor assigner(sql);
	//Sqlite3WriteVisitor writer(sql);
	// Now 
	
	return NULL;
}

bool Sqlite3PersistenceApi::del(const Entity& e) throw(Entception&)
{
	stringstream sql;
	
		// Or is it REMOVE
	sql << "DELETE * FROM " << e.entitytype() << " WHERE(";
	
	// Build up the where clause by iterating the entities properties.
	const Entity::PropertyDeque& props = e.properties();
	Sqlite3ReadVisitor reader(sql);
	for ( size_t i; i < props.size(); ++i ) {
		sql << props[i]->propertyName() << '=';
		
		// Attempt to visit the property to get it to SQL text.
		if ( !props[i]->acceptReader(reader) ) {
			string msg("Could not visit property '");
			msg += props[i]->propertyName();
			msg += '\'';
			throw DelEntception(&e, msg.c_str());
		}
		
		if ( i < props.size() - 2 ) {	// Add comma for all but last field.
			sql << ',';
		}
	}
	sql << ");";	// Terminate the query.
	
	// if ( sqlite3_exec(...) != SQLITE_OK ) {
	//
	//}
	
	return false;
}

}	// End namespace ent
}	// End namespace tdk
