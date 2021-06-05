#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <map>

// https://stackoverflow.com/questions/5047971/how-do-i-check-for-c11-support
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
    #define CPP_11
#endif

using namespace std;

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
        vector<float> v { 1, 2, 3 };
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

    template <class T> vector<CollectionInfo*> getCollection(T &t) { 
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

        template <class P> void addProperty(const string& name, P C::*prop) {
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
        vector<int> v{ 1, 2, 3};
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
    template <class T, ostringstream&(*)(T)> struct MyTest2 { };
    
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
        TypeInfo()  { }

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
    ostream& operator<<(ostream& os, const WithStream& ws) { os << "WithStream::operator<<"; return os; }

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
        vector<ObjectInfo*> oil;
        oil.push_back(new ConcreteObjectInfo<WithoutStream>(wos));
        oil.push_back(new ConcreteObjectInfo<int>(i));
        oil.push_back(new ConcreteObjectInfo<float>(f));
        oil.push_back(new ConcreteObjectInfo<WithStream>(ws));

        for (size_t i = 0; i < oil.size(); i++) {
            if (oil[i]->hasStream())
                cout << oil[i]->toString() << endl;
            else
                cout << "no toString()" << endl;
        }

    }
}

void test()
{
    t35::test();
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

// Notes:
//     - Object hierarchy
//         - EntityInfo
//             - ClassInfo
//                 - ConcreteClassInfo<T>
//             - FunctionInfo
//                 - ConcreteFunctionInfo<T>
//             - FieldInfo
//                 - ConcreteFieldInfo<T>


int main() {
    test();
#ifdef WIN32
    system("pause");
#endif
}
