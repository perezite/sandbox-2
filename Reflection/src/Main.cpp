#include "TextSerializer.h"
#define SB_SERIALIZER sb::TextSerializer
#include "Reflectable.h"
#include "Logger.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>

void version() {
	#ifdef _DEBUG
		std::string configuration = "Debug";
	#else
		std::string configuration = "Release";
	#endif	
	SB_MESSAGE("Audio - Build: " << configuration << ", " << __DATE__ << ", " << __TIME__);
}

class BaseStaticCallback0 {
public:
	virtual void call() = 0;
};

class StaticCallback : public BaseStaticCallback0 {
	std::string(*_functionPointer)();
public:
	StaticCallback(std::string(*functionPointer)())
		: _functionPointer(functionPointer)
	{ }
	virtual void call() {
		std::cout << _functionPointer() << std::endl;
	}
};

class MyClass0 {
public:
	static std::string someCallback() {
		return "MyClass0::someCallback";
	}
};

class MyOtherClass0 {
public:
	static std::string someOtherCallback() {
		return "MyOtherClass0::someOtherCallback";
	}
};

void demo0() {
	StaticCallback myStaticCallback(&MyClass0::someCallback);
	StaticCallback myOtherStaticCallback(&MyOtherClass0::someOtherCallback);
	std::vector<BaseStaticCallback0*> staticCallbacks;
	staticCallbacks.push_back(&myStaticCallback);
	staticCallbacks.push_back(&myOtherStaticCallback);
	for (size_t i = 0; i < staticCallbacks.size(); i++)
		staticCallbacks[i]->call();
}

// https://stackoverflow.com/questions/37031844/logic-of-stdis-base-of-in-c-11
template<typename D, typename B>
class IsDerivedFrom5
{
	class No { };
	class Yes { No no[2]; };

	static Yes Test(B*) {};
	static No Test(...) {};
public:
	static bool value() {
		return sizeof(Test(static_cast<D*>(0))) == sizeof(Yes);
	}
};

template <class B, class D>
bool isDerivedFrom(D& d) {
	return IsDerivedFrom5<D, B>::value();
}

class Base5 {
public:
	virtual void test() {
		std::cout << "Base::test()" << std::endl;
	}
};

class Derived5 : public Base5 {
public:
	virtual void test() {
		std::cout << "Derived5::test()" << std::endl;
	}
};

template <class T>
void call(T& t) {
	std::cout << "call<T>()" << std::endl;
	t.test();
}

template <>
void call<Base5>(Base5& b) {
	std::cout << "call<Base>()" << std::endl;
	b.test();
}

class Something5 { };

void demo5() {
	std::cout << IsDerivedFrom5<Derived5, Base5>::value();
	std::cout << IsDerivedFrom5<Something5, Base5>::value();

	Derived5 derived;
	Something5 some;
	std::cout << isDerivedFrom<Base5>(derived);
	std::cout << isDerivedFrom<Base5>(some);
}

void error10(const std::string& message) {
	std::cout << message << std::endl;
	std::cin.get();
	exit(0);
}

#define DM_SERIALIZER TextSerializer10

template <class T>
std::string serialize10(T& t, const std::string& typeName, size_t depth = 0);

class BaseProperty10 {
public:
	virtual ~BaseProperty10() { }
	virtual std::string serialize(size_t depth = 0) = 0;
};

class BaseReflectable10 {
public:
	std::string serialize(size_t depth = 0) {
		return ::serialize10(*this, getTypeName(), depth);
	}
	virtual const std::string getTypeName() = 0;
	virtual	const std::vector<BaseProperty10*>& getProperties() = 0;
};

class TextSerializer10 {
protected:
	static std::string serializeReflectable(BaseReflectable10& baseReflectable, size_t depth = 0) {
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
		if (IsDerivedFrom5<T, BaseReflectable10>::value()) {
			return serializeReflectable((BaseReflectable10&)t, depth);
		}
			
		error10("serialization not specified for type '" + typeName + "'");
		return "";
	}
	template <class T>
	static std::string serialize(std::vector<T>& v, const std::string& typeName, size_t depth = 0) {
		std::ostringstream os; os << std::string(depth, ' ') << typeName << std::endl;
		for (auto it = v.begin(); it != v.end(); it++) 
			os << serialize10(*it, stripTemplateTypename(typeName), depth + 1);
		return os.str();
	}
};

