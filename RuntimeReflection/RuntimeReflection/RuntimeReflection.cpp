#include <iostream>

using namespace std;

class Vector2f {
public: 
    float x, y;

    Vector2f(float x_ = 0, float y_ = 0)
        : x(x_), y(y_)
    { }
};

class Class100 {
public:
    Vector2f position;

    int health;

public:
    Class100() 
        : position(1, 2), health(100)
    { }
};

class Derived100 : public Class100
{
public:
    float test;
};

void indent100(int level) {
    const int spaces = level * 4;
    for (size_t i = 0; i < spaces; i++)
        cout << " ";
}   

/*
void printMetadata100(Metadata& md, int level = 0) {
    for (size_t i = 0; i < md.countOwnFields(); i++) { 
        Metadata field = md.getOwnFields()[i];
        if (field.isComposite())
            printMetadata(field, level + 1);
        else if (field.isSimple())
            cout << indent100(level) << field.getName() 
                 << ": " << field.toString() << endl;
    }
}
*/

void demo100() {
    // Reflection reflection;
    // reflection
    //     .beginClass<Vector2f>("Vector2f")
    //         .addField("x", x)
    //         .addField("y", y)
    //     .endClass()
    //     .beginClass<Class100>("Class100")
    //         .addField(position)
    //         .addField(health)
    //     .endClass()
    //     .deriveClass<Derived100, Class100>("Derived100")
    //         .addField("test", test)
    //     .endClass();

    Derived100 derived;
    // Metadata md = reflection.getMetadata(derived); 
    // cout << md.getName();
    //
    // printMetadata100(md); 
    // 
    // for (size_t i = 0; i < md.countFields(); i++) {
    //     if (md.getFields()[i].isOfType<int>())
    //         cout << md.getFields()[i].getName() << " os of type int" << endl;
    // }
}

int main() {
    cout << "Hello World!\n";
    cin.get();
}
