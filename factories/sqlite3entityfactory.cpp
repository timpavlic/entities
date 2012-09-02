/*! \file	sqlite3entityfactory.cpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */
#include "entities/factories/sqlite3entityfactory.hpp"
#include "entities/factories/sqlite3persistenceapi.hpp"

#include <sqlite3.h>

namespace {

// We use a function with a static member to get the sqlite3 persistene to ensure
// that it will be initialised when needed.
Sqlite3PersistenceApi* sqlite3_persistence()
{
	static Sqlite3PersistenceApi persistence;
	return &persistence;
}

}

Sqlite3EntityFactory::Sqlite3EntityFactory(const char* dbFile) throw(Entception&)
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
	e->setPersistence( sqlite3_persistence() );
}
