#include "Logger.h"
#include <string>
#include <iostream>

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

	std::cout << heroString;
	std::cout << serialize(returnedHero);

	Hero otherHero;
	hero.name = "The other Hero";
	hero.health = 200;
	hero.position.x = -1;
	hero.position.y = -2;
	std::string inEditorHero = serialize(otherHero);
	auto otherHeroProperties = getProperties(inEditorHero);
	editAll<Vector2f>(otherHeroProperties);

	Hero otherReturnedHero;
	deserialize(otherReturnedHero, otherHeroProperties.getString());

	std::cout << serialize(otherReturnedHero);
*/
}

int main() {
	version();

	demo1000();

	std::cin.get();
	return 0;
}
