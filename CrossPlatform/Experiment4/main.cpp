#include <iostream>
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

	auto test = 42;
	cout << "test" << endl;

	cin.get();
}