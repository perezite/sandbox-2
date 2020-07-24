#include "LibraryV2.h"
#include "OtherLibraryV1.h"
#include <iostream>

using namespace std;

int main()
{
	v2::Library library;
	v1::OtherLibrary otherLibrary;
	
	library.sayHello();
	otherLibrary.sayHello();

	cin.get();

	return 0;
}
