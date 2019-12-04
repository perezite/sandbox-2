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
// demo6: combine
// demo7: content pointers to reflectable (write, read, edit)
// demo8: inheritance (write, read, edit)
// demo9: stream (write, read)
// demo10: link pointers (write, raed, edit)
// demo11: combine
// demo12: simplified scene (write, read, edit)