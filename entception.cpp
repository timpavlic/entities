/*! \file	sqlite3persistenceapi.cpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 */
#include "entception.hpp"

#include "entity.hpp"

#include <stdio.h>
#include <string.h>

#undef Entception

namespace tdk {
namespace ent {

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

std::ostream& operator << (std::ostream& stream, const Entception& e)
{
	for ( unsigned int i = 0; i < e.traces_.size(); ++i ) {
		stream << e.traces_[i].file << ":" << e.traces_[i].lineNo << " - ";
		stream << e.traces_[i].msg << std::endl;
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

void SaveEntception::buildmsg(const char* file, int lineNo, const Entity* ent, const char* msg)
{
	std::string str("Failed to save a '");
	str += ent->entitytype();
	str += "' entitiy: ";
	str += msg;
	
	add(file, lineNo, str);
}

void UpdateEntception::buildmsg(const char* file, int lineNo, const Entity* ent, const char* msg)
{
	std::string str("Failed to update a '");
	str += ent->entitytype();
	str += "' entitiy: ";
	str += msg;
	
	add(file, lineNo, str);
}

void LoadEntception::buildmsg(const char* file, int lineNo, const Entity* ent, const char* msg)
{
	std::string str("Failed to load a '");
	str += ent->entitytype();
	str += "' entitiy: ";
	str += msg;
	
	add(file, lineNo, str);
}

void DelEntception::buildmsg(const char* file, int lineNo, const Entity* ent, const char* msg)
{
	std::string str("Failed to delete a '");
	str += ent->entitytype();
	str += "' entitiy: ";
	str += msg;
	
	add(file, lineNo, str);
}

}	// End namespace ent
}	// End namespace tdk
