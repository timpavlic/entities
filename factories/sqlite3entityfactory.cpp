/*! \file	sqlite3entityfactory.cpp
 *
 * Copyright 2012. See COPYING for details.
 */
#include "entities/sqlite3entityfactory.hpp"


Sqlite3EntityFactory::Sqlite3EntityFactory(const char* dbFile) : throw(Entception&)
{
	// Attempt to open the database.
	int res = sqlite3_open(dbFile, &db_);
	
	if ( res != SQLITE_OK ) {
		throw Entception("Failed to open database file");
	}
}

Sqlite3EntityFactory::~Sqlite3EntityFactory()
{
	int res = sqlite3_close(db_);
	
	if ( res != SQLITE_OK ) {
		// Report error somehow.
	}
}

void Sqlite3EntityFactory::installPersistenceApi(Entity* e)
{

}
