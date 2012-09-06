#ifndef SQLITE3_ENTITY_FACTORY_HPP
#define SQLITE3_ENTITY_FACTORY_HPP
/*! \file	sqlite3entityfactory.hpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */

#include "entities/entityfactory.hpp"
#include "entities/factories/sqlite3persistenceapi.hpp"

struct sqlite3;

namespace tdk {
namespace ent {

/*! Entity factory that installs an SQLite3 persistence API in to the entities
 * it creates.
 */
class Sqlite3EntityFactory : public EntityFactory
{
public:
	/*! Create an SQLite3 entitiy factory.
	 *
	 * \param	dbFile	Name of the database file to open and operate on.
	 *
	 * \throw	std::runtime_exception	If the database file can not be opened.
	 */
	Sqlite3EntityFactory(const char* dbFile) throw(Entception&);

	~Sqlite3EntityFactory();
	
private:
	virtual void installPersistenceApi(Entity* e);
	Sqlite3PersistenceApi persistence_;
	sqlite3* db_;
};

}	// End namespace ent
}	// End namespace tdk

#endif
