#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

template <class T>
void deleteAll(T& t)
{
    for (typename T::iterator it = t.begin(); it != t.end(); it++)
        delete (*it);
}

class Vector2f100 {
public: 
    float x, y;

    Vector2f100(float x_ = 0, float y_ = 0)
        : x(x_), y(y_)
    { }
};

class Class100 {
public:
    Vector2f100 position;

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

void indent100(size_t level) {
    const size_t spaces = level * 4;
    for (size_t i = 0; i < spaces; i++)
        cout << " ";
}   

/*
void printMetadata100(Metadata& md, size_t level = 0) {
    for (size_t i = 0; i < md.countOwnFields(); i++) { 
        Metadata field = md.getOwnFields()[i];
        if (field.isComposite()) {
            cout << indent100(level) << field.getName()
                 << " (" << field.getTypename() << "):" << endl; 
            printMetadata(field, level + 1);
        else if (field.isSimple()) {
            cout << indent100(level) << field.getName() 
                 << " (" << field.getTypename() << "): "
                 << field.toString() << endl;
        }
    }
}
*/

void demo100() {
    // Reflection reflection;
    // reflection
    //     .beginClass<Vector2f100>("Vector2f100")
    //         .addField("x", x)
    //         .addField("y", y)
    //     .endClass()
    //     .beginClass<Class100>("Class100")
    //         .addField("position", position)
    //         .addField("health", health)
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


    // Output
    // Derived
    // 
}

class Metadata2
{
    string _name;

public:
    Metadata2(const string& name) : _name(name) { }

    template <class T> void addField(const string& name, T& field) {

    }
};

template <class T> class ClassMetadataBuilder2;

class Reflection2
{
    vector<Metadata2*> _metadataList;

public:
    virtual ~Reflection2() { deleteAll(_metadataList); }

    template <class T> ClassMetadataBuilder2<T> beginClass(const string& name) {
        return ClassMetadataBuilder2<T>(*this, name);
    }
};

template <class T>
class ClassMetadataBuilder2
{
    Reflection2& _parent;

    Metadata2 _metadata;

public:
    ClassMetadataBuilder2(Reflection2& parent, const string& name) : _parent(parent), _metadata(name) { }

    template <class T>
    ClassMetadataBuilder2& addField(const string& name, T& field) {
        Metadata2 metadata(name);

        //_metadata.addField(name, field);
    }
};


class MyClass2 {
public:
    int x;

    MyClass2() : x(42) { }
};

void demo2() {
    Reflection2 reflection;
    // reflection
    //     .beginClass<MyClass1>("MyClass1");
    //         .addField("x", x)    
    //     .endClass();

    MyClass2 myClass;

    // Metadata md = reflection.getMetadata(myClass);
    // cout << md.getName();
    // cout << md.getFields(0).getName() << " " 
    //      << md.getFields(0).getTypename() << md.getFields(0).toString();
}

static size_t FieldTypeId = 0;

template <class T>
class FieldType1 {
    static bool MustGenerateTypeId;

    static size_t TypeId;

public:
    FieldType1() {
        if (MustGenerateTypeId) {
            TypeId = ++FieldTypeId;
            MustGenerateTypeId = false;
        }
    }

    static size_t GetTypeId() { return TypeId; }
};

template <class T> size_t FieldType1<T>::TypeId;
template <class T> bool FieldType1<T>::MustGenerateTypeId = true;

class Metadata1 {
    string _name;
   
public:
    Metadata1(const string& name) : _name(name) { }

    inline const string& getName() const { return _name; }

    virtual size_t getTypeId() = 0;

    virtual const string toString() = 0;
};

template <class T> string toString(T& t) {
    ostringstream os;
    os << t;
    return os.str();
}

template <class T>
class Field1 : public Metadata1 {
    FieldType1<T> _type;

    T& _ref;

public:
    Field1(const string& name, T& ref)
        : Metadata1(name), _ref(ref)
    { }

    virtual size_t getTypeId() { return _type.GetTypeId(); }

    virtual const string toString() {
        return ::toString(_ref);
    }
};

class Reflection1
{
    vector<Metadata1*> _metadataList;

public:
    virtual ~Reflection1() {
        deleteAll(_metadataList);
    }

    template <class T>
    void addField(const string& name, T& field) {
        Metadata1* md = new Field1<T>(name, field);
        _metadataList.push_back(md);
    }

    inline vector<Metadata1*> getFields() const { return _metadataList; }

    inline Metadata1& getField(int index) { return *(getFields()[index]); }
};

void print(Metadata1& field)
{
    cout << field.getName()
        << " (typeId " << field.getTypeId() << ")"
        << ": " << field.toString()
        << endl;
}

void demo1()
{
     int x = 42;
     float f = 3.1415f;
     int x2 = 43;

     Reflection1 reflection;
     reflection.addField("x", x);
     reflection.addField("f", f);
     reflection.addField("x2", x2);
     print(reflection.getField(0));
     print(reflection.getField(1));
     print(reflection.getField(2));
}

void demo() {
    demo1();
    //demo2();
    //demo100();
}

int main() {
    demo();
    cin.get();
}
