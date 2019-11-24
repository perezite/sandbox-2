#pragma once
#include "BaseReflectable.h"
#include "InheritanceCheck.h"
#include "Logger.h"
#include "Macro.h"
#include <string>
#include <sstream>

namespace sb {
	namespace reflection {
		static void setInspector(const std::string& inspectorTypeName_);
	}
}

namespace sb {
	class TextSerializer {
		static std::string _result;

	protected:
		static void serializeReflectable(BaseReflectable& reflectable, size_t depth) {
			auto properties = reflectable.getProperties();
			for (size_t i = 0; i < properties.size(); i++)
				properties[i]->inspect(depth + 1);
		}

		template <class T>
		static void serializePrimitive(T& t, size_t depth) {
			std::ostringstream os; os << std::string(depth, ' ') << t << std::endl;
			_result += os.str();
		}

	public:
		template <class T>
		static void inspect(T& t, size_t depth = 0) {
			if (sb::InheritanceCheck<T, sb::BaseReflectable>::value()) {
				serializeReflectable((BaseReflectable&)t, depth);
				return;
			}

			SB_ERROR("The current inspector does not support this type");
		}

		template <class T>
		static std::string serialize(T& t, size_t depth = 0) {
			sb::reflection::setInspector(SB_NAMEOF(sb::TextSerializer));
			_result = std::string();
			inspect<T>(t, depth);
			return _result;
		}
	};

	template <>
	void TextSerializer::inspect<int>(int& t, size_t depth) { serializePrimitive(t, depth); }

	template <>
	void TextSerializer::inspect<float>(float& t, size_t depth) { serializePrimitive(t, depth); }

	std::string TextSerializer::_result;
}