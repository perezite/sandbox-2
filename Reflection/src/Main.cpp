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

/*
struct Vector2f : Reflectable {
	float x, y;

	REFLECT_CLASS(Vector2f)
};

class Hero : Reflectable {
	SB_PROPERTY (Vector2f, position)
	SB_PROPERTY (int, health)
	SB_PROPERTY (std::string name)

	REFLECT_TYPE(Hero);
};*/

/*
template <class T>
inline std::string serializePrimitive(T& t, const std::string& name, size_t depth = 0) {
	std::ostringstream os;
	tabify(os, depth);
	os << std::string(depth, ' ') << name << " " << t << std::endl:
	return os.str();
}

template <class R>
std::string serialize(R& r, const std::string name, size_t depth = 0) {
	std::cout << std::string(depth, ' ') << name << std::endl;
	auto properties = r.getProperties();
	for (size_t i = 0; i < r.getProperties(); i++)
		serialize(properties[i]);
}

template <>
std::string serialize<float>(float& t, const st::string& name, size_t depth = 0) { return serializePrimitive(t, name); }
template <>
std::string serialize<int>(int& t, const std::string& name, size_t depth = 0) { return serializePrimitive(t, name); }

template <class T>
void serialize() {
}

void serialize(Hero& hero) {
	SB_ERROR("Implement!");
}

template <class T>
std::string getName() {
	SB_ERROR("The type name is not specified");
}

DECLARE_TYPENAME(Vector2f)
//template <>
//std::string getName<Vector2f>() {
//	return "float";
//}

template <class T>
void edit(T& t, const std::string typeName) {
	SB_ERROR("edit is not implemented for type " + typeName);		
}

template <class Vector2f>
void edit<Vector2f>(Vector2f& v, const std::string& typeName) {
	std::cout << "x: ";
	std::cin >> v.x;
	std::cout << std::endl << "y:";
	std::cin >> v.y;
	std::cout << std::endl;
}

template <class T>
void editAll(std::vector<Property>& properties) {
	for (size_t i = 0; i < properties.size(); i++) {
		if (properties[i].getType() == getType<T>()) 
			edit<T>(properties[i], getType<T>);

		editAll<T>(properties[i].getChildren());
	}
}

template <class T> 
void editAll(T& t) {
	editAll(t.getProperties());
}

*/

void demo1000() {
/*
	Hero hero;
	hero.name = "The Hero";
	hero.health = 42;
	hero.position.x = 1;
	hero.position.y = 2;

	std::string heroString = serialize(hero);

	Hero returnedHero;
	deserialize(returnedHero, heroString);

	editAll<Vector2f>(returnedHero);
*/
}

void error(const std::string& message) {
	std::cout << message << std::endl;
	std::cin.get();
	exit(0);
}

template <void(*Func)()>
class Caller {
public:
	Caller() {
		Func();
	}
};

template <class T>
std::string serialize(T& t) {
	error("Serialization for this type is not specified");
	return "";
}

template <>
std::string serialize<int>(int& f) {
	std::ostringstream os;
	os << f;
	return os.str();
}

template <class T>
class Property {
	typedef std::string(T::*SerializeMethod)();
	const std::string _name;
	const std::string _typename;

public:
	Property(const std::string& name, const std::string& typeName, SerializeMethod serializer)
		:  _name(name)
	{ }

	virtual ~Property() { }

	virtual std::string serialize(size_t) {
		return std::string();
		//return ::serialize<T>(_value);
	}
};

template <class T>
class Reflectable {
	typedef std::string (T::*SerializeMethod)();
	//typedef void(T::*EditMethod)(EditorCreator&);

	static std::map<std::string, SerializeMethod> _serializerMethods;
	//static std::map<std::string, EditMethod> _editMethods;

public:
	//Editor* editor

	Reflectable()
	{

	}

	virtual ~Reflectable() {

	}

