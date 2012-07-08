/*! \file	entity.hpp
 *
 * Copyright 2012. See COPYING for details.
 */
 
#include "entity.hpp"
#include <cstring>

void Entity::addProperty(AbstractProperty* p)
{
	properties_.push_back(p);
}

bool Entity::save(void)
{
	if ( !persistence_ )	return false;
	
	return persistence_->save(*this);
}

bool Entity::update(const Entity& updated)
{
	if ( !persistence_ )	return false;
	
	return persistence_->update(*this, updated);
}

Entity* Entity::load(void)
{
	if ( !persistence_ )	return NULL;
	
	return persistence_->load(*this);
}

bool Entity::del(void)
{
	if ( !persistence_ )	return false;
	
	return persistence_->del(*this);
}

AbstractProperty* Entity::operator [] (const char* propName) const
{
	for ( unsigned int i=0; i<properties_.size(); ++i ) {
		if ( strcmp(properties_[i]->name(), propName) == 0 )	return properties_[i];
	}
	return NULL;
}
