/** \file	sqlite3exmaple.cpp
 *
 * Example of how to use the Sqlite3EntityFactory and related classes.
 * Also shows you how to visit properties of your own data types.
 */
#include <stddef.h>
#include <string>
#include <iostream>

#include "entity.hpp"
#include "property.hpp"

#include "factories/sqlite3entityfactory.hpp"

using namespace std;
using namespace tdk::ent;

typedef enum {
	UNDISCLOSED,
	MALE,
	FEMALE,
} Gender;
namespace tdk {
namespace ent {
template<> class PersistenceTypeConversion<Gender> : public EnumHelper<Gender> {};
}	// End namespace ent
}	// End namespace tdk

/** Person entity. Assume we have a database with a table called "person" and
 * the fields "name" and "age".
 */
struct Person : public Entity
{
	Property<string>	name;
	Property<int>		age;
	Property<Gender>	gender;

	Person() : Entity("person"),
		name("name", this),
		age("age", this, 0),
		gender("gender", this, UNDISCLOSED)
	{}

};

/** @todo	How do I define how a std::string should be visited for sqlite3 */

int main()
{
	Sqlite3EntityFactory factory("people.db");

	Person* person = factory.create<Person>();

	person->name = std::string("Winnifred");
	//person->age = "71";
	person->gender = MALE;

	try {
		person->save();
	} catch (Entception& e) {
		cout << e << endl;
	}
}
