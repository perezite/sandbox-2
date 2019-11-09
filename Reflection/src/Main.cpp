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

template <void(*Func)()>
class Caller {
public:
	Caller() {
		Func();
	}
};

template <class T>
std::string serialize(T& t) {
	std::cout << "Serialization for this type is not specified" << std::endl;
	std::cin.get();
	exit(0);
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

class ConsoleEditor {
public:
	template <class U>
	static void edit(U& t) {

	}
};

class EditorCaller {
public:	
	ConsoleEditor editor;
	
	template <class T>
	void edit(T& t) {
		editor.edit<T>(t);
		//editor.edit(t);
	}

	/*template <class T>
	void edit(T& t) { editor.edit(u); }*/
};

template <class T>
class Reflectable {
	typedef std::string (T::*SerializeMethod)();
	typedef void(T::*EditMethod)(EditorCaller&);

	static std::map<std::string, SerializeMethod> _serializersMethods;
	static std::map<std::string, EditMethod> _editMethods;

public:
	inline static void addProperty(const std::string& name, SerializeMethod serializeMethod, EditMethod editMethod) {
		_serializersMethods[name] = serializeMethod;
		_editMethods[name] = editMethod;
	}

	std::string serializeXPropety() {
		auto serialize = _serializersMethods["x"];
		T* instance = (T*)this;
		auto result = (instance->*serialize)();
		return result;
	}
};

template <typename T>
std::map<std::string, std::string (T::*)()> Reflectable<T>::_serializersMethods;
template <typename T>
std::map<std::string, void(T::*)(EditorCaller&)> Reflectable<T>::_editMethods;

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
	void __sb_edit_x(EditorCaller& editor) {
		// return editor.edit<int>(x);
	}
	static void __sb_register_x() {
		addProperty("x", &MyReflectable::__sb_serialize_x, &MyReflectable::__sb_edit_x);
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

void demo0 () {
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

int main() {
	version();

	demo0();
	//demo1000();

	std::cin.get();
	return 0;
}
