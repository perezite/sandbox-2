#include "Window.h"
#include "Input.h"
#include "Stopwatch.h"
#include "Music.h"
#include "Sound.h"
#include <iostream>

float getSeconds() {
	static sb::Stopwatch sw;
	return sw.getElapsedSeconds();
}

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	float elapsed = getSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

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
	sb::Music music;
	sb::Sound sound;
	
	music.loadFromAsset("Music/ukulele.ogg");
	music.play();
	sound.loadFromAsset("Sounds/ding.ogg");

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		if (sb::Input::isTouchGoingDown(1)) {
			sound.play();
		}
 
		window.clear(sb::Color(1, 1, 1, 1));
		window.display();
	}
}

int main() {
	version();

	demo0();

	return 0;
}
