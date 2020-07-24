#include "Game.h"
#include "Demo.h"
#include "Logger.h"
#include "Asset.h"
#include <algorithm>
#ifdef WIN32
	#include <Windows.h>
#endif

void hideConsole() {
	#if defined(WIN32) && !defined (_DEBUG)
		if (sb::Asset::exists("Configuration/Config.cfg")) {
			std::string config = sb::Asset::readAllText("Configuration/Config.cfg");
			config.erase(std::remove_if(config.begin(), config.end(), isspace), config.end());
			if (config.find("hideConsole=true") != std::string::npos)
				ShowWindow(GetConsoleWindow(), SW_HIDE);
		}
	#endif
}

void instructions() {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Controls", 
		"Desktop:\n"
		"Move: Left, Right, Down Arrows\n"
		"Rotate: Up Arrow\n"
		"Quickdrop: Spacebar\n\n"
		"Mobile:\n"
		"Move: Drag\n"
		"Rotate: Swipe up\n",
		NULL);

}

void version() {
	#ifdef _DEBUG
		std::string configuration = "Debug";	
	#else
		std::string configuration = "Release";
	#endif

	SB_MESSAGE("Blocks (" << configuration << "): " << __DATE__ << ", " << __TIME__);
}

int main() {
	#ifdef WIN32
		hideConsole();
	#endif

	instructions();

	version();

	game::game();
	
	//demo();
	
	return 0;
}
