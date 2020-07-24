#include "Logger.h"
#include "SDL.h"
#ifdef WIN32
	#include <Windows.h>
#endif

namespace 
{
	#ifdef WIN32
		void showDebugMessageBox(const std::string& error) {
			const SDL_MessageBoxButtonData buttons[] = {
				{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Exit" },
				{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Debug" }
			};

			const SDL_MessageBoxData messageboxdata = { SDL_MESSAGEBOX_ERROR,
				NULL, "An error happend", error.c_str(),
				SDL_arraysize(buttons), buttons, NULL
			};

			int result = -1;
			SDL_ShowMessageBox(&messageboxdata, &result);

			if (result == 1)
				__debugbreak();
		}
	#endif
}

namespace sb
{
	void logMessage(std::stringstream& stream) {
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", stream.str().c_str());
	}

	void logError(std::stringstream& stream) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", stream.str().c_str());
		handleError(stream);
	}

	void logWarning(std::stringstream& stream) {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", stream.str().c_str());
	}

	void handleError(std::stringstream& stream) {
		std::string error = stream.str();

		#if defined(WIN32) && defined(_DEBUG)
			showDebugMessageBox(error);
		#else	
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "A problem occured", error.c_str(), NULL);
		#endif	

			exit(0);
	}
}