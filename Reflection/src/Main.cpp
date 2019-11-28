#include "Demo1.h"
#include "Demo2.h"
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
	reflectionDemo2::run();
	//reflectionDemo1::run();
	std::cin.get();
	return 0;
}

// primitive (write, read, edit)
// properties (https://stackoverflow.com/questions/4225087/c-like-properties-in-native-c)
// reflectable (write, read, edit)
// link pointers (write, raed, edit)
// content pointer (write, read, edit)
// stream (write, read)
// inheritance
// simplified scene
// write scene
// read scene
// edit scene