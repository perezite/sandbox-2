#include "Demo1.h"
#include "Demo2.h"
#include "Demo3.h"
#include "Demo4.h"
#include "Demo5.h"
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
	reflectionDemo5::run();
	//reflectionDemo4::run();
	//reflectionDemo3::run();
	//reflectionDemo2::run();
	//reflectionDemo1::run();
	std::cout << "Done ..." << std::endl;
	std::cin.get();
	return 0;
}

// demo6: stream (write, read)
// demo7: combine
// demo8: abstracting reader/writer/editor
// demo9: content pointers to reflectable (write, read, edit)
// demo10: inheritance (write, read, edit)
// demo11: link pointers (write, raed, edit)
// demo12: combine
// demo13: macros
// demo14: simplified scene (write, read, edit)