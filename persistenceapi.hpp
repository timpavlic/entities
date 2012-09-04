#ifndef PERSISTENCE_API_HPP
#define PERSISTENCE_API_HPP
/*! \file	entityfactory.hpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */

#include "entception.hpp"

class Entity;

/*! API for defining how entities can be saved, loaded and deleted from
 * persistent storage.
 */
class PersistenceApi
{
public:
	/*! Save the given entity to persistant storage. Implementors will assume
	 * that this means that the entity has never been saved before.
	 *
	 * \param	e	Entity to save to persistant storage.
	 *
	 * \return	Whether or not the entity could be saved.
	 *
	 * \throws	Entception		Implementations should throw an Entception for
	 *			any errors.
	 */
	virtual bool save(const Entity& e) throw (Entception&)= 0;
	
	/*! Update the given entity in persistent storage. Implementors will assume
	 * that the entity has been saved before, but it just needs updating.
	 *
	 * \param	ent		The entity to update. This exact entity has to be found.
	 * \param	updates		The new updates to apply to the entity, if found.
	 *
	 * \return	Whether or not the entity was updated.
	 *
	 * \throws	Entception		Implementations should throw an Entception for
	 *			any logic errors.
	 */
	virtual bool update(const Entity& ent, const AbstractPropertyCollection& updates) throw(Entception&) = 0;
	
	/*! Load an entity from persistent storage, using a collection of properties
	 * as the criteria for loading it.
	 *
	 * \param[out]	ent		Entity object to load data in to.
	 * \param	criteria	Collection of proerties to base loading off of.
	 *
	 * \return	Whether or not an entity could be loaded.
	 */
	virtual bool load(Entity& ent, const AbstractPropertyCollection& criteria) throw(Entception&) = 0;
	
	/*! Delete the given entity from persistant storage.
	 * \param	e	Entity to base deletion off of. Entity name and properties
	 *			will be used.
	 * \return	Whether or not such an entity was deleted.
	 */
	virtual bool del(const Entity& e) throw(Entception&) = 0;

	virtual ~PersistenceApi() {}
};

#endif
