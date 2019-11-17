#pragma once
#include "Reflection.h"
#include "BaseReflectable.h"
#include "Property.h"
#include <vector>

namespace sb {
	template <class T>
	class Reflectable : public BaseReflectable {
		typedef void(T::*PropertyCreator)();
		static std::vector<PropertyCreator> PropertyCreators;
		static std::vector<std::string> PropertyCreatorNames;
		static bool AllCreatorsAdded;
		std::vector<BaseProperty*> _properties;
		bool _propertiesCreated;
	protected:
		static inline bool creatorWasAlreadyAdded(const std::string propertyName) {
			return std::find(PropertyCreatorNames.begin(),
				PropertyCreatorNames.end(), propertyName) != PropertyCreatorNames.end();
		}
		static inline void addPropertyCreator(PropertyCreator creator, const std::string& propertyName) {
			if (AllCreatorsAdded)
				return;
			if (creatorWasAlreadyAdded(propertyName)) {
				AllCreatorsAdded = true;
				return;
			}

			PropertyCreators.push_back(creator);
			PropertyCreatorNames.push_back(propertyName);
		}
		inline void createProperties() {
			T* instance = (T*)this;
			for (size_t i = 0; i < PropertyCreators.size(); i++)
				(instance->*PropertyCreators[i])();
			_propertiesCreated = true;
		}
		template <class U>
		inline void createProperty(const std::string& name, U& reference, const std::string& typeName) {
			_properties.push_back(new Property<U>(reference, typeName));
		}

	public:
		inline Reflectable() : _propertiesCreated(false)
		{ }
		inline virtual ~Reflectable() {
			for (size_t i = 0; i < _properties.size(); i++)
				delete _properties[i];
		}
		inline virtual const std::vector<BaseProperty*>& getProperties() {
			if (!_propertiesCreated)
				createProperties();
			return _properties;
		};
	};

	template <class T>
	std::vector<void(T::*)()> Reflectable<T>::PropertyCreators;
	template <class T>
	std::vector<std::string> Reflectable<T>::PropertyCreatorNames;
	template <class T>
	bool Reflectable<T>::AllCreatorsAdded = false;
}

#define SB_CLASS(className) \
	typedef className CurrentClass; \
	inline virtual const std::string getTypeName() { return #className; }

#ifndef __INTELLISENSE__
	#define SB_PROPERTY(type, name)													\
		type name;																	\
		void create_property_##name() {												\
			createProperty<type>(#name, name, #type);								\
		}																			\
		static void add_property_creator_##name() {									\
			addPropertyCreator(&CurrentClass::create_property_##name, #name);		\
		}																			\
		Invocation10<add_property_creator_##name> invocation_##name;						
#else
	#define SB_PROPERTY(type, name) type name;
#endif