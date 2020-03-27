#include "Asset.h"
#include "Logger.h"
#ifdef WIN32
	#include <Windows.h>
#endif
#include <algorithm>

namespace sb
{
	const char Asset::PathSeparator = '/';
	const std::string Asset::AssetFolderName = "Assets";

	std::string Asset::readAllText(const std::string & assetPath)
	{
		std::string filePath = getFilePath(assetPath);
		SDL_RWops* reader = SDL_RWFromFile(filePath.c_str(), "r");
		SDL_CHECK(reader);

		Sint64 fileSize = SDL_RWsize(reader);
		char* text = (char*)malloc((size_t)fileSize + 1);
		Sint64 totalSize = 0;
		Sint64 lastSize = 0;

		do
		{
			lastSize = SDL_RWread(reader, &text[totalSize], sizeof(char), (size_t)(fileSize - totalSize));
			totalSize += lastSize;
		} while (totalSize < fileSize && lastSize != 0);
		text[totalSize] = '\0';
		SDL_CHECK(SDL_RWclose(reader) == 0);
		SDL_CHECK(totalSize == fileSize); 

		return text;
	}

	std::string Asset::getFilePath(const std::string& assetPath) 
	{
		return combinePath({ getAssetFolderPath(), assetPath });
	}

	std::string Asset::getAssetFolderPath() 
	{
		#ifdef WIN32
			return combinePath({ getExecutableFilePath(), "..", "Assets" });		
		#elif defined(__ANDROID__)
			return "";
		#else
			#error os not supported
		#endif	
	}

	std::string Asset::combinePath(std::vector<std::string> parts)
	{
		std::string result;
		for (unsigned int i = 0; i < parts.size() - 1; i++)
		{
			if (parts[i].size() > 0)
			{
				result += parts[i];
				result += PathSeparator;
			}
		}

		result += parts[parts.size() - 1];

		return result;
	}

	#ifdef WIN32
		std::string Asset::getExecutableFilePath()
		{
			wchar_t filePath[MAX_PATH];

			// get executable file path
			HMODULE hModule = GetModuleHandle(NULL);
			SB_ERROR_IF(hModule == NULL, "GetModuleHandle() failed");
			GetModuleFileName(hModule, filePath, (sizeof(filePath)));

			// get file path as string
			std::wstring filePathAsWideString(filePath);
			std::string filePathAsString(filePathAsWideString.begin(), filePathAsWideString.end());

			// remove trailing file name
			size_t lastPathSeparatorPosition = filePathAsString.rfind('\\');
			std::string directoryPathAsString = filePathAsString.substr(0, lastPathSeparatorPosition);

			std::replace(directoryPathAsString.begin(), directoryPathAsString.end(), '\\', PathSeparator);
			
			return directoryPathAsString;
		}
	#endif
}
