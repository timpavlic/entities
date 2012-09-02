#ifndef ENTITIES_EXCEPTIONS_HPP
#define ENTITIES_EXCEPTIONS_HPP
/*! \file	exceptions.hpp
 *
 * \copyright	Copyright 2012. See COPYING for details.
 *
 * Declaration of the exception classes that are used in the entity library.
 */
#include <string>
#include <deque>
#include <iostream>

struct Entity;

/*! Exception base class for the entities library. It is named Entception as
 * a portmanteau of Entity and Exception. This should avoid any collisions with
 * other libraries that define a base Exception class.
 *
 * This exception class keeps a collection of pseudo-stack-traces, allowing
 * for better debugging of the whole chain of events when modifying an entity
 * in a persistent store.
 */
struct Entception
{
	/*! Create an entity exception object, with the file and line number that
	 * the exception is occurring at, along with a useful message.
	 *
	 * \param	file	File the exception has occurred in. Use __FILE__.
	 * \param	lineNo	Line number the exception has occurred at. Use __LINE__.
	 * \param	msg		Useful debug message.
	 */
	Entception(const char* file, int lineNo, const char* msg);
	Entception(const char* file, int lineNo, const std::string& msg);
	
	/*! Make this entity exception throw itself, but add another level of trace.
	 *
	 * \see	Entception constructor for parameter description.
	 *
	 * \throws	Entception. The exception is actually itself: *this.
	 */
	void throwAgain(const char* file, int lineNo, const char* msg); //throw(Entception&)
	
	/*! Print all the trace information to std::cout */
	void print() const;

	
protected:
	// Protected constructor for use by subclasses.
	Entception() {}
	
	// Function used to add a trace to the exception.
	void add(const char* file, int lineNo, const std::string& msg);
	void add(const char* file, int lineNo, const char* msg);

private:
	
	struct Trace {
		const char* file;
		int lineNo;
		std::string msg;
	};
	
	std::deque<Trace> traces_;
	// Although a pointer helps conserve memory through allocations, we would
	// need a shared pointer to ensure that the traces are valid for all objects.
	// I don't know if I want to burden the end user with that, plus, exceptions
	// should only happen during devel.
	//std::shared_ptr< std::deque<Trace> > traces_;	// Shared pointer to a deque of trace information.

	friend std::ostream& operator << (std::ostream&, const Entception& e);
};

// Standard ostream is not what we want.
std::ostream& operator << (std::ostream& stream, const Entception& e);

/*! Macro for creating an Entception. It uses the same name as the class, but I
 * consider this to be alright as it makes the class easy to use, meaning it
 * will then actually get used.
 */
#define Entception(msg) Entception(__FILE__, __LINE__, msg)

/*! Handy macro to make throwing an entity exception again, easier. */
#define THROW_AGAIN(entception, msg)	entception.throwAgain(__FILE__, __LINE__, msg)

/*! Entity exception for save operations. This exception simply adds extra text
 * to the initial exception message, indicating that a save operation on the
 * specified entity type failed.
 */
struct SaveEntception : public Entception
{
	/*! Create a save entity exception.*/
	SaveEntception(const char* file, int lineNo, const Entity* ent, const char* msg) {
		buildmsg(file, lineNo, ent, msg);
	}
	SaveEntception(const char* file, int lineNo, const Entity* ent, const std::string& msg) {
		buildmsg(file, lineNo, ent, msg.c_str());
	}

private:
	void buildmsg(const char* file, int lineNo, const Entity* ent, const char* msg);
};

/*! Macro for creating an entity save exception. Supplies the file and line
 * number, requiring only the entity and message to be supplied. */
#define SaveEntception(ent, msg)	SaveEntception(__FILE__, __LINE__, ent, msg)


/*! Entity exception for update operations. This exception simply adds extra
 * text to the supplied message, to indicate that it was an update that failed.
 */
struct UpdateEntception : public Entception
{
	/*! Create an update entception. */
	UpdateEntception(const char* file, int lineNo, const Entity* ent, const char* msg) {
		buildmsg(file, lineNo, ent, msg);
	}
	UpdateEntception(const char* file, int lineNo, const Entity* ent, const std::string& msg) {
		buildmsg(file, lineNo, ent, msg.c_str());
	}

private:
	void buildmsg(const char* file, int lineNo, const Entity* ent, const char* msg);
};

#define UpdateEntception(ent, msg)	UpdateEntception(__FILE__, __LINE__, ent, msg)


/*! Entity exception for load operations. The exception simply adds extra text
 * to the supplied message to indicate that it was a load that failed.
 */
struct LoadEntception : public Entception
{
	/*! Create a load entception. */
	LoadEntception(const char* file, int lineNo, const Entity* ent, const char* msg) {
		buildmsg(file, lineNo, ent, msg);
	}
	LoadEntception(const char* file, int lineNo, const Entity* ent, const std::string& msg) {
		buildmsg(file, lineNo, ent, msg.c_str());
	}

private:
	void buildmsg(const char* file, int lineNo, const Entity* ent, const char* msg);
};

#define LoadEntception(ent, msg)	LoadEntception(__FILE__, __LINE__, ent, msg)


/*! Entity exception for delete operations. This exception simply adds extra
 * text to the supplied message to indicate that it was a delete that failed.
 */
struct DelEntception : public Entception
{
	/*! Create a delete entity exception. */
	DelEntception(const char* file, int lineNo, const Entity* ent, const char* msg) {
		buildmsg(file, lineNo, ent, msg);
	}
	DelEntception(const char* file, int lineNo, const Entity* ent, const std::string& msg) {
		buildmsg(file, lineNo, ent, msg.c_str());
	}

private:
	void buildmsg(const char* file, int lineNo, const Entity* ent, const char* msg);
};

#define DelEntception(ent, msg)		DelEntception(__FILE__, __LINE__, ent, msg)


#endif
