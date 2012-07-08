#ifndef PERSISTENCE_API_HPP
#define PERSISTENCE_API_HPP
/*! \file	entityfactory.hpp
 *
 * Copyright 2012. See COPYING for details.
 */


struct Entity;

/*! API for defining how entities can be saved, loaded and deleted from
 * persistent storage.
 */
struct PersistenceApi
{
	/*! Save the given entity to persistant storage. Implementors will assume
	 * that this means that the entity has never been saved before.
	 * \param	e	Entity to save to persistant storage.
	 * \return	Whether or not the entity could be saved. If it fails, perhaps
	 *			all the regular properties are required, depending on implementation.
	 */
	virtual bool save(const Entity& e) = 0;
	
	/*! Update the given entity in persistent storage. Implementors will assume
	 * that the entity has been saved before, but it just needs updating.
	 * \param	old		The old entity to update.
	 * \param	updated		The new entity that has updated data.
	 * \return	Whether or not the entity was updated.
	 */
	virtual bool update(const Entity& old, const Entity& updated) = 0;
	
	/*! Load the given entity from persistent storage.
	 * \param	e	Entity of a given name, containing some properties to base
	 *			the loading off of.
	 * \return	A newly allocated entity of the same name, with all the fields
	 *			an entity of that name usually has.
	 */
	virtual Entity* load(const Entity& e) = 0;
	
	/*! Delete the given entity from persistant storage.
	 * \param	e	Entity to base deletion off of. Entity name and properties
	 *			will be used.
	 * \return	Whether or not such an entity was deleted.
	 */
	virtual bool del(const Entity& e) = 0;
};

#endif
