#pragma once
#include "BaseReflectable.h"
#include "InheritanceCheck.h"
#include "Logger.h"
#include <string>
#include <sstream>

namespace sb {
	class TextSerializer {
	protected:
		static std::string serializeReflectable(BaseReflectable& baseReflectable, size_t depth = 0) {
			std::ostringstream os; os << std::string(depth, ' ') << baseReflectable.getTypeName() << std::endl;
			auto& properties = baseReflectable.getProperties();
			for (size_t i = 0; i < properties.size(); i++)
				os << properties[i]->serialize(depth + 1);
			return os.str();
		}
		template <class T>
		static std::string serializePrimitive(T& t, const std::string typeName, size_t depth) {
			std::ostringstream os; os << std::string(depth, ' ') << typeName << std::endl
				<< std::string(depth + 1, ' ') << t << std::endl; return os.str();
		}
		static std::string stripTemplateTypename(const std::string& typeName) {
			auto left = typeName.find('<');
			auto right = typeName.rfind('>');
			return typeName.substr(left + 1, right - left - 1);
		}
	public:
		template <class T>
		static std::string serialize(T& t, const std::string& typeName, size_t depth = 0) {
			if (InheritanceCheck<T, BaseReflectable>::value()) {
				return serializeReflectable((BaseReflectable&)t, depth);
			}

			SB_ERROR("serialization not specified for type '" << typeName << "'");
			return "";
		}
		template <class T>
		static std::string serialize(std::vector<T>& v, const std::string& typeName, size_t depth = 0) {
			std::ostringstream os; os << std::string(depth, ' ') << typeName << std::endl;
			for (auto it = v.begin(); it != v.end(); it++)
				os << serialize(*it, stripTemplateTypename(typeName), depth + 1);
			return os.str();
		}
	};

	template <>
	std::string TextSerializer::serialize<int>(int& t, const std::string& typeName, size_t depth) {
		return serializePrimitive(t, typeName, depth);
	}

	template <>
	std::string TextSerializer::serialize<float>(float& t, const std::string& typeName, size_t depth) {
		return serializePrimitive(t, typeName, depth);
	}

	template <>
	std::string TextSerializer::serialize<std::string>(std::string& t, const std::string& typeName, size_t depth) {
		return serializePrimitive(t, typeName, depth);
	}
}