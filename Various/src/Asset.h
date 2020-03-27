#include "SDL.h" 
#include <string>
#include <vector>

namespace sb
{
	class Asset {
	public:
		static std::string readAllText(const std::string& assetPath);

		static std::string getFilePath(const std::string& assetPath);
	protected:

		static std::string getAssetFolderPath();

		static std::string combinePath(std::vector<std::string> parts);

		#ifdef WIN32
			static std::string getExecutableFilePath();
		#endif

	private:
		static const char PathSeparator;

		static const std::string AssetFolderName;
	};
}
