#include "entception.hpp"

#include <iostream>

#include <stdio.h>

using namespace std;

int foo()
{
	throw Entception("Foo failed");
}

int bar()
{
	try {
		foo();
	} catch (Entception& e) {
		THROW_AGAIN(e, "Calling foo failed");
	}
	
	return 0;
}

int main(void)
{
	try {
		bar();
	} catch (Entception& e) {
		e.print();
	} catch (...) {
		cout << "catch all\n";
	}
}
