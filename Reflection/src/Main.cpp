#include "Demo1.h"
#include "Demo2.h"
#include "Logger.h"
#include <string>

void version() {
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
	demo2::run();
	//demo1::run();
	std::cin.get();
	return 0;
}

// simple scene type
// inheritance
// deserialize
// editor
// save / load demo
// console editor demo