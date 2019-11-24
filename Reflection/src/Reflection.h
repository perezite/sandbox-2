#pragma once
#include "TextSerializer.h"
#include "InheritanceCheck.h"
#include "Logger.h"
#include "BaseReflectable.h"
#include "Macro.h"

#define SB_SET_INSPECTOR(inspector) do { sb::reflection::inspectorTypeName = SB_NAMEOF(inspector); } while (false)

namespace sb {
	namespace reflection {
		std::string inspectorTypeName = SB_NAMEOF(sb::TextSerializer);
	}
}

namespace sb {
	class SimpleSerializer {
		static std::string _result;
	protected:
		static void inspectReflectable(BaseReflectable& reflectable, size_t depth) {
			auto properties = reflectable.getProperties();
			for (size_t i = 0; i < properties.size(); i++)
				properties[i]->inspect(depth + 1);
		}
		template <class T>
		static void serializePrimitive(T& t, size_t depth) {
			std::ostringstream os; os << std::string(depth, ' ') << "*" << t << std::endl;
			_result += os.str();
		}
	public:
		template <class T>
		static void inspect(T& t, size_t depth = 0) {
			if (sb::InheritanceCheck<T, sb::BaseReflectable>::value()) {
				inspectReflectable((BaseReflectable&)t, depth);
				return;
			}

			SB_ERROR("Type not serializable");
		}
		template <class T>
		static std::string toString(T& t, size_t depth = 0) {
			SB_SET_INSPECTOR(sb::SimpleSerializer);
			_result = std::string();
			inspect<T>(t, depth);
			return _result;
		}
	};

	template <>
	void SimpleSerializer::inspect<int>(int& t, size_t depth) { serializePrimitive(t, depth); }

	template <>
	void SimpleSerializer::inspect<float>(float& t, size_t depth) { serializePrimitive(t, depth); }

	std::string SimpleSerializer::_result;
}

namespace sb {
	namespace reflection {

		template <class T>
		inline std::string serialize(T& t, const std::string& typeName, size_t depth) {
			// return SB_SERIALIZER::serialize(t, typeName, depth);
			return std::string();
		}

		template <class T>
		inline void inspect(T& t, size_t depth) {
			if (inspectorTypeName == SB_NAMEOF(sb::SimpleSerializer))
				sb::SimpleSerializer::inspect(t, depth);
			else if (inspectorTypeName == SB_NAMEOF(sb::TextSerializer))
				sb::TextSerializer::inspect(t, depth);
			else
				SB_ERROR("Inspector not supported");
		}

		inline static void setInspector(const std::string& inspectorTypeName_) {
			inspectorTypeName = inspectorTypeName_;
		}
	}
}
