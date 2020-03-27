#pragma once
#include "BaseReflectable.h"
#include "InheritanceCheck.h"
#include "Logger.h"
#include "Macro.h"
#include <string>
#include <sstream>

namespace sb {
	namespace reflection {
		static inline void setInspector(const std::string& inspectorTypeName_);
	}
}

namespace sb {
	class TextSerializer {
		static std::string _result;

	protected:
		static inline void serializeReflectable(BaseReflectable& reflectable, size_t depth) {
			auto properties = reflectable.getProperties();
			for (size_t i = 0; i < properties.size(); i++)
				properties[i]->inspect(depth + 1);
		}

		template <class T>
		static inline void serializePrimitive(T& t, size_t depth) {
			std::ostringstream os; os << std::string(depth, ' ') << t << std::endl;
			_result += os.str();
		}

	public:
		template <class T>
		static inline void inspect(T& t, size_t depth = 0) {
			if (sb::InheritanceCheck<T, sb::BaseReflectable>::value()) {
				serializeReflectable((BaseReflectable&)t, depth);
				return;
			}

			SB_ERROR("The current inspector does not support this type");
		}

		template <class T>
		static inline std::string serialize(T& t, size_t depth = 0) {
			sb::reflection::setInspector(SB_NAMEOF(sb::TextSerializer));
			_result = std::string();
			inspect<T>(t, depth);
			return _result;
		}
	};

	template <>
	inline void TextSerializer::inspect<int>(int& t, size_t depth) { serializePrimitive(t, depth); }

	template <>
	inline void TextSerializer::inspect<float>(float& t, size_t depth) { serializePrimitive(t, depth); }
}