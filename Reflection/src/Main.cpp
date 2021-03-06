#include "Demo1.h"
#include "Demo2.h"
#include "Demo3.h"
#include "Demo4.h"
#include "Demo5.h"
#include "Demo6.h"
#include "Demo7.h"
#include "Demo8.h"
#include "Demo9.h"
#include "Demo10.h"
#include "Logger.h"
#include <string>

static void version() {
	#ifdef _DEBUG
		std::string configuration = "Debug";
	#else
		std::string configuration = "Release";
	#endif	
	SB_MESSAGE("Reflection - Build: " << configuration << ", " << __DATE__ << ", " << __TIME__);
}

int main() 
{
	version();

	reflectionDemo10::run();
	//reflectionDemo9::run();
	//reflectionDemo8::run();
	//reflectionDemo7::run();
	//reflectionDemo6::run();
	//reflectionDemo5::run();
	//reflectionDemo4::run();
	//reflectionDemo3::run();
	//reflectionDemo2::run();
	//reflectionDemo1::run();
	std::cout << "Done ..." << std::endl;
	std::cin.get();
	return 0;
}

// demo11: combine everything
// demo12: macros (see Concept2.cpp)
// demo13: All-in-one demo
// demo14: simplified scene (write, read, edit)
// demo15: single header file
// demo16: repo and release on github