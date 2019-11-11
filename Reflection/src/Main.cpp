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

	virtual std::string serialize() {
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

inline int generateTypeId() {
	static int typeId = -1;
	typeId++;
	return typeId;
}

template <class T>
class Serializer {
public:
	static int getStaticTypeId() {
		static int typeId = generateTypeId();
		return typeId;
	}

	const int getTypeId() const { return getStaticTypeId(); }
};

class TextSerializer : public Serializer<TextSerializer> {
public:
	template <class T>
	static std::string serialize(T& t) {
		error("serialization not specified for given type");
	}
};

template <>
std::string TextSerializer::serialize<int>(int& t) {
	std::ostringstream os; os << t; return os.str();
}

class BaseProperty10 {
	static int _serializerTypeId;
public:
	virtual ~BaseProperty10() { }
	template <class T>
	static void setSerializer() {
		_serializerTypeId = T::getStaticTypeId();
	}
	virtual std::string serialize() = 0;
};

int BaseProperty10::_serializerTypeId;

template <class T>
class Property10 : public BaseProperty10 {
	T _reference;
public:
	Property10(T& reference) : _reference(reference)
	{ }

	std::string serialize() {
		return TextSerializer::serialize<T>(_reference);
	}
};

template <class T>
class Reflectable10 {
	typedef void(T::*PropertyCreator)();
	static std::vector<PropertyCreator> _propertyCreators;
	std::vector<BaseProperty10*> _properties;
protected:
	void createProperties() {
		T* instance = (T*)this;
		for (size_t i = 0; i < _propertyCreators.size(); i++)
			(instance->*_propertyCreators[i])();
	}

public:
	virtual ~Reflectable10() {
		for (size_t i = 0; i < _properties.size(); i++)
			delete _properties[i];
	}
	const std::vector<BaseProperty10*>& getProperties() { 
		createProperties();
		return _properties;
	};
	template <class U>
	void createProperty(const std::string& name, U& reference) {
		_properties.push_back(new Property10<U>(reference));
	}
	static void addPropertyCreator(PropertyCreator creator) {
		_propertyCreators.push_back(creator);
	}
};

template <typename T>
std::vector<void(T::*)()> Reflectable10<T>::_propertyCreators;

template <void(*Func)()>
class Caller10 {
public:
	Caller10() {
		Func();
	}
};

class MyReflectable10 : public Reflectable10<MyReflectable10> {
public:
	int myInt;
	void create_property_x() {
		createProperty<int>("myInt", myInt);
	}
	static void register_x() {
		addPropertyCreator(&MyReflectable10::create_property_x);
	}
	Caller10<register_x> caller_x;
};

void demo10() {
	MyReflectable10 myReflectable;
	myReflectable.myInt = 42;
	auto test = myReflectable.getProperties();
	BaseProperty10::setSerializer<TextSerializer>();
	std::cout << myReflectable.getProperties()[0]->serialize() << std::endl;
}

int main() {
	version();

	demo10();
	//demo1000();

	std::cin.get();
	return 0;
}

