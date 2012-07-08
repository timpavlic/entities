#ifndef ABSTRACT_PROPERTY_HPP
#define ABSTRACT_PROPERTY_HPP
/*! \file	abstractproperty.hpp
 *
 * Copyright 2012. See COPYING for details.
 */

#include <stdio.h>
 
#include "propertyvisitor.hpp"

struct Entity;

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
struct AbstractProperty
{
	virtual ~AbstractProperty() {}
	/*! Get the name of this property. This is the name that is assigned at
	 * construction.
	 */
	const char* name() const { return name_; }
	
	/*! Accept a PropertyVisitorBase class that all property visitors should
	 * inherit from.
	 */
	virtual bool accept(PropertyVisitorBase&) {
		return false;
	}
	
protected:
	/*! Construct a property with the given name.
	 * \param	name	Name of the property. This pointer must be valid for the
	 *			life of the property, and as such, string literals are
	 *			recommended.
	 * \param	owner	Entity that this property belongs to. This property will
	 *			be added to the Entity's list of properties.
	 */
	AbstractProperty(const char* name, Entity* owner);
	
	/*! Protected static method available for concrete Property classes to call.
	 */
	template <typename EncapsulatedType>
	static bool acceptVisitor(EncapsulatedType& val, PropertyVisitorBase& visitor)
	{
		PropertyVisitor<EncapsulatedType>* pv;
		pv = dynamic_cast< PropertyVisitor<EncapsulatedType>* >(&visitor);
		
		if ( pv ) {
			return pv->visit(val);
		}
		return false;
	}
	
	
	const char* name_;
};


#endif
