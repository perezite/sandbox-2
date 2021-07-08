#include <iostream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <map>
#include <complex>
#include <Windows.h>

extern "C"
{
#include "lua542/include/lua.h"
#include "lua542/include/lauxlib.h"
#include "lua542/include/lualib.h"
}

#include "LuaBridge/LuaBridge.h"

#ifdef _WIN32
#pragma comment(lib, "lua542/liblua54.a")
#endif

// https://stackoverflow.com/questions/5047971/how-do-i-check-for-c11-support
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define CPP_11
#endif

using namespace std;
using namespace luabridge;

template <class T>
void deleteAll(T& t)
{
    for (typename T::iterator it = t.begin(); it != t.end(); it++)
        delete (*it);
}

template <class T> string toString(T& t) {
    ostringstream os;
    os << t;
    return os.str();
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
    //         .addField("myInt", myInt)
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
    vector<Metadata2*> _fields;

public:
    virtual ~Reflection2() { deleteAll(_fields); }

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

void test2() {
    Reflection2 reflection;
    // reflection
    //     .beginClass<MyClass1>("MyClass1");
    //         .addField("myInt", myInt)
    //     .endClass();

    MyClass2 myClass;

    // Metadata md = reflection.getMetadata(myClass);
    // cout << md.getName();
    // cout << md.getFields(0).getName() << " "
    //      << md.getFields(0).getTypename() << md.getFields(0).toString();
}

static size_t TypeIdCounter = 0;

template <class T> class Type1 {
    static bool MustGenerateTypeId;

    static size_t TypeId;

public:
    Type1() {
        if (MustGenerateTypeId) {
            TypeId = ++TypeIdCounter;
            MustGenerateTypeId = false;
        }
    }

    static size_t GetTypeId() { return TypeId; }
};

template <class T> size_t Type1<T>::TypeId;
template <class T> bool Type1<T>::MustGenerateTypeId = true;

class Metadata1 {
    string _name;

public:
    Metadata1(const string& name) : _name(name) { }

    inline const string& getName() const { return _name; }

    virtual size_t getTypeId() = 0;

    virtual const string toString() = 0;
};

class FieldInfo1 : public Metadata1 {
public:
    FieldInfo1(const string& name) : Metadata1(name) { }
};

template <class T> class ConcreteFieldInfo1 : public FieldInfo1 {
    Type1<T> _type;

    T& _ref;

public:
    ConcreteFieldInfo1(const string& name, T& ref)
        : FieldInfo1(name), _ref(ref)
    { }

    virtual size_t getTypeId() { return _type.GetTypeId(); }

    virtual const string toString() {
        return ::toString(_ref);
    }
};

class Reflection1
{
    vector<FieldInfo1*> _fields;

public:
    virtual ~Reflection1() {
        deleteAll(_fields);
    }

    template <class T>
    void addField(const string& name, T& field) {
        FieldInfo1* fieldInfo = new ConcreteFieldInfo1<T>(name, field);
        _fields.push_back(fieldInfo);
    }

    inline vector<FieldInfo1*> getFieldInfoList() const { return _fields; }

    inline Metadata1& getFieldInfo(int index) { return *(getFieldInfoList()[index]); }

    inline size_t countFieldInfoList() const { return _fields.size(); }
};

void print(Metadata1& fieldInfo)
{
    cout << fieldInfo.getName()
        << " (typeId " << fieldInfo.getTypeId() << ")"
        << ": " << fieldInfo.toString()
        << endl;
}

void test1()
{
    int x = 42;
    float f = 3.1415f;
    int x2 = 43;

    Reflection1 reflection;
    reflection.addField("x", x);
    reflection.addField("f", f);
    reflection.addField("x2", x2);

    for (size_t i = 0; i < reflection.countFieldInfoList(); i++)
        print(reflection.getFieldInfo(i));
}

template<class T, class UnaryPredicate>
vector<T*> find_all(const vector<T*>& elements, UnaryPredicate pred) {
    vector<T*> result;
    for (size_t i = 0; i < elements.size(); i++)
        if (pred(*elements[i]))
            result.push_back(elements[i]);

    return result;
}

namespace t1 {
    class EntityInfo {
        string _name;

    public:
        EntityInfo(const string& name) : _name(name) { }

        const string& getName() const { return _name; }

        virtual const string toString() const = 0;
    };

    class VariableInfo : public EntityInfo {
    public:
        VariableInfo(const string& name) : EntityInfo(name) { }
    };

    template <class Var> class ConcreteVariableInfo : public VariableInfo {
        Var& _var;

    public:
        ConcreteVariableInfo(const string& name, Var& var)
            : VariableInfo(name), _var(var) { }

        virtual const string toString() const {
            return ::toString(_var);
        }
    };

    class Reflection {
        vector<VariableInfo*> _variables;

    public:
        virtual ~Reflection() { deleteAll(_variables); }

        vector<VariableInfo*> getVariables() const { return _variables; }

        inline VariableInfo& getVariable(size_t index) { return *_variables[index]; }

        template <class Var> void addVariable(const string& name, Var& var) {
            VariableInfo* variable = new ConcreteVariableInfo<Var>(name, var);
            _variables.push_back(variable);
        }
    };

    void print(VariableInfo& variable) {
        cout << variable.getName()
            << ": " << variable.toString()
            << endl;
    }

    void test() {
        int x = 42;

        Reflection rf;
        rf.addVariable("x", x);

        print(rf.getVariable(0));
    }
}

namespace t2 {
    class MyClass {
    };

    class ClassInfo {
        string _name;

    public:
        ClassInfo(const string& name) : _name(name) { }

        const string getName() const { return _name; }

        const string toString() { return "<Class>"; }
    };

    template <class C> class ConcreteClassInfo : public ClassInfo {
    public:
        ConcreteClassInfo(const string& name) : ClassInfo(name) { }
    };

    class Reflection {
        vector<ClassInfo*> _classes;

    public:
        virtual ~Reflection() { deleteAll(_classes); }

        template <class C> void addClass(const string& name) {
            ClassInfo* info = new ConcreteClassInfo<C>(name);
            _classes.push_back(info);
        }

        ClassInfo& getClass(size_t index) { return *_classes[index]; }
    };

    void print(ClassInfo& theClass) {
        cout << theClass.getName()
            << ": " << theClass.toString()
            << endl;
    }

    void test() {
        Reflection rf;
        rf.addClass<MyClass>("MyClass");

        print(rf.getClass(0));
    }
}

namespace my {
    template <class Key, class Val> Val& find(const Key& key, map<Key, Val>& m) {
        return m.find(key)->second;
    }

    template <class T, class U> void deleteAll(map<T, U>& m) {
        for (typename map<T, U>::iterator it = m.begin(); it != m.end(); ++it)
            delete it->second;
    }
}

namespace t3 {
    class VariableInfo {
        string _name;
    public:
        VariableInfo(const string& name) : _name(name) { }
        const string& getName() const { return _name; }
        virtual void inspect() = 0;
    };

    template <class Var, class TInspector> class ConcreteVariableInfo : public VariableInfo {
        Var& _ref;
    public:
        ConcreteVariableInfo(const string& name, Var& ref) : VariableInfo(name), _ref(ref) { }

        virtual void inspect() {
            TInspector::inspectVariable(_ref);
        }
    };

    class DefaultInspector {
    public:
        template <class Var> static void inspectVariable(Var& var) { }
    };

    template <class TInspector = DefaultInspector> class Reflection {
        map<string, VariableInfo*> _variables;
    public:
        virtual ~Reflection() { my::deleteAll(_variables); }

        template <class Var> void addVariable(const string& name, Var& var) {
            VariableInfo* info = new ConcreteVariableInfo<Var, TInspector>(name, var);
            _variables[name] = info;
        }

        VariableInfo& getVariable(const string& name) {
            return *my::find(name, _variables);
        }
    };

    class Inspector {
    public:
        template <class Var> static void inspectVariable(Var& var) {
            cout << "Raw variable: " << var << endl;
#ifdef CPP_11
            cout << "is integral: " << std::is_integral<Var>::value << endl;
#endif
        }
    };

    void print(VariableInfo& info) {
        cout << "inspect " << info.getName() << endl;
        info.inspect();
    }

    void test() {
        int myInt = 42;
        float myFloat = 3.1415f;

        Reflection<Inspector> rf;
        rf.addVariable("myInt", myInt);
        rf.addVariable("myFloat", myFloat);
        print(rf.getVariable("myInt"));
        print(rf.getVariable("myFloat"));
    }
}

namespace t4 {
    class Reflection {
    public:
    };

    struct Vector2f {
        float x, y;
        Vector2f(float x_ = 0, float y_ = 0) : x(x_), y(y_) { }
    };

    template <class Object> void xmlSerialize(Object& o, Reflection& rf) {
    }

    void test() {
        Vector2f vec(1, 2);
        //Reflection rf;
        // rf.beginClass<Vector2f>("Vector2f");
        //     .addProperty(&Vector2f::x);
        //     .addProperty(&Vector2f::y);
        // endClass();
        // xmlSerialize(vec, rf);

        // Expected output:
        // References:
        //     https://gist.github.com/perezite/c08c3b5458a66db3a936490d53014b06
        //     https://docs.microsoft.com/en-us/dotnet/standard/serialization/examples-of-xml-serialization
        // <Vector2f>
        //    <x>1</x>
        //    <y>2</y>
        // </Vector>
    }
}

namespace t5 {
    class Reflection { };

    class ObjectInfo { };

    void indent(size_t depth) {
        cout << string(depth * 4, ' ');
    }

    template <class T> void xmlSerialize(ObjectInfo& info, size_t depth = 0) {
        // if (info.isCollection()) {
        //     CollectionInfo collectionInfo = info.getCollectionInfo();
        //     for (size_t i = 0; i < collectionInfo.size(); i++)
        //         xmlSerialize(collectionInfo[i], depth + 1);
        // }
        // else if (info.isSimple()) {
        //     cout << "<" << info.getName() << ">" << info.toString() << "</" << info.getName() << ">" << endl;
        // }
        // else {
        //     throw string("not supported");
        // }
    }

    void test() {
        //Reflection rf;

        vector<int> numbers;
        numbers.push_back(42);
        numbers.push_back(43);
        // ObjectInfo info = rf.getInfo(numbers);
        //xmlSerialize(info, rf);

        /*  Expected Output:
            <numbers>
                <e>42</e>
                <e>43</e>
            </numbers>

        */
    }
}

namespace t5a {
    template <typename Class, typename, typename> struct test_struct { };

    template <class T> void getElements();
    template <> void getElements<int>() { }

    void test() {
        getElements<int>();
        // getElements<float>();           // linker error
    }
}

namespace my {
    template <class T>
    void join(vector<T>& v, ostringstream& os, const string& delimiter) {
        if (v.empty())
            return;

        for (size_t i = 0; i < v.size() - 1; i++)
            os << v[i] << delimiter;
        os << v[v.size() - 1];
    }
}

namespace t5b {
    // https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time

    template <typename T> struct ContainerInspector {
        static const bool isContainerType = false;

        static void getContainerInfo(T& t) {
            cout << "get non-container info" << endl;
        }
    };

    template <typename T, typename Alloc>
    struct ContainerInspector<vector<T, Alloc> > {
        static const bool isContainerType = true;

        static void getContainerInfo(vector<T, Alloc>& vec) {
            cout << "get vector info" << endl;

            ostringstream os;
            my::join(vec, os, ", ");
            cout << os.str() << endl;
        }
    };

    template <typename T, typename U, typename Alloc>
    struct ContainerInspector<map<T, U, Alloc> > {
        static const bool isContainerType = true;

        static void getContainerInfo(map<T, U, Alloc>& m) {
            cout << "get map info" << endl;
        }
    };

    template <class T>
    void check(T& t) {
        ContainerInspector<T>::getContainerInfo(t);
        cout << ContainerInspector<T>::isContainerType << endl;
    }

    void test() {
        vector<float> v{ 1, 2, 3 };
        map<int, int> m;
        int i;

        check(v);
        check(m);
        check(i);
    }
}

namespace t6 {
    template <class T>
    struct FieldInfo {
        const T& _ref;

        FieldInfo(const T& ref) : _ref(ref) { }

        template <class Inspector>
        void inspect(Inspector& inspector) {
            inspector.inspect(_ref);
        }
    };

    struct MyInspector {
        template <class T> void inspect(const T& t) {
            cout << "inspecting unknown type" << endl;
        }

        template <> void inspect<int>(const int& i) {
            cout << "inspecting int value: " << i << endl;
        }
    };

    void test() {
        int i = 42;
        float f = 3.1415f;

        FieldInfo<int> info1(i);
        FieldInfo<float> info2(f);

        MyInspector inspector;
        info1.inspect(inspector);
        info2.inspect(inspector);
    }
}

namespace t7 {
    template <class T> string stringify(const T& t) {
        ostringstream os;
        os << t;
        return os.str();
    }

    struct Person {
        int age = 42;
    };

    template <class T> struct TypeInfo {
        static const bool IsClass = false;

        const T& _ref;

        TypeInfo(const T& ref) : _ref(ref) { }

        string toString() const { return stringify(_ref); }
    };

    template <> struct TypeInfo<Person> {
        static const bool IsClass = true;

        const Person& _ref;

        TypeInfo(const Person& ref) : _ref(ref) { }

        string toString() const { return "nope"; }

        template <class Serializer> void serialize(Serializer& serializer) {
            serializer.serialize<Person>(_ref);
        }
    };

    //template <> struct TypeInfo<Person> : public ClassTypeInfo<Person> {
    //    TypeInfo(const Person& ref) : ClassTypeInfo(ref) { }
    //    template <class Serializer> void serialize(Serializer& serializer) {
    //         serializer.serialize(age);
    //    }
    //};

    struct MySerializer {
        template <class T> void serialize(const TypeInfo<T>& typeInfo) {
            if (TypeInfo<T>::IsClass)
                serializeClass<T>(typeInfo);
            else
                serializeObject<T>(typeInfo);
        }

        template <class T> void serializeClass(const TypeInfo<T>& typeInfo) {
            /*
                for (size_t i = 0; i < typeInfo.countFields(); i++) {
                    typeInfo.getField(i).serialize<MySerializer>(*this);
                }
            */
        }

        template <class T> void serializeObject(const TypeInfo<T>& typeInfo) {
            cout << typeInfo.toString() << endl;
        }
    };

    void test() {
        int i = 123;
        Person person;
        MySerializer serializer;
        serializer.serialize<int>(i);
        serializer.serialize<Person>(person);
    }
}

namespace t8 {
    struct Person {
        float height = 1.7f;
    };

    template <class T> string stringify(T& obj) {
        ostringstream os;
        os << obj;
        return os.str();
    }

    enum class TypeCategory { Simple, Composite };

    template <class T> struct Object {
        virtual TypeCategory getTypeCategory() const = 0;

        virtual string toString() const = 0;
    };

    template <class T> struct SimpleObject : public Object<T> {
        T& _obj;

        SimpleObject(T& obj) : _obj(obj) { }

        virtual TypeCategory getTypeCategory() const { return TypeCategory::Simple; }

        virtual string toString() const { return stringify(_obj); }
    };

    template <class T, class Inspector> Object<T>* inspect(T& obj, Inspector& insp, size_t depth) {
        return new SimpleObject<T>(obj);
    }

    void printLine(const string& name, const string& str, size_t depth) {
        cout << string(depth * 4, ' ') << name << ": " << str << endl;
    }

    struct Writer {
        template <class T> void inspect(const string& name, T& t, size_t depth = 0) {
            Object<T>* object = t8::inspect<T, Writer>(t, *this, depth);
            if (object && object->getTypeCategory() == TypeCategory::Simple)
                printLine(name, object->toString(), depth);
        }
    };

    template <> Object<Person>* inspect<Person, Writer>(Person& person, Writer& writer, size_t depth) {
        writer.inspect("height", person.height, depth + 1);
        return NULL;
    }

    template <class T, class U> struct temp {
        static void call() { }
    };

    template <class T> struct temp<T, int> {
        static void call() { }
    };

    void test() {
        int i = 42;
        Person person;
        Writer writer;
        writer.inspect<int>("i", i);
        writer.inspect<Person>("person", person);
    }
}

namespace t9 {
    struct Person {
        int age = 42;
    };

    string indent(size_t depth) {
        return string(4 * depth, ' ');
    }

    enum class Type { Simple, Composite };

    template <class Serializer, class T> struct Inspector {
        static const Type TheType = Type::Simple;

        template <class Serializer> static string inspect(const string& name, T& t, Serializer& serializer, size_t depth = 0) {
            ostringstream os; os << t;
            return os.str();
        }
    };

    template <class Serializer> struct Inspector<Serializer, Person> {
        static const Type TheType = Type::Composite;

        template <class Serializer> static string inspect(const string& name, Person& person, Serializer& serializer, size_t depth = 0) {
            serializer.serialize("age", person.age, depth + 1);
            return "";
        }
    };

    struct Writer {
        template <class T> void serialize(const string& name, T& t, size_t depth = 0) {
            typedef Inspector<Writer, T> TheInspector;

            if (TheInspector::TheType == Type::Simple) {
                string val = TheInspector:: template inspect<Writer>(name, t, *this);
                cout << indent(depth) << name << ": " << val << endl;
            }
            else {
                cout << indent(depth) << name << ": ClassObject" << endl;
                TheInspector::template inspect<Writer>(name, t, *this, depth + 1);
            }
        }
    };

    struct Employee {
        vector<string> jobs = { "Chef", "Programmer" };
    };

    /*
    *
        class Employee {
            vector<string> jobs = { "Chef", "Programmer" } ;
        }

        rf.beginClass<Person>()
            .addCollection(jobs)
        .endClass();

    */

    void test() {
        Person person;
        Writer writer;
        writer.serialize("person", person);

        // Expected:
        //     person : ClassObject
        //         height: 42
    }
}

namespace t10 {
    typedef char Invalid[sizeof(vector<void*>) + 1];

    char func1() { return ' '; }

    int func2(int) { return 42; }

    template <int(*)(int)> struct call {
        static const bool IsCall = true;
    };

    template <class T> struct call2 {
        static const bool IsCall = false;
    };

    class SomeClass { };

    template <class T> float func() {
    }

    template <> float func<int>() {
        return 1;
    }

    void test() {
        cout << call<func2>::IsCall << endl;
        cout << sizeof(func1()) << endl;
        cout << sizeof(vector<int>) << endl;
        cout << sizeof(vector<double>) << endl;
        cout << sizeof(vector<void*>) << endl;
        cout << sizeof(Invalid) << endl;

        //cout << call2<func1>::IsCall << endl;
    }
}

namespace t11 {
    struct CollectionInfo { };

    template <class T> vector<CollectionInfo*> getCollection(T& t) {
        cout << "get generic collection" << endl;
        return vector<CollectionInfo*>();
    }

    template <class T> vector<CollectionInfo*> getCollection(vector<T>& v) {
        cout << "get vector collection" << endl;
        return vector<CollectionInfo*>();
    }

    template <class T, class U> vector<CollectionInfo*> getCollection(map<T, U>& m) {
        cout << "get map collection" << endl;
        return vector<CollectionInfo*>();
    }

    template <class T> struct ObjectInfo {
        T& _ref;

        ObjectInfo(T& ref) : _ref(ref) { }

        vector<CollectionInfo*> getCollection() {
            return t11::getCollection(_ref);
        }
    };

    void test() {
        int i = 42;
        vector<int> v = { 1, 2, 3 };
        map<string, int> m = { {"one", 1}, {"two", 2}, {"three", 3} };

        ObjectInfo<int> intInfo(i);
        ObjectInfo<vector<int>> vectorInfo(v);
        ObjectInfo<map<string, int>> mapInfo(m);
        intInfo.getCollection();
        vectorInfo.getCollection();
        mapInfo.getCollection();
    }
}

namespace t12 {
    template <class T> void inspect(T& t) {
        cout << "generic inspect" << endl;
    }

    template <class T> void inspect(vector<T>& v) {
        cout << "vector inspect" << endl;
    }

    template <class K, class V> void inspect(map<K, V>& m) {
        cout << "map inspect" << endl;
    }

    template <class T> void callInspect(T& t) {
        // beware: inspect<T>(t) will always call generic inspect!
        inspect(t);     // here, generic parameters are implicitly forwarded
    }

    void test() {
        int i = 42;
        vector<float> v = { 3.1415f, 2.5f };
        map<string, double> m = { { "one", 1 }, { "two", 2 } };
        callInspect(i);
        callInspect(v);
        callInspect(m);
    }
}

namespace t13 {
    struct Person {
        string name = "Peter";
        vector<int> numbers = { 1, 2, 3 };
    };

    string indent(size_t depth) {
        return string(depth * 4, ' ');
    }

    template <class T> void print(const string& key, T& t, size_t depth = 0) {
        ostringstream os;
        os << t;
        cout << indent(depth) << "\"" << key << "\": " << "\"" << os.str() << "\"";
    }

    template <class T, class Inspector> void inspectField(const string& name, T& t, Inspector& inspector, size_t depth = 0) {
        print(name, t, depth);
    }

    template <class T, class Inspector> void inspectObject(const string& name, T& t, Inspector& inspector, size_t depth = 0) {
        cout << "[inspectObject not implemented for this type]";
    }

    template <class T, class Inspector> void inspectCollection(const string& name, T& t, Inspector& inspector, size_t depth = 0) {
        cout << "[inspectCollection not implemented for this type]";
    }

    template <class Inspector, class T> void inspectCollection(const string& name, vector<T>& v, Inspector& inspector, size_t depth = 0) {
        cout << indent(depth) << "\"" << name << "\": " << "[";
        for (size_t i = 0; i < v.size(); i++) {
            string comma = i == 0 ? "" : ",";
            cout << comma << " \"" << v[i] << "\"";
        }
        cout << " ]" << endl;
    }

    template <class Inspector> void inspectObject(const string& name, Person& p, Inspector& inspector, size_t depth = 0) {
        cout << indent(depth) << "{" << endl;
        inspectField("name", p.name, inspector, depth + 1);
        cout << "," << endl;
        inspectCollection("numbers", p.numbers, inspector, depth + 1);
        cout << indent(depth) << "}" << endl;
    }

    struct Writer { };

    void test() {
        Person peter;
        Writer writer;

        inspectObject("person", peter, writer);

        // Expected output
        // {
        //     "name": "Peter"
        //     "numbers": [ "1", "2", "2" ]
        // }
    }
}

namespace t14 {
    template <class Inspector> struct SimpleMemberInfo {
        string _name;

        SimpleMemberInfo(const string& name) : _name(name) { }

        string getName() const { return _name; }

        virtual void inspect(Inspector& inspector) = 0;
    };

    template <class Inspector, class T> struct ConcreteSimpleMemberInfo : public SimpleMemberInfo<Inspector> {
        T& _ref;

        ConcreteSimpleMemberInfo(const string& name, T& ref) : SimpleMemberInfo<Inspector>(name), _ref(ref) { }

        virtual void inspect(Inspector& inspector) {
            inspector.inspectSimpleMember(SimpleMemberInfo<Inspector>::getName(), _ref);
        }
    };

    template <class Inspector, class T>
    SimpleMemberInfo<Inspector>* getSimpleMemberInfo(const string& name, T& t) {
        return new ConcreteSimpleMemberInfo<Inspector, T>(name, t);
    }

    struct Writer {
        template <class T> void inspectSimpleMember(const string& name, T& t) {
            cout << name << ": " << t << endl;
        }
    };

    void test() {
        int i = 42;
        Writer writer;
        SimpleMemberInfo<Writer>* info = getSimpleMemberInfo<Writer>("i", i);
        info->inspect(writer);
    }
}

namespace t15 {
    struct Person {
        int age = 42;
    };

    template <class C> struct PropertyInfo {
        // virtual void inspect(I& inspector) = 0
    };

    template <class C, class P> struct ConcretePropertyInfo : public PropertyInfo<C> {
        //virtual void inspect(I& inspector) {
        //
        //}
    };

    template <class C> struct ClassInfo {
        vector<PropertyInfo<C>*> _properties;

        template <class P> void addProperty(const string& name, P C::* prop) {
            _properties.push_back(new ConcretePropertyInfo<C, P>());
        }

        vector<PropertyInfo<C>*> getProperties() { return _properties; }
    };

    template <class C> ClassInfo<C> getClassInfo() { return ClassInfo<C>(); }

    template <> ClassInfo<Person> getClassInfo<Person>() {
        ClassInfo<Person> info;
        info.addProperty("age", &Person::age);
        return info;
    }

    struct LuaRegistry {
        template <class C> void beginClass(const string& name) {
            cout << "LuaRegistry::beginClass(\"" << name << "\")" << endl;
        }
    };

    struct LuaBinder {
        LuaRegistry _registry;

        template <class T> void inspectProperty() {
            //_registry.
        }

        template <class C> void bindClass(const string& name) {
            _registry.beginClass<C>(name);
            ClassInfo<C> classInfo = getClassInfo<C>();
            vector<PropertyInfo<C>*> props = classInfo.getProperties();
            for (size_t i = 0; i < props.size(); i++) {
                //    props[i]->inspect(*this);
            }
        }
    };

    void test() {
        Person person;

        auto test = &Person::age;

        LuaBinder binder;
        binder.bindClass<Person>("Person");
    }
}

namespace t16 {
    class Writer { };

    class Person { int age = 42; };

    template <class I> class ClassInfo {
    };

    template <class C, class I> ClassInfo<I> getClassInfo() { return ClassInfo<I>(); }

    template <class I> ClassInfo<I> getClassInfo(Person* person) {
        return ClassInfo<I>();
    }

    void test() {
        ClassInfo<Writer> personClassInfo = getClassInfo<Writer>((Person*)NULL);
    }
}

namespace t17 {
    template<class T> struct Visitor { };

    template<class Cont> struct StlVisitor {
        Cont& c;
        typename Cont::iterator it;

        StlVisitor(Cont& c_) : c(c_), it(c_.begin()) { }

        typename Cont::value_type& get() { return *it; }

        void next() { it++; }

        bool end() { return it == c.end(); }
    };

    template <class T> struct Visitor<vector<T>> : public StlVisitor<vector<T>> {
        Visitor(vector<T>& v) : StlVisitor<vector<T>>(v) { }
    };

    template <class K, class V> struct Visitor<map<K, V>> : public StlVisitor<map<K, V>> {
        Visitor(map<K, V>& m) : StlVisitor<map<K, V>>(m) { }
    };

    template <class T> void printValue(T& t) {
        cout << t << endl;
    }

    template <class K, class V> void printValue(pair<K, V>& p) {
        cout << p.first << ": " << p.second << endl;
    }

    template <class V> void print(V& vis) {
        while (!vis.end()) {
            printValue(vis.get());
            vis.next();
        }
    }

    void test() {
        vector<int> v{ 1, 2, 3 };
        map<string, int> m{ {"z", 1}, {"y", 2}, {"x", 3} };
        Visitor<vector<int>> vvis(v);
        Visitor<map<string, int>> mvis(m);
        print(vvis);
        print(mvis);
    }
}

namespace t18 {
    // check if free function overload exists for given type
    // Combination of the following sources:
    // https://stackoverflow.com/questions/38717341/test-if-a-not-class-member-function-exists-sfinae
    // https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature

    void print(int x) { }

    typedef char No[1];
    typedef char Yes[2];

    template<typename T, void(*)(T)> struct Sfinae {};

    template <class T> Yes& check(Sfinae<T, print>*) { }

    template <class T> No& check(...) { }

    template <class T> struct has_print {
        template<typename T, void(*)(T)> struct Sfinae {};

        template <class T> static Yes& test(Sfinae<T, print>*) { }

        template <class T> static No& test(...) { }

        static const bool value = sizeof(test<T>(0)) == sizeof(Yes);
    };

    void test() {
#pragma warning( push )
#pragma warning( disable : 4101)
        Sfinae<int, print> temp;
#pragma warning( pop )
        //Sfinae<float, print> temp;                // compile error

        cout << "1 = no, 2 = yes" << endl;
        cout << sizeof(check<int>(0)) << endl;      // returns 2
        cout << sizeof(check<float>(0)) << endl;    // returns 1

        cout << boolalpha;
        cout << has_print<int>::value << endl;
        cout << has_print<float>::value << endl;
    }
}

namespace t19 {
    template<class T> struct Visitor { };

    template<class Cont> struct StlVisitor {
        Cont& c;
        typename Cont::iterator it;

        StlVisitor(Cont& c_) : c(c_), it(c_.begin()) { }

        typename Cont::value_type& get() { return *it; }

        void next() { it++; }

        bool end() { return it == c.end(); }
    };

    template <class T> struct Visitor<vector<T>> : public StlVisitor<vector<T>> {
        Visitor(vector<T>& v) : StlVisitor<vector<T>>(v) { }
    };

    template <class K, class V> struct Visitor<map<K, V>> : public StlVisitor<map<K, V>> {
        Visitor(const map<K, V>& m) : StlVisitor<map<K, V>>(m) { }
    };

    struct Writer {
        template <class T> void inspectSimple(T& t) {
            cout << t << endl;
        }

        template <class T> void inspectCollection(T& t) {
            Visitor<T> vis(t);
            while (!vis.end()) {
                inspectSimple(vis.get());
                vis.next();
            }
        }
    };

    void test() {
        pair<string, int> bla;
        cout << bla.first << endl;

        //cout << is_class<int>::value << endl;

        float f;
        cin >> f;
        cout << f << endl;
        vector<int> v = { 1, 2, 3 };
        Writer writer;
        Visitor<vector<int>> temp(v);
        writer.inspectCollection(v);
    }
}

namespace t20 {
    // TODO: try out https://stackoverflow.com/questions/35213658/how-does-this-implementation-of-stdis-class-work
    // to implement an is_class template
    // this would allow us to distinguish between class types, simple types and possibly collection types
    // Note: The latter would be the case, if a Visitor without a field Visitor<T>::NotImplemented is defined. The
    // field Visitor<T>::NotImplemented is defined inside the default visitor (but of course not in the specialized Visitors).

    typedef char no[1];
    typedef char yes[2];

    template <class T> yes& check(int T::*) { }
    template <class T> no& check(...) { }

    class TheClass { };
    struct NotPrintable { };

    void test() {
        cout << sizeof(check<TheClass>(0)) << endl;
        cout << sizeof(check<NotPrintable>(0)) << endl;
        cout << sizeof(check<int>(0)) << endl;
    }
}

namespace t21 {
    typedef char no[1];
    typedef char yes[2];

    template <class T> struct IsClass {
        template <class U> static yes& check(int U::*) { }
        template <class U> static no& check(...) { }
        static const bool value = sizeof(check<T>(0)) == sizeof(yes);
    };

    class TheClass { };
    template <class T> class TheTemplatedClass { };
    struct NotPrintable { };

    void test() {
        cout << boolalpha;
        cout << IsClass<TheClass>::value << endl;
        cout << IsClass<TheTemplatedClass<int>>::value << endl;
        cout << IsClass<NotPrintable>::value << endl;
        cout << IsClass<int>::value << endl;
    }
}

namespace t22 {
    template <class T> void print(T& t) { cout << t << endl; }

    typedef char no[1];
    typedef char yes[2];

    template <class T, void (*)(T&)> struct Sfinae { };
    template <class T> yes& check(Sfinae<T, print<T>>*) { }
    template <class T> no& check(...) { }

    struct NotPrintable { };

    void test() {
        cout << sizeof(check<int>(0)) << endl;
        cout << sizeof(check<NotPrintable>(0)) << endl;
    }
}

namespace t23 {
    struct NotPrintable { };
    struct Printable { };

    template <class T> void print(T& t) { cout << t << endl; }
    void print(Printable& p) { cout << "printing specialized" << endl; }

    void call(void(*func)(Printable& p)) { cout << "call specialized" << endl; }
    template <class T> void call(void(*func)(T&)) { cout << "call generic" << endl; }

    template <class T> char check(T* t) { return char(); }
    float check(Printable* p) { return float(); }

    void test() {
        int i = 42;
        Printable p;
        print<int>(i);
        print(p);
        call(print);
        call(print<int>);
        int* pi = NULL;
        Printable* pp = NULL;
        cout << sizeof(check((int*)NULL)) << endl;
        cout << sizeof(check((Printable*)NULL)) << endl;
    }
}

namespace t24 {
    class Printable { };

    template <class T> void print(T& t) { cout << t << endl; }
    void print(Printable& p) { cout << "printable" << endl; }

    typedef char no[1];
    typedef char yes[2];

    template <class T, void (*)(T&)> struct Sfinae { };
    //template <class T> yes& check(Sfinae<T, print<T>>*) { }
    template <class T> yes& check(Sfinae<T, &print<T>>*) { }
    template <class T> no& check(...) { }

    struct NotPrintable { };

    template <class T> struct TheCheck {
        void call(T* t) { print(*t); }
    };

    template <class T> void myCheck(TheCheck<T>* theCheck) { cout << "match" << endl; }
    template <class T> void myCheck(...) { cout << "no match" << endl; }

    void test() {
        myCheck<int>(0);
        myCheck<NotPrintable>(0);
        int i = 42;
        print(i);
        //cout << check<Printable>(0) << endl;
        //cout << sizeof(check<NotPrintable>(0)) << endl;
    }
}

namespace t25 {
    struct Printable { };

    template <class T> void print(T& t) { cout << t << endl; }
    template <class T> void print(Printable& p) { cout << "print(Printable&)" << endl; }

    typedef char no[1];
    typedef char yes[2];

    template <class T, void (*)(T&)> struct Sfinae { };
    template <class T> yes& check(Sfinae<T, print<T>>*) { }
    template <class T> no& check(...) { }

    struct NotPrintable { };

    void test() {
        cout << sizeof(check<int>(0)) << endl;
        cout << sizeof(check<Printable>(0)) << endl;
        cout << sizeof(check<NotPrintable>(0)) << endl;
    }
}

namespace t26 {
    struct Printable { };
    struct NotPrintable { };

    template <class T> void print(T& t) { cout << t << endl; }
    template <class T> void print(Printable& p) { cout << "print(Printable&)" << endl; }

    template <class T> char probe() {
        T* t = NULL;
        print(*t);
        return 42;
    }

    template <class T> struct hasPrint {
        typedef char no[1];
        typedef char yes[2];

        template <class U, void (*)(U&)> struct Sfinae { };
        template <class T> static yes& check(Sfinae<T, print<T>>*) { }
        template <class T> static no& check(...) { }
        static const size_t dummy = sizeof(check<char>(0));      // HACK: needed to prevent weird compile error...
        static const bool value = sizeof(check<T>(0)) == sizeof(yes);
    };

    void test() {
        cout << boolalpha;
        cout << hasPrint<NotPrintable>::value << endl;      // would fail if this is the first check and the HACK above did not exist
        cout << hasPrint<Printable>::value << endl;
        cout << hasPrint<int>::value << endl;
        cout << hasPrint<char>::value << endl;

        probe<int>();
        //probe<NotPrintable>();
    }
}

namespace t27 {
    struct Printable { };

    template <class T> void print(T& t) { cout << t << endl; }
    void print(Printable& p) { cout << "print(Printable&)" << endl; }
    template <class T> void printWrap(T& t) { print(t); }

    typedef char no[1];
    typedef char yes[2];

    template <class T, void (*)(T&)> struct Sfinae { };
    template <class T> char check(Sfinae<T, &printWrap<T>>*) { cout << "check1" << endl; return char(); }
    template <class T> int check(...) { cout << "check2" << endl; return int(); }
    static const size_t dummy = sizeof(check<char>(0));      // HACK: needed to prevent weird compile error...

    void myTest();

    void test() {
        check<int>(0);
        check<Printable>(0);

        //Sfinae<Printable, &printWrap<Printable>>* sfinae;

        //cout << sizeof(check<int>(0)) << endl;
        //cout << sizeof(check<Printable>(0)) << endl;
    }
}

namespace t28 {
    void overloaded(int a) { }
    void overloaded(float b) { }

    struct MyStruct {
        void operator<<(int a) { }
        void operator<<(float a) { }
    };

    template <void(MyStruct::*)(int)> struct MyTest {  };
    template <class T, ostringstream& (*)(T)> struct MyTest2 { };

    // basic_ostream& __CLR_OR_THIS_CALL operator<<(int _Val) { // insert an int
    struct SomeStruct { };

    void test() {
        //auto temp = &MyStruct::operator+;
        //MyTest<&MyStruct::operator<< > temp2;
        //int i = 42;
        //ostringstream os; os << i << endl;
        //ostringstream& (ostringstream::*temp3) (int);
        //MyTest2<int, &MyStruct::operator<< > temp4;
        //MyTest2<int, (&ostringstream::operator<<) > temp4;
    }
}

namespace t29 {
    template <class T> string stringify(T& t) { ostringstream os; os << t; return os.str(); }
    string toString(int i) { return stringify(i); }
    string toString(float f) { return stringify(f); }
    template <class T> int toString(T& t) { return 0; }

    struct SomeStruct { };

    template <class T> struct hasPrint {
        static const bool value = sizeof(toString(*((T*)NULL))) == sizeof(string);
    };

    void test() {
        cout << boolalpha;
        cout << hasPrint<int>::value << endl;
        cout << hasPrint<float>::value << endl;
        cout << hasPrint<unsigned int>::value << endl;
        cout << hasPrint<double>::value << endl;
        cout << hasPrint<SomeStruct>::value << endl;
    }
}

namespace t30 {
    template <class T> string stringify(T& t) { ostringstream os; os << t; return os.str(); }
    string toString(int i) { return stringify(i); }
    string toString(float f) { return stringify(f); }
    template <class T> int toString(T& t) { return 0; }

    struct SomeStruct { };

    template <class T> struct hasPrint {
        static const bool value = sizeof(toString(*((T*)NULL))) == sizeof(string);
    };

    template <class T, bool> struct Extract;
    template <class T> struct Extract<T, true> {
        static string asString(T& t) { return toString(t); }
    };
    template <class T> struct Extract<T, false> {
        static string asString(T& t) { return "<unknown>"; }
    };

    template <class T> string getStringValue(T& t) {
        return Extract<T, hasPrint<T>::value>::asString(t);
    }

    void test() {
        int i = 42;
        SomeStruct s;
        cout << getStringValue(i) << endl;
        cout << getStringValue(s) << endl;
    }
}

namespace t31 {
    // https://stackoverflow.com/questions/6534041/how-to-check-whether-operator-exists
    struct A { int  a; };
    bool operator==(const A& l, const A& r) { return (l.a == r.a); }

    struct B {
        int  b;
        bool operator==(const B& rhs) const { return (b == rhs.b); }
    };

    struct C { };

    namespace CHECK
    {
        class No { bool b[2]; };
        template<typename T, typename Arg> No operator== (const T&, const Arg&);

        bool Check(...);
        No& Check(const No&);

        template <typename T, typename Arg = T> struct EqualExists
        {
            enum { value = (sizeof(Check(*(T*)(0) == *(Arg*)(0))) != sizeof(No)) };
        };
    }

    void test() {
        cout << CHECK::EqualExists<A>::value << endl;
        cout << CHECK::EqualExists<B>::value << endl;
        cout << CHECK::EqualExists<C>::value << endl;
    }
}

namespace t32 {
    // https://stackoverflow.com/questions/6534041/how-to-check-whether-operator-exists
    struct NoStream { };
    struct WithStream { };

    ostream& operator<<(ostream& os, const WithStream& ws) { os << "WithStream::operator<<" << endl; return os; }

    template<typename T> char operator<< (ostream& os, const T&);

    void test() {
        cout << sizeof(*(ostringstream*)(0) << *(double*)(0)) << endl;
        cout << sizeof(*(ostringstream*)(0) << *(int*)(0)) << endl;
        cout << sizeof(*(ostringstream*)(0) << *(WithStream*)(0)) << endl;
        cout << sizeof(*(ostringstream*)(0) << *(NoStream*)(0)) << endl;
    }
}

namespace t33 {
    // https://stackoverflow.com/questions/6534041/how-to-check-whether-operator-exists
    namespace HasStream {
        typedef char No[sizeof(ostringstream&) + 1];
        template<typename T> No& operator<< (ostream& os, const T&);

        template <class T> struct StreamExists {
            enum {                      // neded to make 'value' a compile time constant
                value = sizeof(*(ostringstream*)(0) << *(T*)(0)) != sizeof(No&)
            };
        };
    };

    template <class T> struct hasStream {
        static const bool value = HasStream::StreamExists<T>::value;
    };

    struct WithoutStream { };
    struct WithStream { };
    ostream& operator<<(ostream& os, const WithStream& ws) { os << "WithStream::operator<<" << endl; return os; }

    template <bool> struct CompileTimeCheck { };
    template <> struct CompileTimeCheck<true> { static void print() { cout << "true value" << endl; } };
    template <> struct CompileTimeCheck<false> { static void print() { cout << "false value" << endl; } };

    void test() {
        cout << boolalpha;
        cout << hasStream<WithoutStream>::value << endl;
        cout << hasStream<int>::value << endl;
        cout << hasStream<double>::value << endl;
        cout << hasStream<WithStream>::value << endl;

        CompileTimeCheck<hasStream<WithoutStream>::value>::print();
        CompileTimeCheck<hasStream<WithStream>::value>::print();
    }
}

namespace t34 {
    namespace HasStream {
        typedef char No[sizeof(ostringstream&) + 1];
        template<typename T> No& operator<< (ostream& os, const T&);

        template <class T> struct StreamExists {
            enum {                      // neded to make 'value' a compile time constant
                value = sizeof(*(ostringstream*)(0) << *(T*)(0)) != sizeof(No&)
            };
        };
    };

    template <class T> struct hasStream {
        static const bool value = HasStream::StreamExists<T>::value;
    };

    struct WithoutStream { };
    struct WithStream { };
    ostream& operator<<(ostream& os, const WithStream& ws) { os << "WithStream::operator<<" << endl; return os; }

    template <class T> string toString(T& t) { ostringstream os; os << t; return os.str(); }

    template <class T, bool /*has Stream*/> struct ToString;
    template <class T> struct ToString<T, true> { static string value(T& t) { return toString(t); } };
    template <class T> struct ToString<T, false> { static string value(T& t) { return "<unknown>"; } };

    template <class T> struct TypeInfo {
        TypeInfo() { }

        inline static bool hasStream() { return hasStream<T>::value; }

        static string toString(T& t) {
            static const bool hasStream = t34::hasStream<T>::value;
            return ToString<T, hasStream>::value(t);
        }
    };

    void test() {
        int i = 42;
        TypeInfo<int> ti;
        cout << ti.toString(i) << endl;

        WithoutStream wos;
        TypeInfo<WithoutStream> twos;
        cout << twos.toString(wos) << endl;

        WithStream ws;
        TypeInfo<WithStream> tws;
        cout << tws.toString(ws) << endl;
    }
}

namespace t35 {
    namespace HasStream {
        typedef char No[sizeof(ostringstream&) + 1];
        template<typename T> No& operator<< (ostream& os, const T&);

        template <class T> struct StreamExists {
            enum {                      // neded to make 'value' a compile time constant
                value = sizeof(*(ostringstream*)(0) << *(T*)(0)) != sizeof(No&)
            };
        };
    };

    template <class T> struct hasStream {
        static const bool value = HasStream::StreamExists<T>::value;
    };

    struct WithoutStream { };
    struct WithStream { };
    ostream& operator<<(ostream& os, const WithStream& ws) { os << "WithStream::operator <<"; return os; }

    template <class T> string toString(T& t) { ostringstream os; os << t; return os.str(); }

    template <class T, bool /*has Stream*/> struct ToString;
    template <class T> struct ToString<T, true> { static string value(T& t) { return toString(t); } };
    template <class T> struct ToString<T, false> { static string value(T& t) { return "<error>"; } };

    struct ObjectInfo {
        virtual bool hasStream() = 0;

        virtual string toString() = 0;
    };

    template <class T> struct ConcreteObjectInfo : public ObjectInfo {
        T& _ref;

        ConcreteObjectInfo(T& ref) : _ref(ref) { }

        virtual bool hasStream() { return t35::hasStream<T>::value; }

        string toString() {
            static const bool hasStream = t35::hasStream<T>::value;
            return ToString<T, hasStream>::value(_ref);
        }
    };

    void test() {
        int i = 42;
        float f = 3.1415f;
        WithoutStream wos;
        WithStream ws;
        vector<ObjectInfo*> voi;
        voi.push_back(new ConcreteObjectInfo<WithoutStream>(wos));
        voi.push_back(new ConcreteObjectInfo<WithStream>(ws));
        voi.push_back(new ConcreteObjectInfo<int>(i));
        voi.push_back(new ConcreteObjectInfo<float>(f));

        for (size_t i = 0; i < voi.size(); i++) {
            if (voi[i]->hasStream())
                cout << voi[i]->toString() << endl;
            else
                cout << "operator << not implemented" << endl;
        }
    }
}

namespace my {
    template <class T> T& last(vector<T>& v) {
        return v[v.size() - 1];
    }

    template <class T> void deleteAll(vector<T*>& v) {
        for (size_t i = 0; i < v.size(); i++)
            delete v[i];
    }

    template <class T, class UnaryPredicate> typename T::iterator& find_if(T& t, UnaryPredicate pred) {
        return std::find_if(t.begin(), t.end(), pred);
    }
}

namespace t36 {
    struct Hero {
        string name = "The Hero";
        int health = 42;
    };

    struct PropertyInfo { 
        const string _name;

        PropertyInfo(const string& name) : _name(name) { }
    };

    template <class C, class P> struct ConcretePropertyInfo : public PropertyInfo {
        P C::* _prop;
        
        ConcretePropertyInfo(const string& name, P C::* prop) : PropertyInfo(name), _prop(prop) { }
    };

    size_t LastTypeId = 0;

    template <class T> size_t getTypeId() {
        static size_t typeId = ++LastTypeId;
        return typeId;
    }

    struct ClassInfo { 
        size_t _typeId;
        const string _name;
    
        ClassInfo(size_t typeId, const string& name) : _typeId(typeId), _name(name) { }
    };

    struct ReflectionInfo;

    template <class C> struct ConcreteClassInfo : public ClassInfo {
        ReflectionInfo& _reflectionInfo;
        vector<PropertyInfo*> _propertyInfos;

        ConcreteClassInfo(const string& name, ReflectionInfo& reflectionInfo) : 
            _reflectionInfo(reflectionInfo), ClassInfo(getTypeId<C>(), name) { }

        template <class P> ConcreteClassInfo<C>& addProperty(const string& name, P C::* prop) {
            ConcretePropertyInfo<C, P>* propInfo = new ConcretePropertyInfo<C, P>(name, prop);
            _propertyInfos.push_back(propInfo);
            return *this;
        }

        ReflectionInfo& endClass() { return _reflectionInfo; }
    };

    template <class T> struct ClassObjectInfo { 
        ConcreteClassInfo<T>& _classInfo;

        ClassObjectInfo(ConcreteClassInfo<T>& classInfo) : _classInfo(classInfo) { }
    };

    template <class T> T& empty() {
        return *((T*)(NULL));
    }

    struct ReflectionInfo {
        vector<ClassInfo*> _classInfos;
        
        virtual ~ReflectionInfo() { my::deleteAll(_classInfos); }

        template <class C> ConcreteClassInfo<C>& beginClass(const string& name) {
            ConcreteClassInfo<C>* classInfo = new ConcreteClassInfo<C>(name, *this);
            _classInfos.push_back(classInfo);
            return *classInfo;
        }

        template <class T> ClassObjectInfo<T> getObjectInfo(T& t) {
            for (size_t i = 0; i < _classInfos.size(); i++) {
                if (_classInfos[i]->_typeId == getTypeId<T>()) {
                    ConcreteClassInfo<T>* concreteClassInfoPointer = (ConcreteClassInfo<T>*)_classInfos[i];
                    ConcreteClassInfo<T> concreteClassInfo = *concreteClassInfoPointer;
                    return ClassObjectInfo<T>(concreteClassInfo);
                }
            }

            cout << "type is not registered" << endl;
            return empty<ClassObjectInfo<T>>();
        }
    };

    // void print(ObjectInfo& object) {
    //     if (object.hasClassType()) {
    //         ClassObjectInfo& classObject = object.asClassObjectInfo();
    //         for(size_t i = 0; i < classObject.countProperties(); i++)
    //             ObjectInfo& property = classObject.getProperty(i);
    //             print(property);
    //     }
    // 
    //     else if (objectInfo.hasBasicType()) 
    //         cout << objectInfo.getName() << ": " << objectInfo.toString() << endl;
    // }

    void test() {
        ReflectionInfo reflectionInfo;
        reflectionInfo.beginClass<Hero>("Hero")
            .addProperty("name", &Hero::name)
            .addProperty("health", &Hero::health)
        .endClass();

        Hero hero;
        // ObjectInfo heroInfo = reflectionInfo.getObjectInfo(hero);
        // print(heroInfo);
    }
}

namespace t37 {
    class NoStream { };

    // https://people.eecs.berkeley.edu/~brock/blog/detection_idiom.php
    template <class T> struct HasOutStream {
        typedef char yes[1];
        typedef char no[2];

        template <class U> static yes& check(int(*)[sizeof(*(ostream*)(0) << *(U*)(0))]);
        template <class U> static no& check(...);

        static const bool value = sizeof(check<T>(0)) == sizeof(yes);
    };

    void test() {
        cout << HasOutStream<int>::value << endl;
        cout << HasOutStream<NoStream>::value << endl;
        cout << HasOutStream<complex<float>>::value << endl;
    }
}

namespace my {
    typedef char no[1];
    typedef char yes[2];

    template <class T> struct IsClass {
        template <class U> static yes& check(int U::*) { }
        template <class U> static no& check(...) { }
        static const bool value = sizeof(check<T>(0)) == sizeof(yes);
    };

    template <class T> string toString(T& t) {
        ostringstream os; os << t;
        return os.str();
    }

    template <class T> void fromString(T& t, const string& str) { istringstream is(str); is >> t; }

    // https://people.eecs.berkeley.edu/~brock/blog/detection_idiom.php
    template <class T> struct HasOutStream {
        typedef char yes[1];
        typedef char no[2];

        template <class U> static yes& check(int(*)[sizeof(*(ostream*)(0) << *(U*)(0))]);
        template <class U> static no& check(...);

        static const bool value = sizeof(check<T>(0)) == sizeof(yes);
    };

    template <class T> struct HasInStream {
        typedef char yes[1];
        typedef char no[2];

        template <class U> static yes& check(int(*)[sizeof(*(istream*)(0) >> *(U*)(0))]);
        template <class U> static no& check(...);

        static const bool value = sizeof(check<T>(0)) == sizeof(yes);
    };
}

namespace t38 {
    struct Inspector;
    struct Object;
    template <class T> struct ConcreteObject;

    struct Property {
        string _name;
        Property(const string& name) : _name(name) { }
    };

    template <class C> struct ClassProperty : public Property { 
        ClassProperty(const string& name) : Property(name) { }
        virtual Object* getPropertyObject(C& parent, Inspector& inspector) = 0;
    };

    template <class C, class P> struct ConcreteProperty : public ClassProperty<C> {
        P C::* _member;
        ConcreteProperty(const string& name, P C::* member) : ClassProperty<C>(name), _member(member) { }
        virtual Object* getPropertyObject(C& parent, Inspector& inspector) {
            P* propertyObject = &(parent.*_member);
            return new ConcreteObject<P>(*propertyObject, Property::_name, inspector);
        }
    };

    static size_t TypeIdCounter = 0;
    template <class T> size_t getTypeId() {
        static size_t typeId = ++TypeIdCounter;
        return typeId;
    }

    struct Class {
        string _name;
        Class(const string& name) : _name(name) { }
        virtual size_t countProperties() = 0;
        virtual size_t getTypeId() = 0;
    };

    template <class C> struct ConcreteClass : public Class {
        vector<ClassProperty<C>*> _properties;
        ConcreteClass(const string& name) : Class(name) { }
        template <class P> void addProperty(const string& name, P C::* member) { _properties.push_back(new ConcreteProperty<C, P>(name, member)); }
        virtual size_t countProperties() { return _properties.size(); }
        virtual size_t getTypeId() { return t38::getTypeId<C>(); }
    };

    template <class C> struct ClassBuilder {
        Inspector& _inspector;
        ConcreteClass<C> _class;
        ClassBuilder(const string& name, Inspector& inspector) : _class(name), _inspector(inspector) { }
        template <class P> ClassBuilder& addProperty(const string& name, P C::* member) { _class.addProperty<P>(name, member); return *this; }
        Inspector& endClass() { _inspector.addClass<C>(_class); return _inspector; }
    };
 
    template <bool> struct Identity { };                                                        // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
    
    struct Inspector {
        vector<Class*> _classes;
        template <class C> ClassBuilder<C> beginClass(const string &name) { return ClassBuilder<C>(name, *this); }
        template <class C> void addClass(ConcreteClass<C>& theClass) { _classes.push_back(new ConcreteClass<C>(theClass)); }
        template <class T> Object* getObject(T& t, const string& name) { return new ConcreteObject<T>(t, name, *this); }
        template <class T> bool hasClass() { return getClass<T>() != NULL; }
        template <class T> ConcreteClass<T>* getClass() { return getClass<T>(Identity<my::IsClass<T>::value>()); }           // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
        template <class T> ConcreteClass<T>* getClass(Identity<true> isClass) {
            for (size_t i = 0; i < _classes.size(); i++)
                if (_classes[i]->getTypeId() == getTypeId<T>())
                    return (ConcreteClass<T>*)_classes[i];
            return NULL;
        }
        template <class T> ConcreteClass<T>* getClass(Identity<false> isClass) { return NULL; }
    };

    struct Object {
        string _name;
        Object(const string& name) : _name(name) { }
        const string& getName() { return _name; };
        virtual bool hasClassType() = 0;
        virtual const string getTypename() = 0;
        virtual size_t countProperties() = 0;
        virtual Object* getProperty(size_t index) = 0;
        virtual string toString() = 0;
    };

    template <class T> struct ConcreteObject : public Object {
        static const bool IsClass = my::IsClass<T>::value;
        static const bool HasOutStream = my::HasOutStream<T>::value;
        T& _ref;
        Inspector& _inspector;
        ConcreteObject(T& t, const string& name, Inspector& inspector) : Object(name), _ref(t), _inspector(inspector) { }
        virtual bool hasClassType() { return hasClassType(Identity<IsClass>()); }
        bool hasClassType(Identity<true> isClass) { return _inspector.getClass<T>(); }
        bool hasClassType(Identity<false> isclass) { return false; }
        virtual const string getTypename() { return getTypename(Identity<IsClass>()); }   // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
        const string getTypename(Identity<true> isClass) { return _inspector.hasClass<T>() ? _inspector.getClass<T>()->_name : "unknown or builtin type"; }
        const string getTypename(Identity<false> isClass) { return "builtin type"; }
        virtual size_t countProperties() { return countProperties(Identity<IsClass>()); }
        size_t countProperties(Identity<true> isClass) { return _inspector.hasClass<T>() ? _inspector.getClass<T>()->countProperties() : 0; }
        size_t countProperties(Identity<false> isClass) { return 0; }
        virtual Object* getProperty(size_t index) { return getProperty(index, Identity<IsClass>()); }
        Object* getProperty(size_t index, Identity<true> isClass) { 
            return _inspector.hasClass<T>() ? _inspector.getClass<T>()->_properties[index]->getPropertyObject(_ref, _inspector) : NULL; 
        }
        Object* getProperty(size_t index, Identity<false> isClass) { return NULL; }
        virtual string toString() { return toString(Identity<HasOutStream>()); }
        string toString(Identity<true> hasOutStream) { return my::toString(_ref); }
        string toString(Identity<false> hasOutStream) { return "operator << missing"; }
    };

    struct Vector2f { float x = 3, y = 4; };

    struct Hero { string name = "Chuck"; int health = 42; Vector2f position; };

    inline string indent(size_t depth) { return string(depth * 4, ' '); }

    void print(Object& object, size_t depth = 0) { 
        if (object.hasClassType()) {
            cout << indent(depth) << object.getName() << " (" << object.getTypename() <<  ")" << ": " << endl;
            for (size_t i = 0; i < object.countProperties(); i++)
                print(*object.getProperty(i), depth + 1);
        } 
        else 
            cout << indent(depth) << object.getName() << ": " << object.toString() << endl;
    }

    // Serialize object with two basic properties
    void test() {
        Inspector inspector;
        inspector.beginClass<Hero>("Hero")
            .addProperty("name", &Hero::name)
            .addProperty("health", &Hero::health)
            .addProperty("position", &Hero::position)
        .endClass();

        Hero hero;
        Object* heroObject = inspector.getObject<Hero>(hero, "hero");
        print(*heroObject);
    }
}

namespace t39 {
    struct Inspector;
    struct Object;
    template <class T> struct ConcreteObject;

    struct Property {
        string _name;
        Property(const string& name) : _name(name) { }
    };

    template <class C> struct ClassProperty : public Property {
        ClassProperty(const string& name) : Property(name) { }
        virtual Object* getPropertyObject(C& parent, Inspector& inspector) = 0;
    };

    template <class C, class P> struct ConcreteProperty : public ClassProperty<C> {
        P C::* _member;
        ConcreteProperty(const string& name, P C::* member) : ClassProperty<C>(name), _member(member) { }
        virtual Object* getPropertyObject(C& parent, Inspector& inspector) {
            P* propertyObject = &(parent.*_member);
            return new ConcreteObject<P>(*propertyObject, Property::_name, inspector);
        }
    };

    static size_t TypeIdCounter = 0;
    template <class T> size_t getTypeId() {
        static size_t typeId = ++TypeIdCounter;
        return typeId;
    }

    struct Class {
        string _name;
        Class(const string& name) : _name(name) { }
        virtual size_t countProperties() = 0;
        virtual size_t getTypeId() = 0;
    };

    template <class C> struct ConcreteClass : public Class {
        vector<ClassProperty<C>*> _properties;
        ConcreteClass(const string& name) : Class(name) { }
        template <class P> void addProperty(const string& name, P C::* member) { _properties.push_back(new ConcreteProperty<C, P>(name, member)); }
        virtual size_t countProperties() { return _properties.size(); }
        virtual size_t getTypeId() { return t39::getTypeId<C>(); }
    };

    template <class C> struct ClassBuilder {
        Inspector& _inspector;
        ConcreteClass<C> _class;
        ClassBuilder(const string& name, Inspector& inspector) : _class(name), _inspector(inspector) { }
        template <class P> ClassBuilder& addProperty(const string& name, P C::* member) { _class.addProperty<P>(name, member); return *this; }
        Inspector& endClass() { _inspector.addClass<C>(_class); return _inspector; }
    };

    template <bool> struct Identity { };                                                        // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope

    struct Inspector {
        vector<Class*> _classes;
        template <class C> ClassBuilder<C> beginClass(const string& name) { return ClassBuilder<C>(name, *this); }
        template <class C> void addClass(ConcreteClass<C>& theClass) { _classes.push_back(new ConcreteClass<C>(theClass)); }
        template <class T> Object* getObject(T& t, const string& name) { return new ConcreteObject<T>(t, name, *this); }
        template <class T> bool hasClass() { return getClass<T>() != NULL; }
        template <class T> ConcreteClass<T>* getClass() { return getClass<T>(Identity<my::IsClass<T>::value>()); }           // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
        template <class T> ConcreteClass<T>* getClass(Identity<true> isClass) {
            for (size_t i = 0; i < _classes.size(); i++)
                if (_classes[i]->getTypeId() == getTypeId<T>())
                    return (ConcreteClass<T>*)_classes[i];
            return NULL;
        }
        template <class T> ConcreteClass<T>* getClass(Identity<false> isClass) { return NULL; }
    };

    struct Object {
        string _name;
        Object(const string& name) : _name(name) { }
        const string& getName() { return _name; };
        virtual bool hasClassType() = 0;
        virtual const string getTypename() = 0;
        virtual size_t countProperties() = 0;
        virtual Object* getProperty(size_t index) = 0;
        virtual string toString() = 0;
    };

    template <class T> struct ConcreteObject : public Object {
        static const bool IsClass = my::IsClass<T>::value;
        static const bool HasOutStream = my::HasOutStream<T>::value;
        T& _ref;
        Inspector& _inspector;
        ConcreteObject(T& t, const string& name, Inspector& inspector) : Object(name), _ref(t), _inspector(inspector) { }
        virtual bool hasClassType() { return hasClassType(Identity<IsClass>()); }
        bool hasClassType(Identity<true> isClass) { return _inspector.getClass<T>(); }
        bool hasClassType(Identity<false> isclass) { return false; }
        virtual const string getTypename() { return getTypename(Identity<IsClass>()); }   // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
        const string getTypename(Identity<true> isClass) { return _inspector.hasClass<T>() ? _inspector.getClass<T>()->_name : "unknown or builtin type"; }
        const string getTypename(Identity<false> isClass) { return "builtin type"; }
        virtual size_t countProperties() { return countProperties(Identity<IsClass>()); }
        size_t countProperties(Identity<true> isClass) { return _inspector.hasClass<T>() ? _inspector.getClass<T>()->countProperties() : 0; }
        size_t countProperties(Identity<false> isClass) { return 0; }
        virtual Object* getProperty(size_t index) { return getProperty(index, Identity<IsClass>()); }
        Object* getProperty(size_t index, Identity<true> isClass) {
            return _inspector.hasClass<T>() ? _inspector.getClass<T>()->_properties[index]->getPropertyObject(_ref, _inspector) : NULL;
        }
        Object* getProperty(size_t index, Identity<false> isClass) { return NULL; }
        virtual string toString() { return toString(Identity<HasOutStream>()); }
        string toString(Identity<true> hasOutStream) { return my::toString(_ref); }
        string toString(Identity<false> hasOutStream) { return "operator << missing"; }
    };

    struct Hero { string name = "Chuck"; int health = 42; };
    
    inline string indent(size_t depth) { return string(depth * 4, ' '); }

    struct Writer : public Inspector {
        void write(Object& object, ostringstream& os, size_t depth) {
            if (object.hasClassType()) {
                os << indent(depth) << object.getName() << " (" << object.getTypename() << ")" << ": " << endl;
                for (size_t i = 0; i < object.countProperties(); i++)
                    write(*object.getProperty(i), os, depth + 1);
            }
            else
                os << indent(depth) << object.getName() << ": " << object.toString() << endl;
        }

        template <class T> void writeToString(string name, T& t, string& result) {
            ostringstream os;
            Object* object = this->getObject<T>(t, "hero");
            write(*object, os, 0);
            delete object;
            result = os.str();
        }
    };

    // Serialize object with one basic and one nested Property
    void test() {
        Writer writer;
        writer.beginClass<Hero>("Hero")
            .addProperty("name", &Hero::name)
            .addProperty("health", &Hero::health)
        .endClass();

        Hero hero;
        string result;
        writer.writeToString("hero", hero, result);
        
        cout << result << endl;
    }
}

namespace my {
    void split(const string& str, const string& delim, vector<string>& parts) {
        string s(str); size_t pos = 0; string token;
        while ((pos = s.find(delim)) != string::npos) {
            parts.push_back(s.substr(0, pos));
            s.erase(0, pos + delim.length());
        }

        if (s.length() > 0)
            parts.push_back(s);
    }

    // https://www.techiedelight.com/remove-whitespaces-string-cpp/
    const std::string whitespaces(" \t\f\v\n\r");

    void ltrim(const string& str, string& result) {
        string s(str);
        size_t pos = s.find_first_not_of(whitespaces);
        result = pos != string::npos ? s.substr(pos) : "";
    }

    void rtrim(const string& str, string& result) {
        string s(str);
        size_t pos = s.find_last_not_of(whitespaces);
        result = pos != string::npos ? s.substr(0, pos + 1) : "";
    }

    void trim(const string& str, string& result) {
        ltrim(str, result);
        rtrim(result, result);
    }

    void error(const string& message) {
        cout << "error: " << message << endl; 
        cin.get(); cin.get(); exit(0);
    }
}

namespace t40 {
    void test() {
        string str = " this\n  is a \n   test test ";
        vector<string> parts;
        my::split(str, "\n", parts);
        for (size_t i = 0; i < parts.size(); i++)
            cout << parts[i] << endl;
    }
}

namespace t41 {
    void test() {
        string str = "  abc";
        string trimmed;
        my::ltrim(str, trimmed);
        cout << str << endl;
        cout << trimmed << endl;

        string str2 = "abc  ";
        string trimmed2;
        my::rtrim(str2, trimmed2);
        cout << str2 << "x" << endl;
        cout << trimmed2 << "x" << endl;

        string str3 = "  abc  ";
        string trimmed3;
        my::trim(str3, trimmed3);
        cout << str3 << "x" << endl;
        cout << trimmed3 << "x" << endl;
    }
}

namespace t42 {
    struct Inspector;
    struct Object;
    template <class T> struct ConcreteObject;

    struct Property {
        string _name;
        Property(const string& name) : _name(name) { }
    };

    template <class C> struct ClassProperty : public Property {
        ClassProperty(const string& name) : Property(name) { }
        virtual Object* getPropertyObject(C& parent, Inspector& inspector) = 0;
    };

    template <class C, class P> struct ConcreteProperty : public ClassProperty<C> {
        P C::* _member;
        ConcreteProperty(const string& name, P C::* member) : ClassProperty<C>(name), _member(member) { }
        virtual Object* getPropertyObject(C& parent, Inspector& inspector) {
            P* propertyObject = &(parent.*_member);
            return new ConcreteObject<P>(*propertyObject, Property::_name, inspector);
        }
    };

    static size_t TypeIdCounter = 0;
    template <class T> size_t getTypeId() {
        static size_t typeId = ++TypeIdCounter;
        return typeId;
    }

    struct Class {
        string _name;
        Class(const string& name) : _name(name) { }
        virtual size_t countProperties() = 0;
        virtual size_t getTypeId() = 0;
    };

    template <class C> struct ConcreteClass : public Class {
        vector<ClassProperty<C>*> _properties;
        ConcreteClass(const string& name) : Class(name) { }
        template <class P> void addProperty(const string& name, P C::* member) { _properties.push_back(new ConcreteProperty<C, P>(name, member)); }
        virtual size_t countProperties() { return _properties.size(); }
        virtual size_t getTypeId() { return t42::getTypeId<C>(); }
    };

    template <class C> struct ClassBuilder {
        Inspector& _inspector;
        ConcreteClass<C> _class;
        ClassBuilder(const string& name, Inspector& inspector) : _class(name), _inspector(inspector) { }
        template <class P> ClassBuilder& addProperty(const string& name, P C::* member) { _class.addProperty<P>(name, member); return *this; }
        Inspector& endClass() { _inspector.addClass<C>(_class); return _inspector; }
    };

    template <bool> struct Identity { };                                                        // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope

    struct Inspector {
        vector<Class*> _classes;
        template <class C> ClassBuilder<C> beginClass(const string& name) { return ClassBuilder<C>(name, *this); }
        template <class C> void addClass(ConcreteClass<C>& theClass) { _classes.push_back(new ConcreteClass<C>(theClass)); }
        template <class T> Object* getObject(T& t, const string& name) { return new ConcreteObject<T>(t, name, *this); }
        template <class T> bool hasClass() { return getClass<T>() != NULL; }
        template <class T> ConcreteClass<T>* getClass() { return getClass<T>(Identity<my::IsClass<T>::value>()); }           // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
        template <class T> ConcreteClass<T>* getClass(Identity<true> isClass) {
            for (size_t i = 0; i < _classes.size(); i++)
                if (_classes[i]->getTypeId() == getTypeId<T>())
                    return (ConcreteClass<T>*)_classes[i];
            return NULL;
        }
        template <class T> ConcreteClass<T>* getClass(Identity<false> isClass) { return NULL; }
    };

    struct Object {
        string _name;
        Object(const string& name) : _name(name) { }
        const string& getName() { return _name; };
        virtual bool hasClassType() = 0;
        virtual const string getTypename() = 0;
        virtual size_t countProperties() = 0;
        virtual Object* getProperty(size_t index) = 0;
        virtual Object* getProperty(const string& name) = 0;
        virtual string toString() = 0;
        virtual void fromString(const string& str) = 0;
    };

    template <class T> struct ConcreteObject : public Object {
        static const bool IsClass = my::IsClass<T>::value;
        static const bool HasOutStream = my::HasOutStream<T>::value;
        static const bool HasInStream = my::HasInStream<T>::value;
        T& _ref;
        Inspector& _inspector;
        ConcreteObject(T& t, const string& name, Inspector& inspector) : Object(name), _ref(t), _inspector(inspector) { }
        virtual bool hasClassType() { return hasClassType(Identity<IsClass>()); }
        bool hasClassType(Identity<true> isClass) { return _inspector.getClass<T>(); }
        bool hasClassType(Identity<false> isclass) { return false; }
        virtual const string getTypename() { return getTypename(Identity<IsClass>()); }   // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
        const string getTypename(Identity<true> isClass) { return _inspector.hasClass<T>() ? _inspector.getClass<T>()->_name : "unknown or builtin type"; }
        const string getTypename(Identity<false> isClass) { return "builtin type"; }
        virtual size_t countProperties() { return countProperties(Identity<IsClass>()); }
        size_t countProperties(Identity<true> isClass) { return _inspector.hasClass<T>() ? getProperties().size() : 0; }
        size_t countProperties(Identity<false> isClass) { return 0; }
        virtual Object* getProperty(size_t index) { return getProperty(index, Identity<IsClass>()); }
        Object* getProperty(size_t index, Identity<true> isClass) {
            return _inspector.hasClass<T>() ? getProperties()[index]->getPropertyObject(_ref, _inspector) : NULL;
        }
        Object* getProperty(size_t index, Identity<false> isClass) { return NULL; }
        virtual Object* getProperty(const string& name) { return getProperty(name, Identity<IsClass>()); }
        Object* getProperty(const string& name, Identity<true> isClass) {
            if (!_inspector.hasClass<T>()) return NULL;
            for (size_t i = 0; i < getProperties().size(); i++)
                if (getProperties()[i]->_name == name) return getProperties()[i]->getPropertyObject(_ref, _inspector);
            return NULL;
        }
        Object* getProperty(const string& name, Identity<false> isClass) { return NULL; }
        virtual string toString() { return toString(Identity<HasOutStream>()); }
        string toString(Identity<true> hasOutStream) { return my::toString(_ref); }
        string toString(Identity<false> hasOutStream) { return "operator << missing"; }
        virtual void fromString(const string& str) { fromString(str, Identity<HasInStream>()); }
        void fromString(const string& str, Identity<true> hasInStream) { my::fromString(_ref, str); }
        void fromString(const string& str, Identity<false> hasInStream) { my::error( "operator >> missing"); }
        vector<ClassProperty<T>*> getProperties() { return _inspector.getClass<T>()->_properties; }
    };

    inline string indent(size_t depth) { return string(depth * 4, ' '); }

    struct Writer : public Inspector {
        void write(Object& object, ostringstream& os, size_t depth = 0) {
            if (object.hasClassType()) {
                os << indent(depth) << object.getName() << ": " << endl;
                for (size_t i = 0; i < object.countProperties(); i++)
                    write(*object.getProperty(i), os, depth + 1);
            }
            else
                os << indent(depth) << object.getName() << ": " << object.toString() << endl;
        }

        template <class T> void write(string name, T& t, string& result) {
            ostringstream os;
            Object* object = this->getObject<T>(t, name);
            write(*object, os, 0);
            delete object;
            result = os.str();
        }
    };

    void sanitize(const vector<string>& parts, vector<string>& result) {
        for (size_t i = 0; i < parts.size(); i++) {
            string trimmed;
            my::trim(parts[i], trimmed);
            if (!trimmed.empty())
                result.push_back(trimmed);
        }
    }

    struct InputStream {
        istringstream _is;
        string _peekLine;
        InputStream(const string& str) : _is(str) { }
        bool hasContent() { return !_is.eof() || !_peekLine.empty(); }
        void peekLine(string& result) { std::getline(_is, _peekLine); result = _peekLine; }
        void readLine(string& result) { 
            if (_peekLine.empty()) std::getline(_is, result);
            else result = _peekLine;
        }
    };
    
    struct Reader : public Inspector {
        void splitLine(const string& line, vector<string>& result) {
            vector<string> parts;  my::split(line, ":", parts);
            sanitize(parts, result);
        }
        bool isClassObject(vector<string>& parts) { return parts.size() == 1; }
        size_t getIndent(const string& line) { return line.find_first_not_of(my::whitespaces) / 4; }
        void read(Object& object, const string& line, InputStream& stream) {    // https://cpppatterns.com/patterns/read-line-by-line.html
            vector<string> parts; splitLine(line, parts);
            if (isClassObject(parts)) {
                while (stream.hasContent()) {
                    size_t propertyIndent = getIndent(line) + 1;
                    string nextLine; stream.peekLine(nextLine);
                    if (getIndent(nextLine) != propertyIndent) return;
                    string currentLine; stream.readLine(currentLine);
                    vector<string> currentParts; splitLine(currentLine, currentParts);
                    read(*object.getProperty(currentParts[0]), currentLine, stream);
                }
            } else {
                object.fromString(parts[1]);
            }
        }

        template <class T> void read(const string& name, const string& data, T& t) { 
            InputStream stream(data);
            Object* object = this->getObject<T>(t, name);
            string line; stream.readLine(line);
            read(*object, line, stream);
            delete object;
        }
    };

    struct Vector2f { float x = 1; float y = 2; };

    struct Hero { string name = "Chuck"; int health = 42; Vector2f position; };

    template <class Inspector> void registerTypes(Inspector& inspector) {
        inspector.beginClass<Hero>("Hero")
            .addProperty("name", &Hero::name)
            .addProperty("health", &Hero::health)
            .addProperty("position", &Hero::position)
        .endClass()
        .beginClass<Vector2f>("Vector2f")
            .addProperty("x", &Vector2f::x)
            .addProperty("y", &Vector2f::y)
        .endClass();
    }

    // Deserialize object with two basic properties and one nested Property
    void test() {        
        Writer writer; registerTypes(writer);
        Reader reader; registerTypes(reader);

        Hero hero; hero.name = "Mary"; hero.health = 9000; hero.position.x = 42; hero.position.y = 43;
        string result;
        writer.write("hero", hero, result);
        cout << result;

        Hero hero2;
        reader.read("hero", result, hero2);

        cout << hero2.health << endl;
        cout << hero2.name << endl;
        cout << hero2.position.x << " " << hero2.position.y << endl;
    }
}

namespace t43 {
    template <class I> struct Inspector;
    struct Object;
    template <class T, class I> struct ConcreteObject;

    template<class Inspector, class Object> struct HasOnInspectMethod {
        typedef char yes[1]; typedef char no[2];
        template<class Inspector2, class Object2, void(Inspector2::*)(ConcreteObject<Object2, Inspector2>&)> struct Sfinae {};
        template<class Inspector2, class Object2> static yes& check(Sfinae<Inspector2, Object2, &Inspector2::onInspect>*);
        template<class Inspector2, class Object2> static no& check(...);
        static const bool value = sizeof(check<Inspector, Object>(0)) == sizeof(yes);
    };

    struct Property {
        string _name;
        Property(const string& name) : _name(name) { }
    };

    template <class C, class I> struct ClassProperty : public Property {
        ClassProperty(const string& name) : Property(name) { }
        virtual Object* getPropertyObject(C& parent, I& inspector) = 0;
    };

    template <class C, class P, class I> struct ConcreteProperty : public ClassProperty<C, I> {
        P C::* _member;
        ConcreteProperty(const string& name, P C::* member) : ClassProperty<C, I>(name), _member(member) { }
        virtual Object* getPropertyObject(C& parent, I& inspector) {
            P* propertyObject = &(parent.*_member);
            return new ConcreteObject<P, I>(*propertyObject, Property::_name, inspector); }
    };

    static size_t TypeIdCounter = 0;
    template <class T> size_t getTypeId() {
        static size_t typeId = ++TypeIdCounter;
        return typeId;
    }

    struct Class {
        string _name;
        Class(const string& name) : _name(name) { }
        virtual size_t countProperties() = 0;
        virtual size_t getTypeId() = 0;
        virtual void inspect() = 0;
    };

    template <class C, class I> struct ConcreteClass : public Class {
        vector<ClassProperty<C, I>*> _properties;
        ConcreteClass(const string& name) : Class(name) { }
        template <class P> void addProperty(const string& name, P C::* member) { _properties.push_back(new ConcreteProperty<C, P, I>(name, member)); }
        virtual size_t countProperties() { return _properties.size(); }
        virtual size_t getTypeId() { return t43::getTypeId<C>(); }
        virtual void inspect() { }
    };

    template <class C, class I> struct ClassBuilder {
        I& _inspector;
        ConcreteClass<C, I> _class;
        ClassBuilder(const string& name, I& inspector) : _class(name), _inspector(inspector) { }
        template <class P> ClassBuilder& addProperty(const string& name, P C::* member) { _class.addProperty<P>(name, member); return *this; }
        I& endClass() { _inspector.addClass<C>(_class); return _inspector; }
    };

    template <bool> struct Identity { };                                                        // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope

    template <class I> struct Inspector {
        vector<Class*> _classes;
        template <class C> ClassBuilder<C, I> beginClass(const string& name) { return ClassBuilder<C, I>(name, (I&)(*this)); }
        template <class C> void addClass(ConcreteClass<C, I>& theClass) { _classes.push_back(new ConcreteClass<C, I>(theClass)); }
        template <class T> Object* getObject(T& t, const string& name, I& inspector) { return new ConcreteObject<T, I>(t, name, inspector); }
        template <class T> bool hasClass() { return getClass<T>() != NULL; }
        template <class C> ConcreteClass<C, I>* getClass() { return getClass<C>(Identity<my::IsClass<C>::value>()); }           // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
        template <class C> ConcreteClass<C, I>* getClass(Identity<true> isClass) {
            for (size_t i = 0; i < _classes.size(); i++)
                if (_classes[i]->getTypeId() == getTypeId<C>())
                    return (ConcreteClass<C, I>*)_classes[i];
            return NULL; }
        template <class C> ConcreteClass<C, I>* getClass(Identity<false> isClass) { return NULL; }
    };

    struct Object {
        string _name;
        Object(const string& name) : _name(name) { }
        const string& getName() { return _name; };
        virtual bool hasClassType() = 0;
        virtual const string getTypename() = 0;
        virtual size_t countProperties() = 0;
        virtual Object* getProperty(size_t index) = 0;
        virtual Object* getProperty(const string& name) = 0;
        virtual string toString() = 0;
        virtual void fromString(const string& str) = 0;
        virtual void inspect() = 0;
    };

    template <class T, class I> struct ConcreteObject : public Object {
        static const bool IsClass = my::IsClass<T>::value;
        static const bool HasOutStream = my::HasOutStream<T>::value;
        static const bool HasInStream = my::HasInStream<T>::value;
        static const bool HasOnInspect = HasOnInspectMethod<I, T>::value;
        T& _ref;
        I& _inspector;
        ConcreteObject(T& t, const string& name, I& inspector) : Object(name), _ref(t), _inspector(inspector) { }
        virtual bool hasClassType() { return hasClassType(Identity<IsClass>()); }
        bool hasClassType(Identity<true> isClass) { return _inspector.getClass<T>(); }
        bool hasClassType(Identity<false> isclass) { return false; }
        virtual const string getTypename() { return getTypename(Identity<IsClass>()); }   // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
        const string getTypename(Identity<true> isClass) { return _inspector.hasClass<T>() ? _inspector.getClass<T>()->_name : "unknown or builtin type"; }
        const string getTypename(Identity<false> isClass) { return "builtin type"; }
        virtual size_t countProperties() { return countProperties(Identity<IsClass>()); }
        size_t countProperties(Identity<true> isClass) { return _inspector.hasClass<T>() ? getProperties().size() : 0; }
        size_t countProperties(Identity<false> isClass) { return 0; }
        virtual Object* getProperty(size_t index) { return getProperty(index, Identity<IsClass>()); }
        Object* getProperty(size_t index, Identity<true> isClass) {
            return _inspector.hasClass<T>() ? getProperties()[index]->getPropertyObject(_ref, _inspector) : NULL; }
        Object* getProperty(size_t index, Identity<false> isClass) { return NULL; }
        virtual Object* getProperty(const string& name) { return getProperty(name, Identity<IsClass>()); }
        Object* getProperty(const string& name, Identity<true> isClass) {
            if (!_inspector.hasClass<T>()) return NULL;
            for (size_t i = 0; i < getProperties().size(); i++)
                if (getProperties()[i]->_name == name) return getProperties()[i]->getPropertyObject(_ref, _inspector);
            return NULL; }
        Object* getProperty(const string& name, Identity<false> isClass) { return NULL; }
        virtual string toString() { return toString(Identity<HasOutStream>()); }
        string toString(Identity<true> hasOutStream) { return my::toString(_ref); }
        string toString(Identity<false> hasOutStream) { return "operator << missing"; }
        virtual void fromString(const string& str) { fromString(str, Identity<HasInStream>()); }
        void fromString(const string& str, Identity<true> hasInStream) { my::fromString(_ref, str); }
        void fromString(const string& str, Identity<false> hasInStream) { my::error("operator >> missing"); }
        vector<ClassProperty<T, I>*> getProperties() { return _inspector.getClass<T>()->_properties; }
        virtual void inspect() { inspect(Identity<HasOnInspect>()); }
        void inspect(Identity<true> hasOnInspect) { _inspector.onInspect(*this); }
        void inspect(Identity<false> hasOnInspect) { my::error("onInspect() method missing"); }
    };

    inline string indent(size_t depth) { return string(depth * 4, ' '); }

    struct NonSerializable { };
    struct Hero { string name = "Chuck"; int health = 42; NonSerializable temp; };
    
    struct Editor : public Inspector<Editor> {
        template <class T> void onInspect(ConcreteObject<T, Editor>& obj) {
            cout << "Enter new value: "; 
            string input; cin >> input; cout << endl;
            obj.fromString(input);
        }

        template <class T> void edit(T& t, const string& name) {
            Object* object = getObject(t, name, *this);
            while (true) {
                cout << "Enter property name to edit (or 'done' to exit): ";
                string input; cin >> input; cout << endl;
                if (input == "done") return;
                Object* prop = object->getProperty(input);
                prop->inspect();
            }
        }
    };

    // Attach editor to object and edit values
    void test() { 
        Editor editor;
        editor.beginClass<Hero>("Hero")
            .addProperty("name", &Hero::name)
            .addProperty("health", &Hero::health)
            .addProperty("temp", &Hero::temp)
        .endClass();

        Hero hero;
        editor.edit(hero, "hero");

        cout << "Object dump after editing: " << endl;
        cout << hero.health << endl << hero.name << endl;
        cin.get();
    }
}

namespace t44 { 
    // https://www.youtube.com/watch?v=4l5HdmPoynw&ab_channel=javidx9
    void test() {
        string cmd = "a = 7 + blub";
        lua_State* L = luaL_newstate();
        int r = luaL_dostring(L, cmd.c_str());
        if (r == LUA_OK) {
            lua_getglobal(L, "a");
            if (lua_isnumber(L, -1)) {
                float a_in_cpp = (float)lua_tonumber(L, -1);
                cout << "the number: " << a_in_cpp << endl;
            }
        } else {
            string errormsg = lua_tostring(L, -1);
            cout << errormsg << endl;
        }

        lua_close(L);
    }
}

namespace t45 {
    void test() {
        string script = "testString = 'LuaBridge works!'\n"
                        "number = 42";
        lua_State* L = luaL_newstate();
        int result = luaL_dostring(L, script.c_str());
        if (result != LUA_OK) my::error(lua_tostring(L, -1));
        luaL_openlibs(L);
        lua_pcall(L, 0, 0, 0);
        LuaRef s = getGlobal(L, "testString");
        LuaRef n = getGlobal(L, "number");
        std::string luaString = s.cast<std::string>();
        int answer = n.cast<int>();
        std::cout << luaString << std::endl;
        std::cout << "And here's our number: " << answer << std::endl;
    }
}

namespace t46 {
    struct Test {
        void say(int x) { cout << "hello world " << x << endl; }
        void say2() { cout << "hello world 2" << endl; }
    };
    
    void test() {
        string script = "local test = Test()\n"
                        "test:say(42)"
                        "test:say2()"
                        "print 'Just a test'";
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        getGlobalNamespace(L)
            .beginClass<Test>("Test")
                .addConstructor<void(*) (void)>()
                .addFunction("say", &Test::say)
            .endClass()
            .beginClass<Test>("Test")
                .addFunction("say2", &Test::say2)
            .endClass();

        int result = luaL_dostring(L, script.c_str());
        if (result != LUA_OK) my::error(lua_tostring(L, -1));
    }
}

namespace t47 {
    struct Test {
        void say(int x) { cout << "hello world " << x << endl; }
        void say2() { cout << "hello world 2" << endl; }
    };

    template <class T> struct LuabridgeClass {
        T _luabridgeClass;
        LuabridgeClass(T luabridgeClass) : _luabridgeClass(luabridgeClass) { }
        template <class U> void addConstructor(U& u) { _luabridgeClass.addConstructor(u); }
        template <class U> void addProperty(const char* name, U& u) { _luabridgeClass.addProperty(name, u); }
        void endClass() { _luabridgeClass.endClass; }
    };

    void* luabridgeClass;
    template <class T> void storeLuabridgeClass(T t) { 
        luabridgeClass = malloc(sizeof(T));
        memcpy(luabridgeClass, &t, sizeof(T));
    }

    template <class T> LuabridgeClass<T> createLuabridgeClass(T t) {
        return LuabridgeClass<T>(t);
    }

    template <class T> void endClass() {
        //((Namespace::Class<T>*)luabridgeClass)->endClass();
    }

    // https://stackoverflow.com/questions/13532784/why-can-i-use-auto-on-a-private-type
    void test() {
        string script = "local test = Test()\n"
            "test:say(42)"
            "test:say2()"
            "print 'Just a test'";
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        Namespace ns = getGlobalNamespace(L);
        storeLuabridgeClass(ns.beginClass<Test>("Test"));
        //endClass<Test>();
        //LuabridgeClass<Namespace::Class<Test>> cls(ns.beginClass<Test>());
       /* auto cls = ns.beginClass<Test>("Test");
        cls.addConstructor<void(*) (void)>();
        cls.addFunction("say", &Test::say);
        cls.addFunction("say2", &Test::say2);
        cls.endClass();*/

        //int result = luaL_dostring(L, script.c_str());
        //if (result != LUA_OK) my::error(lua_tostring(L, -1));
    }
}

namespace t48 {
    template <class I> struct Inspector;
    struct Object;
    template <class T, class I> struct ConcreteObject;
    template <class C, class I> struct ConcreteClass;
    template <class C, class P, class I> struct ConcreteProperty;
    template <bool> struct Identity { };                                                        // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
    
    template<class Inspector, class Object> struct HasOnInspectMethod {
        typedef char yes[1]; typedef char no[2];
        template<class Inspector2, class Object2, void(Inspector2::*)(ConcreteObject<Object2, Inspector2>&)> struct Sfinae {};
        template<class Inspector2, class Object2> static yes& check(Sfinae<Inspector2, Object2, &Inspector2::onInspect>*);
        template<class Inspector2, class Object2> static no& check(...);
        static const bool value = sizeof(check<Inspector, Object>(0)) == sizeof(yes);
    };

    template<class Inspector, class Class> struct HasOnInspectClassMethod {
        typedef char yes[1]; typedef char no[2];
        template<class Inspector2, class Class2, void(Inspector2::*)(ConcreteClass<Class2, Inspector2>&)> struct Sfinae {};
        template<class Inspector2, class Class2> static yes& check(Sfinae<Inspector2, Class2, &Inspector2::onInspect>*);
        template<class Inspector2, class Class2> static no& check(...);
        static const bool value = sizeof(check<Inspector, Class>(0)) == sizeof(yes);
    };

    template<class Inspector, class Class, class Prop> struct HasOnInspectPropertyMethod {
        typedef char yes[1]; typedef char no[2];
        template<class Inspector2, class Class2, class Prop2, void(Inspector2::*)(ConcreteProperty<Class2, Prop2, Inspector2>&)> struct Sfinae {};
        template<class Inspector2, class Class2, class Prop2> static yes& check(Sfinae<Inspector2, Class2, Prop2, &Inspector2::onInspect>*);
        template<class Inspector2, class Class2, class Prop2> static no& check(...);
        static const bool value = sizeof(check<Inspector, Class, Prop>(0)) == sizeof(yes);
    };


    struct Property {
        string _name;
        Property(const string& name) : _name(name) { }
        virtual void inspect() = 0;
    };

    template <class C, class I> struct ClassProperty : public Property {
        ClassProperty(const string& name) : Property(name) { }
        virtual Object* getPropertyObject(C& parent, I& inspector) = 0;
    };

    template <class C, class P, class I> struct ConcreteProperty : public ClassProperty<C, I> {
        static const bool HasOnInspectProperty = HasOnInspectPropertyMethod<I, C, P>::value;
        P C::* _member;
        I& _inspector;
        ConcreteProperty(const string& name, P C::* member, I& inspector) : ClassProperty<C, I>(name), _member(member), _inspector(inspector) { }
        virtual Object* getPropertyObject(C& parent, I& inspector) {
            P* propertyObject = &(parent.*_member);
            return new ConcreteObject<P, I>(*propertyObject, Property::_name, inspector);
        }
        virtual void inspect() { inspect(Identity<HasOnInspectProperty>()); }
        void inspect(Identity<true> hasOnInspectProperty) { _inspector.onInspect(*this); }
        void inspect(Identity<false> hasOnInspectProperty) { my::error("onInspect() method for properties missing"); }
    };

    static size_t TypeIdCounter = 0;
    template <class T> size_t getTypeId() {
        static size_t typeId = ++TypeIdCounter;
        return typeId;
    }

    struct Class {
        string _name;
        Class(const string& name) : _name(name) { }
        virtual size_t countProperties() = 0;
        virtual size_t getTypeId() = 0;
        virtual void inspect() = 0;
    };

    template <class C, class I> struct ConcreteClass : public Class {
        static const bool HasOnInspectClass = HasOnInspectClassMethod<I, C>::value;
        I& _inspector;
        vector<ClassProperty<C, I>*> _properties;
        ConcreteClass(const string& name, I& inspector) : Class(name), _inspector(inspector) { }
        template <class P> void addProperty(const string& name, P C::* member) { _properties.push_back(new ConcreteProperty<C, P, I>(name, member, _inspector)); }
        virtual size_t countProperties() { return _properties.size(); }
        virtual size_t getTypeId() { return t48::getTypeId<C>(); }
        virtual void inspect() { inspect(Identity<HasOnInspectClass>()); }
        void inspect(Identity<true> hasOnInspectClass) { _inspector.onInspect(*this); }
        void inspect(Identity<false> hasOnInspectClass) { my::error("onInspect() method for classes missing"); }
    };

    template <class C, class I> struct ClassBuilder {
        I& _inspector;
        ConcreteClass<C, I> _class;
        ClassBuilder(const string& name, I& inspector) : _class(name, inspector), _inspector(inspector) { }
        template <class P> ClassBuilder& addProperty(const string& name, P C::* member) { _class.addProperty<P>(name, member); return *this; }
        I& endClass() { _inspector.addClass<C>(_class); return _inspector; }
    };


    template <class I> struct Inspector {
        vector<Class*> _classes;
        template <class C> ClassBuilder<C, I> beginClass(const string& name) { return ClassBuilder<C, I>(name, (I&)(*this)); }
        template <class C> void addClass(ConcreteClass<C, I>& theClass) { _classes.push_back(new ConcreteClass<C, I>(theClass)); }
        template <class T> Object* getObject(T& t, const string& name, I& inspector) { return new ConcreteObject<T, I>(t, name, inspector); }
        template <class T> bool hasClass() { return getClass<T>() != NULL; }
        template <class C> ConcreteClass<C, I>* getClass() { return getClass<C>(Identity<my::IsClass<C>::value>()); }           // https://stackoverflow.com/questions/3052579/explicit-specialization-in-n
        template <class C> ConcreteClass<C, I>* getClass(Identity<true> isClass) {
            for (size_t i = 0; i < _classes.size(); i++)
                if (_classes[i]->getTypeId() == getTypeId<C>())
                    return (ConcreteClass<C, I>*)_classes[i];
            return NULL;
        }
        template <class C> ConcreteClass<C, I>* getClass(Identity<false> isClass) { return NULL; }
    };

    struct Object {
        string _name;
        Object(const string& name) : _name(name) { }
        const string& getName() { return _name; };
        virtual bool hasClassType() = 0;
        virtual const string getTypename() = 0;
        virtual size_t countProperties() = 0;
        virtual Object* getProperty(size_t index) = 0;
        virtual Object* getProperty(const string& name) = 0;
        virtual string toString() = 0;
        virtual void fromString(const string& str) = 0;
        virtual void inspect() = 0;
    };

    template <class T, class I> struct ConcreteObject : public Object {
        static const bool IsClass = my::IsClass<T>::value;
        static const bool HasOutStream = my::HasOutStream<T>::value;
        static const bool HasInStream = my::HasInStream<T>::value;
        static const bool HasOnInspect = HasOnInspectMethod<I, T>::value;
        T& _ref;
        I& _inspector;
        ConcreteObject(T& t, const string& name, I& inspector) : Object(name), _ref(t), _inspector(inspector) { }
        virtual bool hasClassType() { return hasClassType(Identity<IsClass>()); }
        bool hasClassType(Identity<true> isClass) { return _inspector.getClass<T>(); }
        bool hasClassType(Identity<false> isclass) { return false; }
        virtual const string getTypename() { return getTypename(Identity<IsClass>()); }   // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
        const string getTypename(Identity<true> isClass) { return _inspector.hasClass<T>() ? _inspector.getClass<T>()->_name : "unknown or builtin type"; }
        const string getTypename(Identity<false> isClass) { return "builtin type"; }
        virtual size_t countProperties() { return countProperties(Identity<IsClass>()); }
        size_t countProperties(Identity<true> isClass) { return _inspector.hasClass<T>() ? getProperties().size() : 0; }
        size_t countProperties(Identity<false> isClass) { return 0; }
        virtual Object* getProperty(size_t index) { return getProperty(index, Identity<IsClass>()); }
        Object* getProperty(size_t index, Identity<true> isClass) {
            return _inspector.hasClass<T>() ? getProperties()[index]->getPropertyObject(_ref, _inspector) : NULL;
        }
        Object* getProperty(size_t index, Identity<false> isClass) { return NULL; }
        virtual Object* getProperty(const string& name) { return getProperty(name, Identity<IsClass>()); }
        Object* getProperty(const string& name, Identity<true> isClass) {
            if (!_inspector.hasClass<T>()) return NULL;
            for (size_t i = 0; i < getProperties().size(); i++)
                if (getProperties()[i]->_name == name) return getProperties()[i]->getPropertyObject(_ref, _inspector);
            return NULL;
        }
        Object* getProperty(const string& name, Identity<false> isClass) { return NULL; }
        virtual string toString() { return toString(Identity<HasOutStream>()); }
        string toString(Identity<true> hasOutStream) { return my::toString(_ref); }
        string toString(Identity<false> hasOutStream) { return "operator << missing"; }
        virtual void fromString(const string& str) { fromString(str, Identity<HasInStream>()); }
        void fromString(const string& str, Identity<true> hasInStream) { my::fromString(_ref, str); }
        void fromString(const string& str, Identity<false> hasInStream) { my::error("operator >> missing"); }
        vector<ClassProperty<T, I>*> getProperties() { return _inspector.getClass<T>()->_properties; }
        virtual void inspect() { inspect(Identity<HasOnInspect>()); }
        void inspect(Identity<true> hasOnInspect) { _inspector.onInspect(*this); }
        void inspect(Identity<false> hasOnInspect) { my::error("onInspect() method missing"); }
    };

    struct LuaScripting : public Inspector<LuaScripting> {
        lua_State* _lua = luaL_newstate();
        virtual ~LuaScripting() { lua_close(_lua); }
        string _className;
        LuaScripting() { luaL_openlibs(_lua); }
        template <class C, class P> void onInspect(ConcreteProperty<C, P, LuaScripting>& prop) {
            getGlobalNamespace(_lua).beginClass<C>(_className.c_str())
                .addProperty(prop._name.c_str(), prop._member)
            .endClass();
        }
        template <class C> void onInspect(ConcreteClass<C, LuaScripting>& theClass) {
            _className = theClass._name;
            getGlobalNamespace(_lua).beginClass<C>(_className.c_str())
                .addConstructor<void(*) (void)>()
            .endClass();
            for (size_t i = 0; i < theClass.countProperties(); i++)
                theClass._properties[i]->inspect();
        }
        void bind() {
            for (size_t i = 0; i < _classes.size(); i++)
                _classes[i]->inspect();
        }
        void runScript(const string& script) {
            bind();
            int result = luaL_dostring(_lua, script.c_str());
            if (result != LUA_OK) my::error(lua_tostring(_lua, -1));
        }
    };

    struct Person { string name = "Chuck"; int health = 42; };
 
    // Register reflected object in luabridge and use it in LUA script
    void test() {
        string script = "local person = Person()\n"
                        "print(person.name)\n"
                        "print(person.health)\n";
        LuaScripting scripting;
        scripting.beginClass<Person>("Person")
            .addProperty("name", &Person::name)
            .addProperty("health", &Person::health)
        .endClass();
        scripting.runScript(script);
    }
}

namespace t49 {
    struct TheClass {
        int i = 42;
    };

    template <class T> struct Store {
        T _t;
        Store(T& t) : _t(t) { }
        void addProperty() {
            _t.addProperty("i", &TheClass::i);
        }
        void addConstructor() {
            _t.addConstructor<void(*) (void)>();
        }
        void endClass() {
            _t.endClass();
        }
    };

    template <class T> void myFun(T bla) {
        Store<T> store(bla);
        store.addProperty();
        store.addConstructor();
        store.endClass();
    }

    void runScript(lua_State* lua) {
        string script = "local theClass = TheClass()\n"
            "print(theClass.i)\n";

        int result = luaL_dostring(lua, script.c_str());
        if (result != LUA_OK) my::error(lua_tostring(lua, -1));
    }

    void demo() {
        lua_State* lua = luaL_newstate();
        luaL_openlibs(lua);
        myFun(getGlobalNamespace(lua).beginClass<TheClass>("TheClass"));
        runScript(lua);
    }
}

namespace d1 {
    template <class I> struct Inspector;
    struct Object;
    template <class T, class I> struct ConcreteObject;
    template <class C, class I> struct ConcreteClass;
    template <class C, class P, class I> struct ConcreteProperty;
    template <bool> struct Identity { };                                                        // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope

    template<class Inspector, class Object> struct HasOnInspectMethod {
        typedef char yes[1]; typedef char no[2];
        template<class Inspector2, class Object2, void(Inspector2::*)(ConcreteObject<Object2, Inspector2>&)> struct Sfinae {};
        template<class Inspector2, class Object2> static yes& check(Sfinae<Inspector2, Object2, &Inspector2::onInspect>*);
        template<class Inspector2, class Object2> static no& check(...);
        static const bool value = sizeof(check<Inspector, Object>(0)) == sizeof(yes);
    };

    template<class Inspector, class Class> struct HasOnInspectClassMethod {
        typedef char yes[1]; typedef char no[2];
        template<class Inspector2, class Class2, void(Inspector2::*)(ConcreteClass<Class2, Inspector2>&)> struct Sfinae {};
        template<class Inspector2, class Class2> static yes& check(Sfinae<Inspector2, Class2, &Inspector2::onInspect>*);
        template<class Inspector2, class Class2> static no& check(...);
        static const bool value = sizeof(check<Inspector, Class>(0)) == sizeof(yes);
    };

    template<class Inspector, class Class, class Prop> struct HasOnInspectPropertyMethod {
        typedef char yes[1]; typedef char no[2];
        template<class Inspector2, class Class2, class Prop2, void(Inspector2::*)(ConcreteProperty<Class2, Prop2, Inspector2>&)> struct Sfinae {};
        template<class Inspector2, class Class2, class Prop2> static yes& check(Sfinae<Inspector2, Class2, Prop2, &Inspector2::onInspect>*);
        template<class Inspector2, class Class2, class Prop2> static no& check(...);
        static const bool value = sizeof(check<Inspector, Class, Prop>(0)) == sizeof(yes);
    };


    struct Property {
        string _name;
        Property(const string& name) : _name(name) { }
        virtual void inspect() = 0;
    };

    template <class C, class I> struct ClassProperty : public Property {
        ClassProperty(const string& name) : Property(name) { }
        virtual Object* getPropertyObject(C& parent, I& inspector) = 0;
    };

    template <class C, class P, class I> struct ConcreteProperty : public ClassProperty<C, I> {
        static const bool HasOnInspectProperty = HasOnInspectPropertyMethod<I, C, P>::value;
        P C::* _member;
        I& _inspector;
        ConcreteProperty(const string& name, P C::* member, I& inspector) : ClassProperty<C, I>(name), _member(member), _inspector(inspector) { }
        virtual Object* getPropertyObject(C& parent, I& inspector) {
            P* propertyObject = &(parent.*_member);
            return new ConcreteObject<P, I>(*propertyObject, Property::_name, inspector);
        }
        virtual void inspect() { inspect(Identity<HasOnInspectProperty>()); }
        void inspect(Identity<true> hasOnInspectProperty) { _inspector.onInspect(*this); }
        void inspect(Identity<false> hasOnInspectProperty) { my::error("onInspect() method for properties missing"); }
    };

    static size_t TypeIdCounter = 0;
    template <class T> size_t getTypeId() {
        static size_t typeId = ++TypeIdCounter;
        return typeId;
    }

    struct Class {
        string _name;
        Class(const string& name) : _name(name) { }
        virtual size_t countProperties() = 0;
        virtual size_t getTypeId() = 0;
        virtual void inspect() = 0;
    };

    template <class C, class I> struct ConcreteClass : public Class {
        static const bool HasOnInspectClass = HasOnInspectClassMethod<I, C>::value;
        I& _inspector;
        vector<ClassProperty<C, I>*> _properties;
        ConcreteClass(const string& name, I& inspector) : Class(name), _inspector(inspector) { }
        template <class P> void addProperty(const string& name, P C::* member) { _properties.push_back(new ConcreteProperty<C, P, I>(name, member, _inspector)); }
        virtual size_t countProperties() { return _properties.size(); }
        virtual size_t getTypeId() { return d1::getTypeId<C>(); }
        virtual void inspect() { inspect(Identity<HasOnInspectClass>()); }
        void inspect(Identity<true> hasOnInspectClass) { _inspector.onInspect(*this); }
        void inspect(Identity<false> hasOnInspectClass) { my::error("onInspect() method for classes missing"); }
    };

    template <class C, class I> struct ClassBuilder {
        I& _inspector;
        ConcreteClass<C, I> _class;
        ClassBuilder(const string& name, I& inspector) : _class(name, inspector), _inspector(inspector) { }
        template <class P> ClassBuilder& addProperty(const string& name, P C::* member) { _class.addProperty<P>(name, member); return *this; }
        I& endClass() { _inspector.addClass<C>(_class); return _inspector; }
    };


    template <class I> struct Inspector {
        vector<Class*> _classes;
        template <class C> ClassBuilder<C, I> beginClass(const string& name) { return ClassBuilder<C, I>(name, (I&)(*this)); }
        template <class C> void addClass(ConcreteClass<C, I>& theClass) { _classes.push_back(new ConcreteClass<C, I>(theClass)); }
        template <class T> Object* getObject(T& t, const string& name, I& inspector) { return new ConcreteObject<T, I>(t, name, inspector); }
        template <class T> bool hasClass() { return getClass<T>() != NULL; }
        template <class C> ConcreteClass<C, I>* getClass() { return getClass<C>(Identity<my::IsClass<C>::value>()); }           // https://stackoverflow.com/questions/3052579/explicit-specialization-in-n
        template <class C> ConcreteClass<C, I>* getClass(Identity<true> isClass) {
            for (size_t i = 0; i < _classes.size(); i++)
                if (_classes[i]->getTypeId() == getTypeId<C>())
                    return (ConcreteClass<C, I>*)_classes[i];
            return NULL;
        }
        template <class C> ConcreteClass<C, I>* getClass(Identity<false> isClass) { return NULL; }
    };

    struct Object {
        string _name;
        Object(const string& name) : _name(name) { }
        const string& getName() { return _name; };
        virtual bool hasClassType() = 0;
        virtual const string getTypename() = 0;
        virtual size_t countProperties() = 0;
        virtual Object* getProperty(size_t index) = 0;
        virtual Object* getProperty(const string& name) = 0;
        virtual string toString() = 0;
        virtual void fromString(const string& str) = 0;
        virtual void inspect() = 0;
    };

    template <class T, class I> struct ConcreteObject : public Object {
        static const bool IsClass = my::IsClass<T>::value;
        static const bool HasOutStream = my::HasOutStream<T>::value;
        static const bool HasInStream = my::HasInStream<T>::value;
        static const bool HasOnInspect = HasOnInspectMethod<I, T>::value;
        T& _ref;
        I& _inspector;
        ConcreteObject(T& t, const string& name, I& inspector) : Object(name), _ref(t), _inspector(inspector) { }
        virtual bool hasClassType() { return hasClassType(Identity<IsClass>()); }
        bool hasClassType(Identity<true> isClass) { return _inspector.getClass<T>(); }
        bool hasClassType(Identity<false> isclass) { return false; }
        virtual const string getTypename() { return getTypename(Identity<IsClass>()); }   // https://stackoverflow.com/questions/3052579/explicit-specialization-in-non-namespace-scope
        const string getTypename(Identity<true> isClass) { return _inspector.hasClass<T>() ? _inspector.getClass<T>()->_name : "unknown or builtin type"; }
        const string getTypename(Identity<false> isClass) { return "builtin type"; }
        virtual size_t countProperties() { return countProperties(Identity<IsClass>()); }
        size_t countProperties(Identity<true> isClass) { return _inspector.hasClass<T>() ? getProperties().size() : 0; }
        size_t countProperties(Identity<false> isClass) { return 0; }
        virtual Object* getProperty(size_t index) { return getProperty(index, Identity<IsClass>()); }
        Object* getProperty(size_t index, Identity<true> isClass) {
            return _inspector.hasClass<T>() ? getProperties()[index]->getPropertyObject(_ref, _inspector) : NULL;
        }
        Object* getProperty(size_t index, Identity<false> isClass) { return NULL; }
        virtual Object* getProperty(const string& name) { return getProperty(name, Identity<IsClass>()); }
        Object* getProperty(const string& name, Identity<true> isClass) {
            if (!_inspector.hasClass<T>()) return NULL;
            for (size_t i = 0; i < getProperties().size(); i++)
                if (getProperties()[i]->_name == name) return getProperties()[i]->getPropertyObject(_ref, _inspector);
            return NULL;
        }
        Object* getProperty(const string& name, Identity<false> isClass) { return NULL; }
        virtual string toString() { return toString(Identity<HasOutStream>()); }
        string toString(Identity<true> hasOutStream) { return my::toString(_ref); }
        string toString(Identity<false> hasOutStream) { return "operator << missing"; }
        virtual void fromString(const string& str) { fromString(str, Identity<HasInStream>()); }
        void fromString(const string& str, Identity<true> hasInStream) { my::fromString(_ref, str); }
        void fromString(const string& str, Identity<false> hasInStream) { my::error("operator >> missing"); }
        vector<ClassProperty<T, I>*> getProperties() { return _inspector.getClass<T>()->_properties; }
        virtual void inspect() { inspect(Identity<HasOnInspect>()); }
        void inspect(Identity<true> hasOnInspect) { _inspector.onInspect(*this); }
        void inspect(Identity<false> hasOnInspect) { my::error("onInspect() method missing"); }
    };
    
    inline string indent(size_t depth) { return string(depth * 4, ' '); }

    struct Writer : public Inspector<Writer> {
        void write(Object& object, ostringstream& os, size_t depth = 0) {
            if (object.hasClassType()) {
                os << indent(depth) << object.getName() << ": " << endl;
                for (size_t i = 0; i < object.countProperties(); i++)
                    write(*object.getProperty(i), os, depth + 1);
            }
            else
                os << indent(depth) << object.getName() << ": " << object.toString() << endl;
        }

        template <class T> void write(string name, T& t, string& result) {
            ostringstream os;
            Object* object = this->getObject<T>(t, name, *this);
            write(*object, os, 0);
            delete object;
            result = os.str();
        }
    };

    void sanitize(const vector<string>& parts, vector<string>& result) {
        for (size_t i = 0; i < parts.size(); i++) {
            string trimmed;
            my::trim(parts[i], trimmed);
            if (!trimmed.empty())
                result.push_back(trimmed);
        }
    }

    struct InputStream {
        istringstream _is;
        string _peekLine;
        InputStream(const string& str) : _is(str) { }
        bool hasContent() { return !_is.eof() || !_peekLine.empty(); }
        void peekLine(string& result) { std::getline(_is, _peekLine); result = _peekLine; }
        void readLine(string& result) {
            if (_peekLine.empty()) std::getline(_is, result);
            else result = _peekLine;
        }
    };

    struct Reader : public Inspector<Reader> {
        void splitLine(const string& line, vector<string>& result) {
            vector<string> parts;  my::split(line, ":", parts);
            sanitize(parts, result);
        }
        bool isClassObject(vector<string>& parts) { return parts.size() == 1; }
        size_t getIndent(const string& line) { return line.find_first_not_of(my::whitespaces) / 4; }
        void read(Object& object, const string& line, InputStream& stream) {    // https://cpppatterns.com/patterns/read-line-by-line.html
            vector<string> parts; splitLine(line, parts);
            if (isClassObject(parts)) {
                while (stream.hasContent()) {
                    size_t propertyIndent = getIndent(line) + 1;
                    string nextLine; stream.peekLine(nextLine);
                    if (getIndent(nextLine) != propertyIndent) return;
                    string currentLine; stream.readLine(currentLine);
                    vector<string> currentParts; splitLine(currentLine, currentParts);
                    read(*object.getProperty(currentParts[0]), currentLine, stream);
                }
            }
            else {
                object.fromString(parts[1]);
            }
        }

        template <class T> void read(const string& name, const string& data, T& t) {
            InputStream stream(data);
            Object* object = this->getObject<T>(t, name, *this);
            string line; stream.readLine(line);
            read(*object, line, stream);
            delete object;
        }
    };

    struct Editor : public Inspector<Editor> {
        template <class T> void onInspect(ConcreteObject<T, Editor>& obj) {
            cout << "Enter new value: ";
            string input; cin >> input; cout << endl;
            obj.fromString(input);
        }

        template <class T> void edit(T& t, const string& name) {
            Object* object = getObject(t, name, *this);
            while (true) {
                cout << "Enter property name to edit (or 'done' to exit): ";
                string input; cin >> input; cout << endl;
                if (input == "done") return;
                Object* prop = object->getProperty(input);
                prop->inspect();
            }
        }
    };

    struct Vector2f { float x = 1; float y = 2; };

    struct Hero { string name = "Chuck"; int health = 42; Vector2f position; };

    template <class Inspector> void registerTypes(Inspector& inspector) {
        inspector.beginClass<Hero>("Hero")
            .addProperty("name", &Hero::name)
            .addProperty("health", &Hero::health)
            .addProperty("position", &Hero::position)
        .endClass()
            .beginClass<Vector2f>("Vector2f")
            .addProperty("x", &Vector2f::x)
            .addProperty("y", &Vector2f::y)
        .endClass();
    }

    void dump(Hero& hero) {
        cout << "name: " << hero.name << endl;
        cout << "health: " << hero.health << endl;
        cout << "position.x: " << hero.position.x << " position.y: " << hero.position.y << endl;
    }

    void serializationDemo() {
        Writer writer; registerTypes(writer);
        Hero hero; hero.name = "Mary"; hero.health = 9000; hero.position.x = 42; hero.position.y = 43;
        string result;
        writer.write("hero", hero, result);
        cout << "INPUT:" << endl;
        dump(hero);
        cout << "OUTPUT: " << endl << result;
    }

    void deserializationDemo() {
        Reader reader; registerTypes(reader);
        const string input = "hero:\n"
                                    "    name: Tom\n" 
                                    "    health: 215\n"
                                    "    position:\n"
                                    "        x: 98\n"
                                    "        y: 91\n";
        Hero hero; 
        reader.read("hero", input, hero);
        cout << "INPUT:" << endl << input;
        cout << "OUTPUT: " << endl;
        dump(hero);
    }

    void editorDemo() {
        Editor editor; registerTypes(editor);
        Hero hero;
        cout << "INPUT:" << endl; dump(hero);
        editor.edit(hero, "hero");
        cout << "OUTPUT:" << endl; dump(hero);
    }

    void separator() {
        cout << "==============" << endl;
    }

    void demo() {
        cout << "DEMO: Serialize nested object" << endl; separator();
        serializationDemo();

        cout << "DEMO: Deserialize nested object" << endl; separator();
        deserializationDemo();

        cout << "DEMO: Edit content in object" << endl; separator();
        editorDemo();
        cin.get();
    }
}

void run()
{
    // Interesting: http://www.gotw.ca/gotw/076.htm
    // Also interesting: http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html
    // And: https://gist.github.com/dabrahams/1528856
    d1::demo();
    //t49::demo();
    //t48::test();
    //t47::test();
    //t46::test();
    //t45::test();
    //t44::test();
    //t43::test();
    //t42::test();
    //t41::test();
    //t40::test();
    //t39::test();
    //t38::test();
    //t37::test();
    //t36::test();
    //t35::test();
    //t34::test();
    //t33::test();
    //t32::test();
    //t31::test();
    //t30::test();
    //t29::test();
    //t28::test();
    //t27::test();
    //t26::test();
    //t25::test();
    //t24::test();
    //t23::test();
    //t22::test();
    //t21::test();
    //t20::test();
    //t19::test();
    //t18::test();
    //t17::test();
    //t16::test();
    //t15::test();
    //t14::test();
    //t13::test();
    //t12::test();
    //t11::test();
    //t10::test();
    //t9::test();
    //t8::test();
    //t7::test();
    //t6::test();
    //t5b::test();
    //t5a::test();
    //t5::test();
    //t4::test();
    //t3::test();
    //t2::test();
    //t1::test();
    //test1();
    //test2();
    //demo100();
}

int main() {
    run();
    cin.get(); 
}