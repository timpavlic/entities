#ifndef SQLITE3_ENTITY_PERSISTENCE_HPP
#define SQLITE3_ENTITY_PERSISTENCE_HPP
/*! \file	sqlite3persistenceapi.hpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */

#include "entities/entity.hpp"

// Forward declaration.
struct sqlite3;

namespace tdk {
namespace ent {

/*! Persistence implementation for SQLite 3. */
class Sqlite3PersistenceApi : public PersistenceApi
{
public:
	Sqlite3PersistenceApi() : db_(NULL) {}
	Sqlite3PersistenceApi(sqlite3* db);

	virtual bool save(const Entity&) throw(Entception&);
	
	virtual bool update(const Entity& ent, const AbstractPropertyCollection& updates) throw(Entception&);
	
	virtual bool load(Entity& ent, const AbstractPropertyCollection& criteria) throw(Entception&);
	
	virtual bool del(const Entity&) throw(Entception&);
	
	virtual ~Sqlite3PersistenceApi() {}

	void setDb(sqlite3* db) { db_ = db; }

private:
	sqlite3* db_;
};


}	// End namespace ent
}	// End namespace tdk

#endif
