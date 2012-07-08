/*! \file	abstractproperty.cpp
 *
 * Copyright 2012. See COPYING for details.
 */

#include "abstractproperty.hpp"
#include "entity.hpp"

// Protected ctor
AbstractProperty::AbstractProperty(const char* name, Entity* owner) : name_(name)
{
	if ( !owner )	return;
	
	owner->addProperty(this);
}
