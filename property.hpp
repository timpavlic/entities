#ifndef PROPERTY_HPP
#define PROPERTY_HPP
/*! \file	entityfactory.hpp
 *
 * Copyright 2012. See COPYING for details.
 */

 
#include "abstractproperty.hpp"

/*! Template property class which inherits from the AbstractProperty class */
template<typename T>
struct Property : public AbstractProperty
{
	/*!Create a property, giving it a name and owner.
	 */
	Property(const char* name, Entity* owner) : AbstractProperty(name, owner) {}
	
	/*! Create a property, giving it a name, owner, and initial value.
	 */
	Property(const char* name, Entity* owner, const T& value) : AbstractProperty(name, owner), val_(value) {}
	
	/*! Get the value of the property. */
	const T& operator () () const { return val_; }
	const T& val() const { return val_; }
	
	/*! Set the value of the property. */
	void set(const T& newVal) { val_ = newVal; }
	
	/*! Override the virtual accept method, which will allow a visitor for the
	 * encapsulated type T to be used on this property.
	 */
	virtual bool accept(PropertyVisitorBase& pv) {
		// Call the protected static method.
		return acceptVisitor(val_, pv);
	}
	
private:
	T val_;
};


#endif
