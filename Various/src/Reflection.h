#pragma once

#include "TextSerializer.h"
#include "InheritanceCheck.h"
#include "Logger.h"
#include "BaseReflectable.h"
#include "Macro.h"

#define SB_SET_INSPECTOR(inspector) do { sb::reflection::getInspectorTypename() = SB_NAMEOF(inspector); } while (false)

namespace sb {
	namespace reflection {
		static inline std::string& getInspectorTypename() {
			static std::string inspectorTypeName;
			return inspectorTypeName;
		}


		//#ifndef PENIS
		//#define PENIS
		//	static std::string inspectorTypeName;
		//#endif
	}
}

namespace sb {
	class SimpleSerializer {
		static std::string _result;
	protected:
		static inline void inspectReflectable(BaseReflectable& reflectable, size_t depth) {
			auto properties = reflectable.getProperties();
			for (size_t i = 0; i < properties.size(); i++)
				properties[i]->inspect(depth + 1);
		}
		template <class T>
		static inline void serializePrimitive(T& t, size_t depth) {
			std::ostringstream os; os << std::string(depth, ' ') << "*" << t << std::endl;
			_result += os.str();
		}
	public:
		template <class T>
		static inline void inspect(T& t, size_t depth = 0) {
			if (sb::InheritanceCheck<T, sb::BaseReflectable>::value()) {
				inspectReflectable((BaseReflectable&)t, depth);
				return;
			}

			SB_ERROR("Type not serializable");
		}
		template <class T>
		static inline std::string serialize(T& t, size_t depth = 0) {
			//SB_SET_INSPECTOR(sb::SimpleSerializer);
			_result = std::string();
			inspect<T>(t, depth);
			return _result;
		}
	};

	template <>
	inline void SimpleSerializer::inspect<int>(int& t, size_t depth) { serializePrimitive(t, depth); }

	template <>
	inline void SimpleSerializer::inspect<float>(float& t, size_t depth) { serializePrimitive(t, depth); }

}

namespace sb {
	namespace reflection {
		template <class T>
		inline void inspect(T& t, size_t depth) {
			if (getInspectorTypename() == SB_NAMEOF(sb::SimpleSerializer))
				sb::SimpleSerializer::inspect(t, depth);
			else if (getInspectorTypename() == SB_NAMEOF(sb::TextSerializer))
				sb::TextSerializer::inspect(t, depth);
			else
				SB_ERROR("Inspector '" << getInspectorTypename() << "'not supported. Make sure you specified the full inspector namespace."
					<< " Also make sure the inspector is called in serialize::inspect()");

			std::ostringstream os;
		}

		inline static void setInspector(const std::string& inspectorTypeName_) {
			getInspectorTypename() = inspectorTypeName_;
		}
	}
}
