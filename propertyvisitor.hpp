#ifndef PROPERTY_VISITOR_HPP
#define PROPERTY_VISITOR_HPP
/*! \file	propertyvisitor.hpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 *
 * To be able to set and get the values contained within a property, when only
 * an AbstractProperty* is available, the visitor pattern is used to perform
 * a double dispatch.
 */
#include <string>

namespace tdk {
namespace ent {

/** The only string primitive supported is a custom class that has a pointer
 * and length.
 */
class StringPrimitive
{
public:
	StringPrimitive() : data_(NULL), len_(0) {}
	StringPrimitive(const char* str, size_t len) : data_(str), len_(len) {}
	const char* data() const { return data_; }
	size_t len() const { return len_; }
private:
	const char* data_;
	size_t len_;
};

/** And a 16bit version for widestrings / UTF16 */
class String16Primitive
{
public:
	String16Primitive() : data_(NULL), len_(0) {}
	String16Primitive(const wchar_t* str, size_t len) : data_(str), len_(len) {}
	const wchar_t* data() const { return data_; }
	size_t len() const { return len_; }
private:
	const wchar_t* data_;
	size_t len_;
};

/** Visitor class used for reading from properties. */
class ReadVisitor
{
public:
	virtual bool visit(const bool& b) = 0;
	virtual bool visit(const char& c) = 0;
	virtual bool visit(const int& i) = 0;
	virtual bool visit(const unsigned int& ui) = 0;
	virtual bool visit(const double& d) = 0;
	virtual bool visit(const StringPrimitive& str) = 0;

	virtual ~ReadVisitor() {}
};

/** Visitor class used for writing to properties. */
class WriteVisitor
{
public:
	virtual void visit(bool& b) = 0;
	virtual void visit(char& c) = 0;
	virtual void visit(int& i) = 0;
	virtual void visit(unsigned int& ui) = 0;
	virtual void visit(StringPrimitive& str) = 0;

	virtual ~WriteVisitor() {}
};

}	// End namespace ent
}	// End namespace tdk

#endif
