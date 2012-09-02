/*! \file	sqlite3persistenceapi.cpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */
#include <sstream>

#include <sqlite3.h>

#include "sqlite3persistenceapi.hpp"

using namespace std;

namespace {

int sqlite3_callback(void* context, int number, char** something, char** morethings)
{

	return 0;
}

// Property visitor implementation.
//
// How do we allow library users to define SQLite3 Property visitors for their
// own types.???
struct SqliteVisitor : public PropertyVisitorBase, PropertyVisitor<double>,
	PropertyVisitor<int>, PropertyVisitor<unsigned int>, PropertyVisitor<bool>,
	PropertyVisitor<char>, PropertyVisitor<const char*>
{
	SqliteVisitor(stringstream& s) : ss(s) {}
	
	virtual bool visit(double& d) {
		ss << d;
		return true;
	}
	
	virtual bool visit(int& i) {
		ss << i;
		return true;
	}
	
	virtual bool visit(unsigned int& i) {
		ss << i;
		return true;
	}
	
	virtual bool visit(bool& b) {
		ss << (b ? "true" : "false");
		return true;
	}
	
	// Escapes a single character if it is '
	virtual bool visit(char& c) {
		if ( c == '\'' ) {
			ss << "''''";
		} else {
			ss << "'" << c << "'";	// 'z'
			// '''' Four apostrophes. First is literal. Next two are escaped to an apostrophe. Last is liteal
		}
		return true;
	}
	
	// Escapes the string.
	virtual bool visit(const char*& s) {
		ss << "'";
		for ( unsigned int i=0; s[i]; ++i ) {
			if ( s[i] == '\'' ) {
				ss << "''";
			} else {
				ss << s[i];
			}
		}
		ss << "'";
		return true;
	}

private:
	stringstream& ss;
};

}	// End anon namespace


bool Sqlite3PersistenceApi::save(const Entity& e) throw(Entception&)
{
	stringstream ss;
	
	const deque<AbstractProperty*>& props = e.properties();
	
	ss << "INSERT INTO " << e.entitytype() << '(';
	// Get all the property names
	for ( unsigned int i=0; i<props.size(); ++i ) {
		ss << props[i]->name();
		if ( i < props.size() -2 ) {
			ss << ',';
		}
	}
	SqliteVisitor sv(ss);
	ss << ") VALUES(";
	for ( unsigned int i=0; i<props.size(); ++i ) {
		// Attempt to visit the property.
		if ( !props[i]->accept(sv) ) {
			string msg("Could not visit property '");
			msg += props[i]->name();
			msg += '\'';
			throw SaveEntception(&e, msg.c_str());
		}
		if ( i < props.size() - 2 ) {	// Add comma between all but last value in query
			ss << ',';
		}
	}
	ss << ");";	// Terminate the query
	
	// TODO: Call sqlite3_exec or w/e the func is.
	
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
		sql << newVals[i]->name();
		if ( i < newVals.size() -2 ) {
			sql << ',';
		}
	}
	// Specify the update values.
	SqliteVisitor sv(sql);
	sql << ") VALUES(";
	for ( unsigned int i = 0; i < newVals.size(); ++i ) {
		// Attempt to visit the value.
		if ( !newVals[i]->accept(sv) ) {
			string msg("Could not visit update property '");
			msg += newVals[i]->name();
			msg += '\'';
			throw UpdateEntception(&ent, msg);
		}
	}
	
	// Specify the where clause, requiring the entity to be a currently saved on.
	sql << ") WHERE(";
	const Entity::PropertyDeque& props = ent.properties();
	for ( unsigned int i = 0; i < props.size(); ++i ) {
		// Add the property names first.
		sql << props[i]->name() << '=';
		
		// Now attempt to visit the value.
		if ( !props[i]->accept(sv) ) {
			string msg("Could not visit match property '");
			msg += props[i]->name();
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
		sql << props[i]->name();
		
		if ( i < props.size() - 2 ) {
			sql << ',';
		}
	}
	sql << ") WHERE(";
	
	// Now we need to build the where clause equalities.
	SqliteVisitor sv(sql);
	const Entity::PropertyDeque& loadVals = criteria.props();
	for ( size_t i = 0; i < loadVals.size(); ++i ) {
		sql << loadVals[i]->name() << '=';
		
		// Attempt to visit the property so it's value is appended to the query.
		if ( !loadVals[i]->accept(sv) ) {
			string msg("Could not visit match property '");
			msg += loadVals[i]->name();
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
	SqliteVisitor assigner(sql);
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
	SqliteVisitor sv(sql);
	for ( size_t i; i < props.size(); ++i ) {
		sql << props[i]->name() << '=';
		
		// Attempt to visit the property to get it to SQL text.
		if ( !props[i]->accept(sv) ) {
			string msg("Could not visit property '");
			msg += props[i]->name();
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
