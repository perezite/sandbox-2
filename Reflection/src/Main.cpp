#include "Demo1.h"
#include "Demo2.h"
#include "Demo3.h"
#include "Demo4.h"
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
	reflectionDemo4::run();
	//reflectionDemo3::run();
	//reflectionDemo2::run();
	//reflectionDemo1::run();
	std::cin.get();
	return 0;
}

// demo5: reflectable (write, read, edit)
// demo6: content pointers to reflectable (write, read, edit)
// demo7: inheritance (write, read, edit)
// demo8: stream (write, read)
// demo9: link pointers (write, raed, edit)
// demo10: simplified scene (write, read, edit)