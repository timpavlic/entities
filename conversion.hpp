#ifndef PERSISTENCE_PRIMITIVE_CONVERSION_HPP
#define PERSISTENCE_PRIMITIVE_CONVERSION_HPP
/*! \file	conversion.hpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 *
 * This file contains the trait class definition used for converting user types
 * to one of the primitive types that persistences need to support.
 */
#include <string>

namespace tdk {
namespace ent {

/** Traits class used for converting user types to one of the 'primitive' types
 * that the Entity visitor classes can handle.
 */
template <typename UserType>
struct PersistenceTypeConversion;

/** Traits for converting to/from ints */
template <>
struct PersistenceTypeConversion<int>
{
	typedef int PrimitiveType;
	static PrimitiveType toPrimitive(int i) {
		return i;
	}
	static int fromPrimitive(int i) {
		return i;
	}
};

/** Traits for converting to/from std::strings */
template <>
struct PersistenceTypeConversion<std::string>
{
	typedef StringPrimitive PrimitiveType;
	static PrimitiveType toPrimitive(const std::string& str) {
		return StringPrimitive(str.c_str(), str.length());
	}
	static std::string fromPrimitive(const StringPrimitive& sp) {
		return std::string(sp.data(), sp.len());
	}
};

/** Enumeration helper template class which can be inherited from by your
 * trait class definition for enum types.
 *
 * Example:
 * ~~~{.cpp}
 * typedef enum {
 *     UNDISCLOSED,
 *     MALE,
 *     FEMALE,
 * } Gender;
 *
 * template<> class PersistenceTypeConversion<Gender> : public EnumHelper<Gender> {};
 * ~~~
 */
template<typename Enum>
struct EnumHelper
{
	typedef int PrimitiveType;
	static PrimitiveType toPrimitive(Enum enumVal) {
		return static_cast<int>(enumVal);
	}
	static Enum fromPrimitive(int i) {
		return static_cast<Enum>(i);
	}
};

}	// End namespace ent
}	// End namespace tdk

#endif
