/*! \file	sqlite3entityfactory.cpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */
#include "entities/factories/sqlite3entityfactory.hpp"
#include "entities/factories/sqlite3persistenceapi.hpp"

#include <sqlite3.h>

namespace tdk {
namespace ent {


Sqlite3EntityFactory::Sqlite3EntityFactory(const char* dbFile) throw(Entception&)
{
	// Attempt to open the database.
	int res = sqlite3_open(dbFile, &db_);
	
	if ( res != SQLITE_OK ) {
		throw Entception("Failed to open database file");
	}

	persistence_.setDb(db_);
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
	e->setPersistence( &persistence_ );
}


}	// End namespace ent
}	// End namespace tdk
