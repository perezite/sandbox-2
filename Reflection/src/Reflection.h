#pragma once

#ifndef SB_SERIALIZER
	#include "TextSerializer.h"
	#define SB_SERIALIZER sb::TextSerializer
#endif

#define SB_NAMEOF(entity) #entity

#define SB_SET_INSPECTOR(inspector) do { sb::reflection::inspectorTypeName = SB_NAMEOF(inspector); } while (false)

namespace sb {
	class SimpleSerializer {
		static std::string _result;

	public:
		template <class T>
		static std::string toString(T& t, const std::string& name, const std::string& typeName, size_t depth) {
			SB_SET_INSPECTOR(sb::SimpleSerializer);
			_result = std::string();
			inspect(t, name, typeName, depth);
			return _result;
		}

		template <class T>
		static void inspect(T& t, const std::string& name, const std::string& typeName, size_t depth) {
			std::ostringstream os;
			os << t << " " << name << " " << typeName << " " << depth << std::endl;
			_result += os.str();
		}
	};

	std::string SimpleSerializer::_result;
}


namespace sb {
	namespace reflection {
		std::string inspectorTypeName = SB_NAMEOF(sb::TextSerializer);

		template <class T>
		inline std::string serialize(T& t, const std::string& typeName, size_t depth) {
			return SB_SERIALIZER::serialize(t, typeName, depth);
		}

		template <class T>
		inline void inspect(T& t, const std::string& name, const std::string& typeName, size_t depth) {
			if (inspectorTypeName == SB_NAMEOF(sb::SimpleSerializer))
				sb::SimpleSerializer::inspect(t, name, typeName, depth);
			else if (inspectorTypeName == SB_NAMEOF(sb::TextSerializer))
				sb::TextSerializer::serialize(t, typeName, depth);
			else
				SB_ERROR("Inspector not supported");
		}
	}
}
