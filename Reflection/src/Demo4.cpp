#include "Demo4.h"
#include "Macro.h"
#include <string>
#include <iostream>

namespace reflectionDemo4 {

	class Reflection100 {
		static std::string CurrentInspectorName;
	public:
		static void setCurrentInspector(const std::string& inspectorName) { CurrentInspectorName = inspectorName; }
		static const std::string& getCurrentInspector() { return CurrentInspectorName; }

	};

	std::string Reflection100::CurrentInspectorName;

	class TextReader { };
	class TextWriter { };
	class ConsoleEditor { };

	void demo100() {
		Reflection100::setCurrentInspector(SB_NAMEOF(TextReader));
		std::cout << Reflection100::getCurrentInspector() << std::endl;
		Reflection100::setCurrentInspector(SB_NAMEOF(TextWriter));
		std::cout << Reflection100::getCurrentInspector() << std::endl;
		Reflection100::setCurrentInspector(SB_NAMEOF(ConsoleEditor));
		std::cout << Reflection100::getCurrentInspector() << std::endl;
	}

	

	void run() {
		demo100();
	}

	// demo4: switchable inspector
}
