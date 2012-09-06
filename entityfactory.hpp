#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP
/*! \file	entityfactory.hpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */

#include "entity.hpp"
#include "persistenceapi.hpp"

namespace tdk {
namespace ent {

/*! The entity factory is a base class defining a factory that can create
 * entities.
 */
class EntityFactory
{
public:
	/*! Create an entity of the templated entity type.
	 * This entity will have a persistence API installed in to it.
	 *
	 * \tparam	Ent		Entity type to create.
	 *
	 * \result	New entity with persistence.
	 */
	template <typename Ent>
	Ent* create() {
		Ent* ent = new Ent;
		installPersistenceApi(ent);
		return ent;
	}

	virtual ~EntityFactory() {}

protected:
	// Can not instantiate an entity factory.
	EntityFactory() {}

	/*! All EntityFactory subclasses must implement a method which can install
	 * their persistence into created entities. This will be called from the
	 * create method.
	 * \param	e	Entity to install the persistence in to.
	 */
	virtual void installPersistenceApi(Entity* e) = 0;
};

}	// End namespace ent
}	// End namespace tdk

#endif
