#include <iostream>
#include "calculator/calc.h"

using namespace std;

#ifdef NDEBUG
	#define MY_RELEASE
#else
	#define MY_DEBUG
#endif

void main()
{
	#if defined(MY_RELEASE)
		cout << "Release Build" << endl;
	#elif defined(MY_DEBUG)
		cout << "Debug Build" << endl;
	#endif

	cout << add(1, 2) << endl;

	cin.get();
}