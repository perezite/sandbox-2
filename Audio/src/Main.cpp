#include "Window.h"
#include "Input.h"
#include "Stopwatch.h"
#include "Triangle.h"
#include <iostream>

void version() {
	#ifdef _DEBUG
		std::string configuration = "Debug";
	#else
		std::string configuration = "Release";
	#endif	
	SB_MESSAGE("Audio - Build: " << configuration << ", " << __DATE__ << ", " << __TIME__);
}

void demo0() {
	sb::Window window;
	sb::Triangle triangle;

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
 
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(triangle);
		window.display();
	}
}

int main() {
	version();

	demo0();

	return 0;
}