	inline static void addProperty(const std::string& name, SerializeMethod serializeMethod/*, EditMethod editMethod*/) {
		_serializerMethods[name] = serializeMethod;
		//_editMethods[name] = editMethod;
	}

	std::string serializeXPropety() {
		auto serialize = _serializerMethods["x"];
		T* instance = (T*)this;
		auto result = (instance->*serialize)();
		return result;
	}
};

//template <class T>
//class Editor {
//public:
//	static void edit(T& t) { std::cout << "Editor"; }
//};
//
//template <class T>
//class ConsoleEditor : Editor<T> {
//public:
//	static void edit(T& t) { std::cout << "Console editor"; }
//};

//
//template <class T> 
//class BaseEditorInstanceProvider {
//	virtual Editor<T>& getInstance() = 0;
//};

//template <class T, class U>
//class EditorInstanceProvider {
//	static Editor<T>& getInstance() {
//
//		static U<T> instance = U<T>();
//		return instance;
//	}
//};

//template <class T>
//class EditorProvider {
//public:
//	//static Editor<T>* editor;
//	//static EditorInstanceProvider<T> provider;
//
//	static Editor<T>& getEditor() { return *(new Editor<T>()); }
//};
//
//class EditorImpl {
//public:
//	
//	template <class T>
//	edit(T &t) 
//};

//class Editor {
//public:
//	virtual EditorImpl getImpl() = 0;
//};
//
//class ConsoleEditor : public Editor {
//	EditorImpl _impl;
//public:
//	virtual EditorImpl getImpl() { return _impl; }
//
//};

//
//template <class U, class U>
//class MyProvider {
//public:
//	T<U> getInstance() {
//
//	}
//};
//
//template <class T>
//Editor<T> getInstance() {
//
//}

template <typename T>
std::map<std::string, std::string (T::*)()> Reflectable<T>::_serializerMethods;
//template <typename T>
//std::map<std::string, void(T::*)(EditorCreator&)> Reflectable<T>::_editMethods;

template <class T>
class Editor {
public:
	virtual void edit(T &t) {};
};

template <class T>
class ConsoleEditor : public Editor<T> {
public:
	void edit(T& t) {
	}
};

template <class T>
class OtherEditor : public Editor<T> {
public:
	void edit(T& t) {
	}
};

class EditorFactory {
public:
	static size_t current;

	template <class T>
	static Editor<T>& getInstance() {
		if (current == 0) {
			static auto ed = ConsoleEditor<T>();
			return ed;
		}
		if (current == 1) {
			static auto ed = OtherEditor<T>();
			return ed;
		}

		error("bad index");
		static auto ed = OtherEditor<T>();
		return ed;
	}
};

size_t EditorFactory::current = 0;

template <class T>
void edit(T& t) {
	auto editor = EditorFactory::getInstance<T>();
	editor.edit(t);
}

class MyReflectable : public Reflectable<MyReflectable> {
	typedef MyReflectable __sb_CurrentClass;

	int x;
	int& __sb_get_x() {
		return x;
	}
	// todo: inject a serializer class
	std::string __sb_serialize_x() {
		return serialize<int>(x);
	}
	void __sb_edit_x(EditorFactory& factory) {
		edit<int>(x);
		//ConsoleEditor<int> bla;
		//auto editor = EditorProvider<int>::getEditor();
		//editor.edit(x);
		// return editorCaller.edit<int>(x);
	}
	static void __sb_register_x() {
		addProperty("x", &MyReflectable::__sb_serialize_x/*, &MyReflectable::__sb_edit_x*/);
	}
	Caller<__sb_register_x> __sb_caller_register_x;

public:
	MyReflectable() : x(42)
	{ }
};

/* 
	... 
	template <>
	void edit<float>(float & float) {
		...
	}

	void edit(Property& property) {
	if (property.getTypeName() == SB_NAMEOF(int)) 
		convertAndEdit<float>(property);
	else
		exit(0);
} */

