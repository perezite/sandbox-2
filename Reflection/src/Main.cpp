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

// demo2: primitive (write, read, edit)
// demo3: primtive content pointer (write, read, edit)
// demo4: reflectable (write, read, edit)
// demo5: reflectable content pointer (write, read, edit)
// demo6: inheritance (write, read, edit)
// demo7: stream (write, read)
// demo8: link pointers (write, raed, edit)
// demo9: simplified scene (write, read, edit)