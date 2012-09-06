#ifndef ENTITY_HPP
#define ENTITY_HPP
/*! \file	entity.hpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */

#include <deque>
#include "abstractproperty.hpp"
#include "persistenceapi.hpp"
#include "entception.hpp"

namespace tdk {
namespace ent {

/*! Entities essentially represent a row / record from a database table. This is
 * the record part of the active record pattern.
 * They have the ability to be saved, updated and deleted from their persistent
 * storage (which is usually a database).
 * Entity factories are responsible for setting their persistence.
 */
class Entity
{
public:
	/*! Create an entity.
	 * \param	entitytype	Name of the entity class. This must be a string literal
	 *			and can not change during the lifetime of the class.
	 */
	Entity(const char* entitytype) : persistence_(NULL), entitytype_(entitytype) {}

	/*! Save the entity. This calls the entities installed persistence API's save
	 * method.
	 * \return	Whether or not the entity was saved.
	 */
	bool save() throw(Entception&);
	
	/*! Update the entity in persistent storage. This calls the entities
	 * installed persistence API's update method.
	 *
	 * \param	updates	Collection of properties to update this entity with.
	 *
	 * \return	Whether or not the entity was updated with the supplied values,
	 *			also indicating that persistent storage was updated.
	 */
	bool update(const AbstractPropertyCollection& updates) throw(Entception&);
	
	/*! Load an entity of this entities name, which has matching properties.
	 * 
	 * \return	Whether or not this entity was updated with loaded data.
	 */
	bool load(const AbstractPropertyCollection& criteria) throw(Entception&);
	
	/*! Delete an entity of this entities name, which has matching properties,
	 * from persistent storage.
	 * \return	Whether or not an entity was deleted from persistence.
	 */
	bool del() throw(Entception&);
	
	/*! Set the persistence object that should be used when invoking the 
	 * methods of this entity that will interact with persistent storage
	 */
	void setPersistence(PersistenceApi* p) { persistence_ = p; }
	
	/*! Get the name of the entity */
	const char* entitytype() const { return entitytype_; }

	/* Attempt to get a property based on it's name.
	 * \param	propertyName	Name of the property to try and find.
	 * \return	Pointer to the abstract property.
	 * \retval	NULL	No property with the specified name could be found.
	 */
	AbstractProperty* operator [] (const char* propertyName) const;
	
	/*! Check whether or not the supplied property collection is a subset of
	 * this entity. For this to be true, this entity must have a prop with the
	 * same name for each property in the collection.
	 */
	bool isSubset(const AbstractPropertyCollection& props) const;
	
	typedef std::deque<AbstractProperty*> PropertyDeque;
	
	/*! Get access to the entity's complete set of properties. These are only
	 * availble as read only.
	 * If you need to write to a property, then you should have access to the
	 * concrete entity type, and can access the properties directly.
	 */
	const PropertyDeque& properties() const { return properties_; }	
	
	/*! Add a property to this entitiy.
	 * This is used to add arbitrary properties to an entity.
	 * It's main use is for performing loads/searches and updating an entity.
	 */
	void addProperty(AbstractProperty* p);
	
private:
	PersistenceApi* persistence_;
	const char* entitytype_;
	PropertyDeque properties_;
};

}	// End namespace ent
}	// End namespace tdk

#endif
