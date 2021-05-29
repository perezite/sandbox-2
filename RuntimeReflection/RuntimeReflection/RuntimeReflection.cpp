#include <iostream>
#include <sstream>
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
    // https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time

    template <typename Class, typename, typename> struct test_struct { };

    template <class T> void getElements();
    template <> void getElements<int>() { }

    void test() {
        getElements<int>();
        // getElements<float>();           // linker error
    }
}

namespace t5b {
    template <typename T> struct IsContainer {
        static const bool value = false;
    };

    template <typename T, typename Alloc>
    struct IsContainer<std::vector<T, Alloc> > {
        static const bool value = true;
    };

    //template <typename T, typename U> struct isContainer2 {
    //    static const bool value = false;
    //};

    //template <typename T, typename U, typename Alloc>
    //struct IsContainer2<std::map<T, U, Alloc> > {
    //    static const bool value = true;
    //};

    void test() {
    }
}

void test() {
    t5b::test();
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
    cin.get();
}
