/*! \file	sqlite3persistenceapi.cpp
 *
 * Copyright 2012. See COPYING for details.
 */
#include "entception.hpp"

#include "entity.hpp"

#include <stdio.h>
#include <string.h>

#undef Entception

Entception::Entception(const char* file, int lineNo, const char* msg)
{
	add(file, lineNo, msg);
}

Entception::Entception(const char* file, int lineNo, const std::string& msg)
{
	add(file, lineNo, msg);
}

void Entception::throwAgain(const char* file, int lineNo, const char* msg)
{
	add(file, lineNo, msg);
	throw *this;
}
	
std::ostream& Entception::operator << (std::ostream& stream) const
{
	for ( unsigned int i = 0; i < traces_.size(); ++i ) {
		stream << traces_[i].file << ":" << traces_[i].lineNo << " - ";
		stream << traces_[i].msg << std::endl;
	}
	return stream;
}
	
void Entception::print() const
{
	for ( unsigned int i = 0; i < traces_.size(); ++i ) {
		std::cout << traces_[i].file << ":" << traces_[i].lineNo << " - ";
		std::cout << traces_[i].msg << std::endl;
	}
}

void Entception::add(const char* file, int lineNo, const std::string& msg)
{
	Trace t;
	t.file = file;
	t.lineNo = lineNo;
	t.msg = msg;
	traces_.push_back(t);
}

void Entception::add(const char* file, int lineNo, const char* msg)
{
	Trace t;
	t.file = file;
	t.lineNo = lineNo;
	t.msg = msg;
	traces_.push_back(t);
}

#undef SaveEntception
SaveEntception::SaveEntception(const char* file, int lineNo, const Entity* ent, const char* msg)
{
	std::string str("Failed to save a '");
	str += ent->entitytype();
	str += "' entitiy: ";
	str += msg;
	
	add(file, lineNo, str);
}

#undef UpdateEntception
UpdateEntception::UpdateEntception(const char* file, int lineNo, const Entity* ent, const char* msg)
{
	std::string str("Failed to update a '");
	str += ent->entitytype();
	str += "' entitiy: ";
	str += msg;
	
	add(file, lineNo, str);
}

#undef LoadEntception
LoadEntception::LoadEntception(const char* file, int lineNo, const Entity* ent, const char* msg)
{
	std::string str("Failed to load a '");
	str += ent->entitytype();
	str += "' entitiy: ";
	str += msg;
	
	add(file, lineNo, str);
}

#undef DelEntception
DelEntception::DelEntception(const char* file, int lineNo, const Entity* ent, const char* msg)
{
	std::string str("Failed to delete a '");
	str += ent->entitytype();
	str += "' entitiy: ";
	str += msg;
	
	add(file, lineNo, str);
}