void demo100() {

	MyReflectable reflectable;

	auto result = reflectable.serializeXPropety();
	std::cout << result << std::endl;

	/*
	MyReflectable reflectable;
	std::cout << reflectable.getTypeName() << std::endl;
	auto properties = reflectable.getProperties();
	for (size_t i = 0; i < properties.size(); i++)
		std::cout << properties[i]->getName() << " " << properties[i]->getTypeName() << " " << properties[i]->serialize() << std::endl;

	edit(properties[0]);

	properties[0]->deserialize("3.1415");
	

	*/
}

class BaseStaticCallback {
public:
	virtual void call() = 0;
};

class StaticCallback : public BaseStaticCallback {
	std::string(*_functionPointer)();
public:
	StaticCallback(std::string(*functionPointer)())
		: _functionPointer(functionPointer)
	{ }
	virtual void call() {
		std::cout << _functionPointer() << std::endl;
	}
};

class MyClass {
public:
	static std::string someCallback() {
		return "MyClass::someCallback";
	}
};

class MyOtherClass {
public:
	static std::string someOtherCallback() {
		return "MyOtherClass::someOtherCallback";
	}
};

void demo0() {
	StaticCallback myStaticCallback(&MyClass::someCallback);
	StaticCallback myOtherStaticCallback(&MyOtherClass::someOtherCallback);
	std::vector<BaseStaticCallback*> staticCallbacks;
	staticCallbacks.push_back(&myStaticCallback);
	staticCallbacks.push_back(&myOtherStaticCallback);
	for (size_t i = 0; i < staticCallbacks.size(); i++)
		staticCallbacks[i]->call();
}

// https://stackoverflow.com/questions/37031844/logic-of-stdis-base-of-in-c-11
template<typename D, typename B>
class IsDerivedFrom
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
	return IsDerivedFrom<D, B>::value();
}

#define SB_SERIALIZER TextSerializer

template <class T>
std::string serialize10(T& t, const std::string& typeName, size_t depth = 0);

class BaseProperty10 {
public:
	virtual ~BaseProperty10() { }
	virtual std::string serialize(size_t depth = 0) = 0;
};

class BaseReflectable {
public:
	std::string serialize(size_t depth = 0) {
		return ::serialize10(*this, getTypeName(), depth);
	}
	virtual const std::string getTypeName() = 0;
	virtual	const std::vector<BaseProperty10*>& getProperties() = 0;
};

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
		if (IsDerivedFrom<T, BaseReflectable>::value()) {
			return serializeReflectable((BaseReflectable&)t, depth);
		}
			
		error("serialization not specified for type '" + typeName + "'");
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

//template <>
//std::string TextSerializer::serialize<template <> std::vector<T>>(std::vector<T>& t, const std::string typeName, size_t depth) {
//	
//}

