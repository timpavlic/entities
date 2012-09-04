#ifndef ABSTRACT_PROPERTY_HPP
#define ABSTRACT_PROPERTY_HPP
/*! \file	abstractproperty.hpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */

#include <deque>
 
#include "propertyvisitor.hpp"

class Entity;

/*! The abstract property base class provides the interface required for
 * the templated property class.
 *
 * Properties are essentially a key/name and value pair. Therefore, they are all
 * required to have a name.
 *
 * This abstract base class also provides the virtual visit method. This is part
 * of the visitor pattern used to allow querying and assignment of properties
 * if all you have access to is the abstract base class. This is key to
 * implementing an entities persistence backend without needing to modify this
 * library's source.
 */
class AbstractProperty
{
public:
	virtual ~AbstractProperty() {}
	/*! Get the name of this property. This is the name that is assigned at
	 * construction.
	 */
	const char* propertyName() const { return name_; }

	/*! Accept a PropertyVisitorBase class that all property visitors should
	 * inherit from.
	 */
	virtual bool acceptReader(ReadVisitor&) = 0;
	virtual void acceptWriter(WriteVisitor&) = 0;
	
protected:
	/*! Construct a property with the given name.
	 * \param	name	Name of the property. This pointer must be valid for the
	 *			life of the property, and as such, string literals are
	 *			recommended.
	 * \param	owner	Entity that this property belongs to. This property will
	 *			be added to the Entity's list of properties.
	 */
	AbstractProperty(const char* name, Entity* owner);
	
	/*! Construct a property with the given name, that has no owner.
	 * 
	 * \param	name	Name of the property. See other constructor for detail.
	 *
	 * Un-owned properties are used for loading and updating entities, where
	 * all the properies of the entity to be updated / loaded are not known.
	 */
	AbstractProperty(const char* name) : name_(name) {}
	
#if 0
	/*! Protected static method available for concrete Property classes to call.
	 */
	/*
	template <typename PrimitiveType>
	static bool acceptVisitor(PrimitiveType& val, PropertyVisitor& visitor)
	*/
	template <typename EncapsulatedType>
	static bool acceptVisitor(EncapsulatedType& val, PropertyVisitorBase& visitor)
	{
		//return visitor.visit(val);

		PropertyVisitor<EncapsulatedType>* pv;
		pv = dynamic_cast< PropertyVisitor<EncapsulatedType>* >(&visitor);
		
		if ( pv ) {
			return pv->visit(val);
		}

		return false;
	}
#endif
	
private:
	const char* name_;
};

/*! Abstract property collection object. This is required for the persistence
 * API. It is used to create a collection of properties, not under the ownership
 * of an entity.
 * 
 * \see	PropertyCollection
 */
struct AbstractPropertyCollection 
{
	const std::deque<AbstractProperty*>& props() const { return props_; }

protected:
	std::deque<AbstractProperty*> props_;
	AbstractPropertyCollection() {}
};

#endif
