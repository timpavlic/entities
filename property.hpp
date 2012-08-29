#ifndef PROPERTY_HPP
#define PROPERTY_HPP
/*! \file	entityfactory.hpp
 *
 * Copyright 2012. See COPYING for details.
 */

 
#include "abstractproperty.hpp"

/*! Template property class which inherits from the AbstractProperty class.
 * \tparam	Data type to wrap up and assign a name against.
 */
template<typename T>
struct Property : public AbstractProperty
{
	/*!Create a property, giving it a name and owner.
	 */
	Property(const char* name, Entity* owner) : AbstractProperty(name, owner) {}
	
	/*! Create a property, giving it a name, owner, and initial value.
	 */
	Property(const char* name, Entity* owner, const T& value) : AbstractProperty(name, owner), val_(value) {}

	/*! Create a property, giving it a name, but no owner. */
	Property(const char* name) : AbstractProperty(name) {}
	
	/*! Create a property, giving it a name and initial value, but no owner. */
	Property(const char* name, const T& value) : AbstractProperty(name), val_(value) {}
	
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


/*! The PropertyCollection class is used to create a collection properties from
 * entities. These properties take the name (and optionally, value) from the
 * another property, but do not add themselves to the owner of said property.
 * Property collections are important for persistence, as it allows loading and
 * updating to be performed with incomplete entity data.
 */
struct PropertyCollection : public AbstractPropertyCollection
{
	/*! Add a property to the collection, keeping it's name but assigning it a
	 * new value.
	 *
	 * \tparam	T	Data type this method will be expecting a Property<T> of.
	 *
	 * \param	prop	Property to add to the collection, copying the name.
	 * \param	newVal	Value to assign to the property.
	 */
	template <typename T>
	void add(Property<T>& prop, const T& newVal) {
		Property<T>* p = new Property<T>( prop.name(), newVal );
		props_.push_back(p);
	}
	
	/*! Add a property to the collection, keeping it's name and value.
	 * \tparam	T	Data type this method will be expecting a Property<T> of.
	 * \param	prop	Property to add to the collection, copying the name and value.
	 */
	template <typename T>
	void add(Property<T>& prop) {
		Property<T>* p = new Property<T>( prop.name(), prop() );
		props_.push_back(p);
	}
	
	/*! Destroy a property collection, deleting all the properties it contains. */
	~PropertyCollection() {
		for ( size_t i = 0; i < props_.size(); ++i ) {
			delete props_[i];
		}
	}

};
#endif