template <class T>
std::string serialize10(T& t, const std::string& typeName, size_t depth) {
	return SB_SERIALIZER::serialize(t, typeName, depth);
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
class Reflectable10 : public BaseReflectable {
	typedef void(T::*PropertyCreator)();
	static std::vector<PropertyCreator> _propertyCreators;
	static std::vector<std::string> _propertyCreatorNames;
	static bool _allCreatorsAdded;
	std::vector<BaseProperty10*> _properties;
	bool _propertiesCreated;
protected:
	void createProperties() {
		T* instance = (T*)this;
		for (size_t i = 0; i < _propertyCreators.size(); i++)
			(instance->*_propertyCreators[i])();
		_propertiesCreated = true;
	}
	static inline bool creatorWasAlreadyAdded(const std::string propertyName) {
		return std::find(_propertyCreatorNames.begin(), 
			_propertyCreatorNames.end(), propertyName) != _propertyCreatorNames.end();
	}
	template <class U>
	void createProperty(const std::string& name, U& reference, const std::string& typeName) {
		_properties.push_back(new Property10<U>(reference, typeName));
	}
	static void addPropertyCreator(PropertyCreator creator, const std::string& propertyName) {
		if (_allCreatorsAdded)
			return;
		if (creatorWasAlreadyAdded(propertyName)) {
			_allCreatorsAdded = true;
			return;
		}

		_propertyCreators.push_back(creator);
		_propertyCreatorNames.push_back(propertyName);
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
std::vector<void(T::*)()> Reflectable10<T>::_propertyCreators;
template <typename T>
std::vector<std::string> Reflectable10<T>::_propertyCreatorNames;
template <typename T>
bool Reflectable10<T>::_allCreatorsAdded = false;

template <void(*Func)()>
class Caller10 {
public:
	Caller10() {
		Func();
	}
};

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
		Caller10<add_property_creator_##name> caller_##name;						
#else
	#define SB_PROPERTY(type, name) type name;
#endif

class MyReflectable10 : public Reflectable10<MyReflectable10> {
public:
	SB_CLASS(MyReflectable10)
	SB_PROPERTY(int, myInt)
	SB_PROPERTY(float, myFloat)
};

void demo10() {
	MyReflectable10 myReflectable;
	myReflectable.myInt = 42;
	myReflectable.myFloat = 3.1415f;
	std::cout << myReflectable.getProperties()[0]->serialize() << std::endl;
	std::cout << myReflectable.getProperties()[1]->serialize() << std::endl;
}

class Base {
public:
	virtual void test() {
		std::cout << "Base::test()" << std::endl;
	}
};

class Derived : public Base {
public:
	virtual void test() {
		std::cout << "Derived::test()" << std::endl;
	}
};

template <class T>
void call(T& t) {
	std::cout << "call<T>()" << std::endl;
	t.test();
}

template <>
void call<Base>(Base& b) {
	std::cout << "call<Base>()" << std::endl;
	b.test();
}

class Something { };

void demo15() {
	std::cout << IsDerivedFrom<Derived, Base>::value();
	std::cout << IsDerivedFrom<Something, Base>::value();

	Derived derived;
	Something some;
	std::cout << isDerivedFrom<Base>(derived);
	std::cout << isDerivedFrom<Base>(some);
}

class Position16 : public Reflectable10<Position16> {
public:
	SB_CLASS(Position16)
	SB_PROPERTY(std::string, myString)
	SB_PROPERTY(float, myFloat)
};

class MyReflectable16 : public Reflectable10<MyReflectable16> {
public:
	SB_CLASS(MyReflectable16)
	SB_PROPERTY(int, myInt)
	SB_PROPERTY(float, myFloat)
	SB_PROPERTY(Position16, myPosition)
};

void demo16() {
	MyReflectable16 myReflectable;
	myReflectable.myInt = 42;
	myReflectable.myFloat = 3.1415f;
	myReflectable.myPosition.myString = "position string";
	myReflectable.myPosition.myFloat = 2.72f;
	auto result = myReflectable.serialize();
	std::cout << result;
}

class MyLocation17 : public Reflectable10<MyLocation17> {
public:
	SB_CLASS(MyLocation17)
	SB_PROPERTY(float, myFloat)
	SB_PROPERTY(std::string, myString)
};

class MyReflectable17 : public Reflectable10<MyReflectable17> {
public:
	SB_CLASS(MyReflectable17)
	SB_PROPERTY(std::vector<int>, myInts)
	SB_PROPERTY(std::vector<MyLocation17>, myLocations)
};

void demo17() {
	MyReflectable17 myReflectable;
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

int main() {
	version();

	demo17();

	std::cin.get();
	return 0;
}

// cleanup
// hide internal reflection stuff from intellisense
// header and source files
// split into lib and app
// serializer setting if error or not on non serializable types
// deserialize
// inspector
// save / load demo
// console editor demo
// item to incorporate into renderer