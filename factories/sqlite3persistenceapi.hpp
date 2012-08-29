#ifndef SQLITE3_ENTITY_PERSISTENCE_HPP
#define SQLITE3_ENTITY_PERSISTENCE_HPP
/*! \file	sqlite3persistenceapi.hpp
 *
 * Copyright 2012. See COPYING for details.
 */

#include "entity.hpp"

/*! Persistence implementation for SQLite 3. */
struct Sqlite3PersistenceApi : public PersistenceApi
{
	virtual bool save(const Entity&) throw(Entception&);
	
	virtual bool update(const Entity& ent, const AbstractPropertyCollection& updates) throw(Entception&);
	
	virtual bool load(Entity& ent, const AbstractPropertyCollection& criteria) throw(Entception&);
	
	virtual bool del(const Entity&) throw(Entception&);
	
private:
	sqlite3* db_;
};

#endif