template <>
std::string TextSerializer10::serialize<int>(int& t, const std::string& typeName, size_t depth) {
	return serializePrimitive(t, typeName, depth);
}

template <>
std::string TextSerializer10::serialize<float>(float& t, const std::string& typeName, size_t depth) {
	return serializePrimitive(t, typeName, depth);
}

template <>
std::string TextSerializer10::serialize<std::string>(std::string& t, const std::string& typeName, size_t depth) {
	return serializePrimitive(t, typeName, depth);
}

template <class T>
std::string serialize10(T& t, const std::string& typeName, size_t depth) {
	return DM_SERIALIZER::serialize(t, typeName, depth);
}

template <class T>
class Property10 : public BaseProperty10 {
	T _reference;
	std::string _typeName;
public:
	Property10(T& reference, const std::string& typeName)
		: _reference(reference), _typeName(typeName)
	{ }
	inline std::string serialize(size_t depth = 0) {
		return ::serialize10(_reference, _typeName, depth);
	}
};

template <class T>
class Reflectable10 : public BaseReflectable10 {
	typedef void(T::*PropertyCreator)();
	static std::vector<PropertyCreator> PropertyCreators;
	static std::vector<std::string> PropertyCreatorNames;
	static bool AllCreatorsAdded;
	std::vector<BaseProperty10*> _properties;
	bool _propertiesCreated;
protected:
	void createProperties() {
		T* instance = (T*)this;
		for (size_t i = 0; i < PropertyCreators.size(); i++)
			(instance->*PropertyCreators[i])();
		_propertiesCreated = true;
	}
	static inline bool creatorWasAlreadyAdded(const std::string propertyName) {
		return std::find(PropertyCreatorNames.begin(), 
			PropertyCreatorNames.end(), propertyName) != PropertyCreatorNames.end();
	}
	template <class U>
	void createProperty(const std::string& name, U& reference, const std::string& typeName) {
		_properties.push_back(new Property10<U>(reference, typeName));
	}
	static void addPropertyCreator(PropertyCreator creator, const std::string& propertyName) {
		if (AllCreatorsAdded)
			return;
		if (creatorWasAlreadyAdded(propertyName)) {
			AllCreatorsAdded = true;
			return;
		}

		PropertyCreators.push_back(creator);
		PropertyCreatorNames.push_back(propertyName);
	}
public:
	Reflectable10() : _propertiesCreated(false)
	{ }
	virtual ~Reflectable10() {
		for (size_t i = 0; i < _properties.size(); i++)
			delete _properties[i];
	}
	virtual const std::vector<BaseProperty10*>& getProperties() { 
		if (!_propertiesCreated)
			createProperties();
		return _properties;
	};
};

template <typename T>
std::vector<void(T::*)()> Reflectable10<T>::PropertyCreators;
template <typename T>
std::vector<std::string> Reflectable10<T>::PropertyCreatorNames;
template <typename T>
bool Reflectable10<T>::AllCreatorsAdded = false;

template <void(*Func)()>
class Invocation10 {
public:
	Invocation10() {
		Func();
	}
};

