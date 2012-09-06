/*! \file	entity.hpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */
 
#include "entity.hpp"
#include <cstring>

namespace tdk {
namespace ent {

void Entity::addProperty(AbstractProperty* p)
{
	properties_.push_back(p);
}

bool Entity::save(void) throw(Entception&)
{
	if ( !persistence_ ) {
		throw SaveEntception(this, "No persistence installed");
	}
	
	try {
		return persistence_->save(*this);
	} catch (Entception& e) {
		THROW_AGAIN(e, "Persistence failed to save.");
	}

	// We can't actually get here, but the compiler can't work that out, so we
	// need this return statement to avoid a warning.
	return false;
}

bool Entity::update(const AbstractPropertyCollection& updates) throw(Entception&)
{
	if ( !persistence_ )	return false;
	
	return persistence_->update(*this, updates);
}

bool Entity::load(const AbstractPropertyCollection& criteria) throw(Entception&)
{
	if ( !persistence_ )	return NULL;
	
	return persistence_->load(*this, criteria);
}

bool Entity::del(void) throw(Entception&)
{
	if ( !persistence_ )	return false;
	
	return persistence_->del(*this);
}

AbstractProperty* Entity::operator [] (const char* propName) const
{
	for ( unsigned int i=0; i<properties_.size(); ++i ) {
		if ( strcmp(properties_[i]->propertyName(), propName) == 0 )	return properties_[i];
	}
	return NULL;
}

bool Entity::isSubset(const AbstractPropertyCollection& collection) const
{
	const PropertyDeque& prps = collection.props();
	
	// Iterate through all the props in the collection.
	for ( unsigned int i = 0; i < prps.size(); ++i ) {
	
		// ... And check to see if it exists in this entity's properties.
		unsigned int j;
		for ( j = 0; j < properties_.size(); ++j ) {
			if ( strcmp(properties_[j]->propertyName(), prps[i]->propertyName()) == 0 )	break;	// Found
		}
		if ( j >= properties_.size() ) {
			return false;	// Property was not found in entity.
		}
	}
	
	return true;
}

}	// End namespace ent
}	// End namespace tdk
