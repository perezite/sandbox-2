#include <iostream>
#include "calculator/calc.h"

using namespace std;

#ifdef NDEBUG
	#define MY_RELEASE
#else
	#define MY_DEBUG
#endif

void printBuildConfiguration()
{
#if defined(MY_RELEASE)
	cout << "Release Build" << endl;
#elif defined(MY_DEBUG)
	cout << "Debug Build" << endl;
#endif
}

void printVisualStudioCppVersion()
{
	cout << "Visual Studio C++ Version: ";
#ifdef _MSVC_LANG
	if (_MSVC_LANG == 201703L) std::cout << "C++17\n";
	else if (_MSVC_LANG == 201402L) std::cout << "C++14\n";
	else if (_MSVC_LANG == 201103L) std::cout << "C++11\n";
	else if (_MSVC_LANG == 199711L) std::cout << "C++98\n";
	else std::cout << "pre-standard C++\n";
#endif
}

void printCppVersion()
{
	cout << "Standard C++ Version: ";
	if (__cplusplus == 201703L) std::cout << "C++17\n";
	else if (__cplusplus == 201402L) std::cout << "C++14\n";
	else if (__cplusplus == 201103L) std::cout << "C++11\n";
	else if (__cplusplus == 199711L) std::cout << "C++98\n";
	else std::cout << "pre-standard C++\n";
}

int main()
{
	printBuildConfiguration();
	printVisualStudioCppVersion();
	printCppVersion();

	cout << add(1, 2) << endl;

	cin.get();

	return 0;
}