#define DM_CLASS(className) \
	typedef className CurrentClass; \
	inline virtual const std::string getTypeName() { return #className; }

#ifndef __INTELLISENSE__
	#define DM_PROPERTY(type, name)													\
		type name;																	\
		void create_property_##name() {												\
			createProperty<type>(#name, name, #type);								\
		}																			\
		static void add_property_creator_##name() {									\
			addPropertyCreator(&CurrentClass::create_property_##name, #name);		\
		}																			\
		Invocation10<add_property_creator_##name> invocation_##name;						
#else
	#define DM_PROPERTY(type, name) type name;
#endif

class MyReflectable10 : public Reflectable10<MyReflectable10> {
public:
	DM_CLASS(MyReflectable10)
	DM_PROPERTY(int, myInt)
	DM_PROPERTY(float, myFloat)
};

void demo10() {
	MyReflectable10 myReflectable;
	myReflectable.myInt = 42;
	myReflectable.myFloat = 3.1415f;
	std::cout << myReflectable.getProperties()[0]->serialize() << std::endl;
	std::cout << myReflectable.getProperties()[1]->serialize() << std::endl;
}

class Position20 : public Reflectable10<Position20> {
public:
	DM_CLASS(Position20)
	DM_PROPERTY(std::string, myString)
	DM_PROPERTY(float, myFloat)
};

class MyReflectable20 : public Reflectable10<MyReflectable20> {
public:
	DM_CLASS(MyReflectable20)
	DM_PROPERTY(int, myInt)
	DM_PROPERTY(float, myFloat)
	DM_PROPERTY(Position20, myPosition)
};

void demo20() {
	MyReflectable20 myReflectable;
	myReflectable.myInt = 42;
	myReflectable.myFloat = 3.1415f;
	myReflectable.myPosition.myString = "position string";
	myReflectable.myPosition.myFloat = 2.72f;
	auto result = myReflectable.serialize();
	std::cout << result;
}

class MyLocation30 : public Reflectable10<MyLocation30> {
public:
	DM_CLASS(MyLocation30)
	DM_PROPERTY(float, myFloat)
	DM_PROPERTY(std::string, myString)
};

class MyReflectable30 : public Reflectable10<MyReflectable30> {
public:
	DM_CLASS(MyReflectable30)
	DM_PROPERTY(std::vector<int>, myInts)
	DM_PROPERTY(std::vector<MyLocation30>, myLocations)
};

void demo30() {
	MyReflectable30 myReflectable;
	myReflectable.myInts.push_back(1);
	myReflectable.myInts.push_back(2);
	myReflectable.myLocations.emplace_back();
	myReflectable.myLocations.emplace_back();
	myReflectable.myLocations[0].myFloat = 1.1415f;
	myReflectable.myLocations[0].myString = "first string";
	myReflectable.myLocations[1].myFloat = 2.1415f;
	myReflectable.myLocations[1].myString = "second string";

	auto result = myReflectable.serialize();
	std::cout << result;
}

class MyReflectable40 : public sb::Reflectable<MyReflectable40> {
public:
	SB_CLASS(MyReflectable40)
	SB_PROPERTY(int, myInt)
	SB_PROPERTY(float, myFloat)
};

void demo40() {
	MyReflectable40 myReflectable;
	myReflectable.myInt = 42;
	myReflectable.myFloat = 3.1415f;

	auto result = myReflectable.serialize();
	std::cout << result;
}

class MyLocation45 : public sb::Reflectable<MyLocation45> {
public:
	SB_CLASS(MyLocation45)
	SB_PROPERTY(float, myFloat)
	SB_PROPERTY(std::string, myString)
};

class MyReflectable45 : public sb::Reflectable<MyReflectable45> {
public:
	SB_CLASS(MyReflectable45)
	SB_PROPERTY(std::vector<int>, myInts)
	SB_PROPERTY(std::vector<MyLocation45>, myLocations)
};

void demo45() {
	MyReflectable45 myReflectable;
	myReflectable.myInts.push_back(1);
	myReflectable.myInts.push_back(2);
	myReflectable.myLocations.emplace_back();
	myReflectable.myLocations.emplace_back();
	myReflectable.myLocations[0].myFloat = 1.1415f;
	myReflectable.myLocations[0].myString = "first string";
	myReflectable.myLocations[1].myFloat = 2.1415f;
	myReflectable.myLocations[1].myString = "second string";

	auto result = myReflectable.serialize();
	std::cout << result;
}

void demo50() {
	int myInt = 45;
	std::cout << sb::SimpleSerializer::toString(myInt, "myInt", "int", 0);
}

int main() {
	version();

	demo50();
	//demo45();
	//demo40();
	//demo30();
	//demo20();
	//demo10();
	//demo5();
	//demo0();

	std::cin.get();
	return 0;
}

// header and source files
// split into lib and app
// serializer setting if error or not on non serializable types
// deserialize
// inspector
// save / load demo
// console editor demo
// item to incorporate into renderer