#include "Demo1.h"
#include "Demo2.h"
#include "Demo3.h"
#include "Demo4.h"
#include "Demo5.h"
#include "Demo6.h"
#include "Demo7.h"
#include "Demo8.h"
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

	reflectionDemo8::run();
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

// demo8: link pointers (write, raed, edit)
// demo9: inheritance (write, read, edit)
// demo10: combine
// demo11: macros
// demo12: init method
// demo13: combine
// demo14: simplified scene (write, read, edit)