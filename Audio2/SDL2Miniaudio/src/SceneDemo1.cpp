#include "SceneDemo1.h"
#include "Drawable.h"
#include "Transformable.h"
#include "Sprite.h"
#include "DrawTarget.h"
#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "Memory.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;
using namespace sb;

namespace sceneDemo1
{
    struct Assets {
        Texture greenBlock;
        Texture yellowBlock;

        Assets()
            : greenBlock("Textures/GreenBlock.png"),
            yellowBlock("Textures/YellowBlock.png")

        { }
    };

    class MyEntity1 : public Drawable, public Transformable
    {
        Sprite _sprite;

    public:
        MyEntity1(Texture& texture)
            : _sprite(texture)
        { }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            states.transform *= getTransform();
            target.draw(_sprite, states);
        }
    };

    void demo1() {
        Window window;
        Assets assets;
        Sprite sprite(assets.greenBlock);
        MyEntity1 mySprite1(assets.greenBlock);

        window.setFramerateLimit(65);
        window.getCamera().setCenter(200);
        mySprite1.setScale(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(mySprite1);
            window.display();
        }
    }

    class Scene2 : public Drawable {
        vector<Drawable*> _drawables;
    protected:
        template <class T>
        inline T& addDrawable(T* entity) {
            _drawables.push_back(entity);
            return *entity;
        }
    public:
        virtual ~Scene2() {
            deleteAll(_drawables);
        }

        template <class T, class Arg1>
        inline T& createDrawable(Arg1& arg1) { return addDrawable<T>(new T(arg1)); }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            for (size_t i = 0; i < _drawables.size(); i++)
                target.draw(_drawables[i], states);
        }
    };

    void demo2() {
        Window window;
        Assets assets;
        Scene2 scene;
        MyEntity1& myEntity1 = scene.createDrawable<MyEntity1>(assets.greenBlock);

        window.setFramerateLimit(65);
        window.getCamera().setCenter(200);
        myEntity1.setScale(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }

    template <class T>
    class Entity3 : public T {
    public:
        Entity3() { }

        template <class Arg1>
        Entity3(Arg1& arg1) : T(arg1) { }

        template <class Arg1>
        Entity3(const Arg1& arg1) : T(arg1) { }

        virtual void update() {
            cout << "not implemented";
        }
    };

    class MyEntity3 : public Entity3<Sprite> {
    public:
        MyEntity3() { }

        MyEntity3(Texture& tex)
            : Entity3<Sprite>(tex)
        { }

        virtual void update() {
            rotate(0.01f);
        }
    };

    void demo3() {
        Window window;
        Assets assets;
        Entity3<Sprite> sprite(assets.greenBlock);
        MyEntity3 entity(assets.yellowBlock);

        window.setFramerateLimit(65);
        sprite.setScale(100);
        entity.setPosition(-100);
        entity.setScale(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            sprite.update();
            entity.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(sprite);
            window.draw(entity);
            window.display();
        }
    }

    class Scene4 : public Drawable {
        vector<Drawable*> _drawables;
    protected:
        template <class T>
        inline T& addDrawable(T* entity) {
            _drawables.push_back(entity);
            return *entity;
        }
    public:
        virtual ~Scene4() {
            deleteAll(_drawables);
        }

        template <class T, class Arg1>
        inline T& createDrawable(Arg1& arg1) { return addDrawable<T>(new T(arg1)); }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            for (size_t i = 0; i < _drawables.size(); i++)
                target.draw(_drawables[i], states);
        }
    };

    void demo4() {
        Window window;
        Assets assets;
        Scene2 scene;
        // scene.create<MyEntity3>(assets.yellowBlock)
        //MyEntity3 entity(assets.yellowBlock);

        window.setFramerateLimit(65);
        //entity.setPosition(-100);
        //entity.setScale(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            //entity.update();
            window.clear(Color(1, 1, 1, 1));
            //window.draw(entity);
            window.display();
        }
    }

    class Entity5 : public Drawable, public Transformable {
    public:
        virtual void drawSelf(DrawTarget& target, DrawStates states) = 0;

        virtual void draw(DrawTarget& target, DrawStates states) {
            states.transform *= getTransform();
            drawSelf(target, states);
        }

        virtual void update() { }
    };

    class SpriteEntity5 : public Entity5 {
        Sprite _sprite;
    public:
        SpriteEntity5(Texture& tex) : _sprite(tex) { }

        virtual void drawSelf(DrawTarget& target, DrawStates states) {
            target.draw(_sprite, states);
        }

        virtual void update() {
            rotate(0.01f);
        }
    };

    class Scene5 : public Drawable {
        vector<Entity5*> _entities;
    protected:
        template<class T>
        inline T& addEntity(T* entity) {
            _entities.push_back(entity);
            return *entity;
        }
    public:
        virtual ~Scene5() {
            deleteAll(_entities);
        }

        template <class T, class Arg1>
        inline T& create(Arg1& arg1) { return addEntity<T>(new T(arg1)); }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            for (size_t i = 0; i < _entities.size(); i++)
                target.draw(_entities[i], states);
        }

        void update() {
            for (size_t i = 0; i < _entities.size(); i++)
                _entities[i]->update();
        }
    };

    void demo5() {
        Window window;
        Assets assets;
        Scene5 scene;

        window.setFramerateLimit(65);
        SpriteEntity5& entity = scene.create<SpriteEntity5>(assets.greenBlock);
        entity.setScale(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            scene.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }

    // https://stackoverflow.com/questions/2631585/c-how-to-require-that-one-template-type-is-derived-from-the-other
    template <typename B, typename D>
    struct is_base_of6 // check if B is a base of D
    {
        typedef char yes[1];
        typedef char no[2];

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif
        static yes& test(B*) { }
        static no& test(...) { }
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

        static D* get(void) { };

        static const bool value = (sizeof(test(get())) == sizeof(yes));
    };

    template <bool> void test6();

    template <>
    void test6<true>() {
        cout << "true" << endl;
    }

    template <>
    void test6<false>() {
        cout << "false" << endl;
    }

    void demo6() {
        const bool test1 = is_base_of6<Entity5, SpriteEntity5>::value;
        test6<test1>();
        const bool test2 = is_base_of6<Entity5, Sprite>::value;
        test6<test2>();
        test6<is_base_of6<Entity5, Sprite>::value>();
        cin.get();
    }

    class Scene7 : public Drawable {
        vector<Entity5*> _entities;
        vector<Drawable*> _drawables;

    protected:
        template <class T, bool>
        struct elementAdder;

        template <class T>
        struct elementAdder<T, true> {
            inline T& add(T* entity, Scene7& scene) {
                scene._entities.push_back(entity);
                return *entity;
            }
        };

        template <class T>
        struct elementAdder<T, false> {
            inline T& add(T* drawable, Scene7& scene) {
                scene._drawables.push_back(drawable);
                return *drawable;
            }
        };

        template<class T>
        inline T& add(T* entity) {
            const bool isEntity = is_base_of6<Entity5, T>::value;
            return elementAdder<T, isEntity>().add(entity, *this);
        }

        template <class T>
        void draw(vector<T*> elements, DrawTarget& target, DrawStates states) {
            for (size_t i = 0; i < elements.size(); i++)
                target.draw(elements[i], states);
        }
    public:
        virtual ~Scene7() {
            deleteAll(_entities);
            deleteAll(_drawables);
        }

        template <class T, class Arg1>
        inline T& create(Arg1& arg1) { return add<T>(new T(arg1)); }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            draw(_entities, target, states);
            draw(_drawables, target, states);
        }

        void update() {
            for (size_t i = 0; i < _entities.size(); i++)
                _entities[i]->update();
        }
    };

    void demo7() {
        Window window;
        Assets assets;
        Scene7 scene;

        window.setFramerateLimit(65);
        SpriteEntity5& spriteEntity = scene.create<SpriteEntity5>(assets.greenBlock);
        Sprite& sprite = scene.create<Sprite>(assets.yellowBlock);
        spriteEntity.setScale(100);
        sprite.setScale(100);
        sprite.setPosition(-100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            scene.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }

    namespace impl {
        template <class T>
        class WrappedDrawable8 : public Entity5 {
            T* _drawable;
        public:
            virtual ~WrappedDrawable8() {
                delete _drawable;
            }

            WrappedDrawable8(T* drawable)
                : _drawable(drawable)
            { }

            virtual void drawSelf(DrawTarget& target, DrawStates states) { }

            virtual void draw(DrawTarget& target, DrawStates states) {
                _drawable->draw(target, states);
            }
        };

        template <class T, bool isEntity>
        struct Wrapper8;

        template <class T>
        struct Wrapper8<T, true> {
            static Entity5* wrap(T* element) { return element; }
        };

        template <class T>
        struct Wrapper8<T, false> {
            static Entity5* wrap(T* element) {
                Entity5* wrappedElem = new WrappedDrawable8<T>(element);
                return wrappedElem;
            }
        };
    }

    class Scene8 : public Drawable {
        vector<Entity5*> _entities;

    protected:
        template<class T>
        inline T& add(T* element) {
            const bool isEntity = is_base_of6<Entity5, T>::value;
            Entity5* wrappedElem = impl::Wrapper8<T, isEntity>::wrap(element);
            _entities.push_back(wrappedElem);

            return *element;
        }

    public:
        virtual ~Scene8() {
            deleteAll(_entities);
        }

        template <class T, class Arg1>
        inline T& create(Arg1& arg1) { return add<T>(new T(arg1)); }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            for (size_t i = 0; i < _entities.size(); i++)
                target.draw(_entities[i], states);
        }

        void update() {
            for (size_t i = 0; i < _entities.size(); i++)
                _entities[i]->update();
        }
    };

    void demo8() {
        Window window;
        Assets assets;
        Scene8 scene;

        window.setFramerateLimit(65);
        SpriteEntity5& spriteEntity = scene.create<SpriteEntity5>(assets.greenBlock);
        Sprite& sprite = scene.create<Sprite>(assets.yellowBlock);
        spriteEntity.setScale(100);
        sprite.setScale(100);
        sprite.setPosition(-100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            scene.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }

    class BaseEntity9 : public Drawable, public Transformable {
    public:
        virtual void drawSelf(DrawTarget& target, DrawStates states) = 0;

        virtual void draw(DrawTarget& target, DrawStates states) {
            states.transform *= getTransform();
            drawSelf(target, states);
        }

        virtual void update() { }
    };

    template <class T>
    class Entity9 : public BaseEntity9 {
        Drawable* _drawable;

    public:
        virtual ~Entity9() {
            delete _drawable;
        }

        virtual void drawSelf(DrawTarget& target, DrawStates states) { }

        virtual void draw(DrawTarget& target, DrawStates states) {
            states.transform *= getTransform();
            _drawable->draw(target, states);
        }

        Entity9(Drawable* drawable) : _drawable(drawable)
        { }

        friend class Scene9;
    };

    class Scene9 : public Drawable {
        vector<BaseEntity9*> _entities;

    protected:
        template <class T>
        inline T& add(T* entity) {
            _entities.push_back(entity);
            return *entity;
        }

    public:
        virtual ~Scene9() {
            deleteAll(_entities);
        }

        template <class TEntity, class Arg1>
        inline TEntity& create(Arg1& arg1) {
            return add(new TEntity(arg1));
        }

        template <class TDrawable, class Arg1>
        inline Entity9<TDrawable>& createAsEntity(Arg1& arg1) {
            TDrawable* drawable = new TDrawable(arg1);
            return create<Entity9<TDrawable>>(drawable);
        }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            for (size_t i = 0; i < _entities.size(); i++)
                target.draw(_entities[i], states);
        }

        void update() {
            for (size_t i = 0; i < _entities.size(); i++)
                _entities[i]->update();
        }
    };

    class SpriteEntity9 : public BaseEntity9 {
        Sprite _sprite;
    public:
        SpriteEntity9(Texture& tex) : _sprite(tex) { }

        virtual void drawSelf(DrawTarget& target, DrawStates states) {
            target.draw(_sprite, states);
        }

        virtual void update() {
            rotate(0.01f);
        }
    };

    void demo9() {
        Window window;
        Assets assets;
        Scene9 scene;

        window.setFramerateLimit(65);
        SpriteEntity9& spriteEntity = scene.create<SpriteEntity9>(assets.greenBlock);
        Entity9<Sprite>& spriteAsEntity = scene.createAsEntity<Sprite>(assets.yellowBlock);
        spriteEntity.setScale(100);
        spriteAsEntity.setScale(100);
        spriteAsEntity.setPosition(-100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            scene.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }

    template <template <class T> class TEntity>
    void create10() {
        //Texture tex;
        //auto test = new Entity9<Sprite>(tex);
        // auto test = new TEntity<TDrawable>(tex);
    }

    //template <class SomeType, template <class> class OtherType> class NestedTemplateClass
    //{
    //
    //};

    void demo10() {
        vector<int> vec;

        //print_container(vec);

        create10<Entity9>();

        // func2<Sprite, Entity9<Sprite>>();

        //test<Entity9<Sprite>>();

        // Assets assets;
        // Scene10 scene;

        // SpriteEntity10& entity = scene.create<SpriteEntity>(assets);
        // Entity10<Sprite>& sprite = scene.create<Entity10<Sprite>>(assets);
    }

    template <class T>
    class Creatable11 {
    public:
        template <class Arg1>
        static T* create(const Arg1& arg1) { return new T(arg1); }
    };

    class MyClass11 : public Creatable11<MyClass11> {
    public:
        MyClass11(int test) { }
    };

    class BaseEntity11 : public Creatable11<BaseEntity11> {
    };

    template <class TDrawable>
    class Entity11 : public BaseEntity11 {
    };

    void demo11() {
        MyClass11::create(42);
    }

    template <class T, class Arg1>
    T& create12(Arg1& arg1) {
        return *(new T(arg1));
    }

    void demo12() {
        Assets assets;
        /* SpriteEntity9& entity = */create12<SpriteEntity9>(assets.yellowBlock);
        Sprite* sprite = new Sprite(assets.greenBlock);
        /*Entity9<Sprite>& spriteAsEntity = */create12<Entity9<Sprite>>(sprite);
    }

    template <class TVal>
    class Arg13 {
        TVal _val;
        TVal& _valRef;
        bool _isRef;
    public:
        Arg13(TVal& val) : _valRef(val), _isRef(true)
        { }

        Arg13(const TVal& val) : _val(val), _valRef(_val), _isRef(false)
        {
        }

        TVal& get() { return _isRef ? _valRef : _val; }
    };

    template <class TArg1>
    class ArgList1_13 {
        Arg13<TArg1> _arg1;

    public:
        ArgList1_13(const Arg13<TArg1>& arg1) : _arg1(arg1)
        { }

        template <class T>
        T* construct() {
            return new T(_arg1.get());
        }
    };

    template <class T>
    Arg13<T> args13(T& t) {
        return Arg13<T>(t);
    }

    template <class T>
    class IntArg {
        T _val;

    public:
        IntArg(T val) : _val(val)
        { }
    };

    template <class T>
    void testOne13(Arg13<T> arg) {
    }

    template <class T>
    void testTwo13(T arg) {
    }

    template <class T>
    void testThree(IntArg<T> arg) {
    }

    class MyClass {
    public:
        MyClass(int val) {
        }
    };

    template <class TArg1>
    ArgList1_13<TArg1> argList13(TArg1& arg1) {
        return ArgList1_13<TArg1>(arg1);
    }

    template <class TArg1>
    ArgList1_13<TArg1> argList13(const TArg1& arg1) {
        return ArgList1_13<TArg1>(arg1);
    }

    template <class TEntity, class TArgList>
    TEntity& create(TArgList argList) {
        TEntity* entity = argList.template construct<TEntity>();        // https://stackoverflow.com/questions/3786360/confusing-template-error
        return *entity;
    }

    void demo13() {
        testOne13<int>(42);
        testTwo13(42);
        testThree<int>(42);

        int temp = 42;
        Arg13<int> test(temp);

        /*auto test2 = */args13(temp);

        ArgList1_13<int> argList(42);
        argList.construct<MyClass>();

        auto myArgs = argList13(42);
        /*auto myClass = */myArgs.construct<MyClass>();
        /*auto myClass2 =*/ create<MyClass>(argList13(42));

        // scene.create<SpriteEntity>(args(assets.yellowBlock));
    }

    template <class T>
    void safeDelete(T* t) {
        if (t)
            delete t;
    }

    template <class TVal>
    class BaseArg14 {
    protected:
        const TVal* _data;

        bool _isRef;

    public:
        virtual ~BaseArg14() {
            if (!_isRef)
                safeDelete(_data);
        }

        BaseArg14(const TVal* data, bool isRef) : _data(data), _isRef(isRef)
        { }

        BaseArg14(const BaseArg14& other) : _data(other._data), _isRef(other._isRef) {
            if (!_isRef) {
                _data = (TVal*)malloc(sizeof(TVal));
                memcpy((void*)_data, other._data, sizeof(TVal));
            }
        }

        inline const TVal& get() const {
            return *_data;
        }
    };

    template <class TVal>
    class Arg14 : public BaseArg14<TVal> {
        typedef BaseArg14<TVal> base;

    public:
        Arg14(const TVal& val) : base(new TVal(val), false) { }

        Arg14(TVal& val) : base(&val, true) { }
    };

    template <class TVal>
    class ConstArg14 : public BaseArg14<TVal> {
        typedef BaseArg14<TVal> base;

    public:
        ConstArg14(const TVal& val) : base(&val, true) { }
    };

    template <class TArg1>
    class ArgList1_14 {
        const Arg14<TArg1> _arg1;

    public:
        ArgList1_14(const Arg14<TArg1>& arg1) : _arg1(arg1)
        { }

        template <class T>
        T* construct() const {
            return new T(_arg1.get());
        }
    };

    template <class TArg1>
    ArgList1_14<TArg1> args14(const TArg1& arg1) {
        return ArgList1_14<TArg1>(arg1);
    }

    template <class T, class TArgList>
    void create14(const TArgList& argList) {
        argList.template construct<T>();
    }

    void demo14() {
        Arg14<int> test(42);
        int temp = 43;
        Arg14<int> test2(temp);
        const int temp2 = 44;
        ConstArg14<int> test3(temp2);

        cout << test.get() << endl;
        cout << test2.get() << endl;
        cout << test3.get() << endl;
        cin.get();
    }

    class FirstClass15 {
    public:
        FirstClass15(int val) {
            cout << val << endl;
        }
    };

    class SecondClass15 {
    public:
        SecondClass15(int& val) {
            cout << val << endl;
        }
    };

    class ThirdClass15 {
    public:
        ThirdClass15(const int& val) {
            cout << val << endl;
        }
    };

    template <class TVal>
    class BaseArg15 {
    protected:
        TVal* _data;

        bool _isRef;

    public:
        virtual ~BaseArg15() {
            if (!_isRef)
                safeDelete(_data);
        }

        BaseArg15(TVal* data, bool isRef) : _data(data), _isRef(isRef)
        { }

        BaseArg15(const BaseArg15& other) : _data(other._data), _isRef(other._isRef) {
            if (!_isRef) {
                _data = (TVal*)malloc(sizeof(TVal));
                memcpy((void*)_data, other._data, sizeof(TVal));
            }
        }

        inline TVal& get() const {
            return *_data;
        }
    };

    template <class TVal>
    class Arg15 : public BaseArg15<TVal> {
        typedef BaseArg15<TVal> base;

    public:
        Arg15(const TVal& val) : base(new TVal(val), false) { }

        Arg15(TVal& val) : base(&val, true) { }
    };

    template <class TVal>
    class ConstArg15 : public BaseArg15<TVal> {
        typedef BaseArg15<TVal> base;

    public:
        ConstArg15(const TVal& val) : base(&val, true) { }
    };

    template <class TArg1>
    class ArgList15 {
        const BaseArg15<TArg1>* _arg1;

    public:
        virtual ~ArgList15() {
            safeDelete(_arg1);
        }

        ArgList15(const Arg15<TArg1>& arg1) : _arg1(new Arg15<TArg1>(arg1)) {
        }

        ArgList15(const ConstArg15<TArg1>& arg1) : _arg1(new ConstArg15<TArg1>(arg1)) { }

        template <class T>
        T* construct() const {
            return new T(_arg1->get());
        }
    };

    void demo15() {
        ArgList15<int> firstArgs(Arg15<int>(42));
        firstArgs.construct<FirstClass15>();

        int test = 43;
        ArgList15<int> secondArgs((Arg15<int>(test)));
        secondArgs.construct<SecondClass15>();

        const int test2 = 44;
        ArgList15<int> thirdArgs((Arg15<int>(test2)));
        thirdArgs.construct<ThirdClass15>();

        cin.get();
    }

    template <class TArg1>
    ArgList15<TArg1> args16(const TArg1& arg1) {
        return ArgList15<TArg1>(Arg15<TArg1>(arg1));
    }

    void demo16() {
        ArgList15<int> firstArgs = args16(42);
        firstArgs.construct<FirstClass15>();

        //ArgList15<int> firstArgs = args16(42);
        //firstArgs.construct<FirstClass15>();

        // int test = 42;
        // ArgList15<int> secondArgs = args16(test);
        //secondArgs.construct<SecondClass15>();

        // const int test = 42;
        // ArgList15<int> thirdArgs = args16(test);
        //secondArgs.construct<ThirdClass15>();

        cin.get();
    }

    class Class17 {
    public:
        Class17() {
            cout << "created" << endl;
        }

        Class17(const Class17& other) {
            cout << "copied" << endl;
        }
    };

    template <class T>
    void create17(const T& t) {
    }

    void demo17() {
        create17(Class17());

        Class17 test;
        create17(test);

        const Class17 test2;
        create17(test2);

        cin.get();
    }

    void test18b(int& test) {
        cout << test << endl;
    }

    void test18(const int& test) {
        int& myTest = (int&)test;
        test18b(myTest);
    }

    void demo18() {
        int test = 42;
        test18(test);
        cin.get();
    }

    class Class19a {
    public:
        Class19a(int val) {
            cout << val << endl;
        }
    };

    class Class19b {
    public:
        Class19b(int& val) {
            cout << val << endl;
        }
    };

    class Class19c {
    public:
        Class19c(const int& val) {
            cout << val << endl;
        }
    };

    template <class T, class TArg>
    void create19(const TArg& arg) {
        new T((TArg&)arg);      // remove the constness. The value can then be used for const and non-const parameters
    }

    void demo19() {
        create19<Class19a>(42);
        int temp = 43;
        create19<Class19b>(temp);
        const int temp2 = 44;
        create19<Class19c>(temp2);

        cin.get();
    }

    class Texture20 {
        string _name;

    public:
        Texture20(const string& name) : _name(name) {
            cout << "Texture " << _name << " created" << endl;
        }

        Texture20(const Texture20& other) : _name(other._name) {
            cout << "Texture " << _name << " copied" << endl;
        }

        inline const string& getName() const { return _name; }
    };

    class Sprite20a {
    public:
        Sprite20a(Texture20 texture) {
            cout << "Sprite20a created with " << texture.getName() << endl;
        }
    };

    class Sprite20b {
    public:
        Sprite20b(Texture20& texture) {
            cout << "Sprite20b created with " << texture.getName() << endl;
        }
    };

    class Sprite20c {
    public:
        Sprite20c(const Texture20& texture) {
            cout << "Sprite20c created with " << texture.getName() << endl;
        }
    };

    class Sprite20d {
    public:
        Sprite20d(Texture20* texture) {
            cout << "Sprite20d created with " << texture->getName() << endl;
        }
    };

    class Sprite20e {
    public:
        Sprite20e(const Texture20* texture) {
            cout << "Sprite20e created with " << texture->getName() << endl;
        }
    };

    void demo20() {
        create19<Sprite20a>(Texture20("texture A"));
        Texture20 textureA("texture B");
        create19<Sprite20b>(textureA);
        const Texture20 textureC("texture C");
        create19<Sprite20c>(textureC);
        Texture20* textureD = new Texture20("texture D");
        create19<Sprite20d>(textureD);
        const Texture20* textureE = new Texture20("texture E");
        create19<Sprite20e>(textureE);

        cin.get();
    }

    class Entity21 : public Drawable, public Transformable {
    public:
        virtual ~Entity21() { }

        void draw(DrawTarget& target, DrawStates drawStates = DrawStates::getDefault()) {
            drawStates.transform *= getTransform();
            drawSelf(target, drawStates);
        }

        virtual void drawSelf(DrawTarget& target, DrawStates drawStates) { }

        void update() {
            updateSelf();
        }

        virtual void updateSelf() { }
    };

    template <class TDrawable>
    class DrawableEntity21 : public Entity21 {
        Drawable* _drawable;

    public:
        virtual ~DrawableEntity21() {
            delete _drawable;
        }

        DrawableEntity21(TDrawable* ownedDrawable) : _drawable(ownedDrawable)
        { }

        virtual void drawSelf(DrawTarget& target, DrawStates drawStates) {
            _drawable->draw(target, drawStates);
        }

        virtual void updateSelf() {
            cout << "DrawableEntity21::updateSelf()" << endl;
        }
    };

    class Scene21 : public Drawable {
        vector<Entity21*> _entities;

    public:
        virtual ~Scene21() {
            deleteAll(_entities);
        }

        template <class TEntity, class TArg>
        void createEntity(const TArg& arg) {
            TEntity* entity = new TEntity((TArg&)arg);
            _entities.push_back(entity);
        }

        template <class TDrawable, class TArg>
        void createDrawableEntity(const TArg& arg) {
            TDrawable* drawable = new TDrawable((TArg&)arg);
            DrawableEntity21<TDrawable>* entity = new DrawableEntity21<TDrawable>(drawable);
            _entities.push_back(entity);
        }

        virtual void draw(DrawTarget& target, DrawStates drawStates = DrawStates::getDefault()) {
            for (size_t i = 0; i < _entities.size(); i++)
                _entities[i]->draw(target);
        }

        void update() {
            for (size_t i = 0; i < _entities.size(); i++)
                _entities[i]->update();
        }
    };

    class Texture21 {
        string _path;
    public:
        Texture21(const string& path) : _path(path)
        { }

        inline const string& getPath() { return _path; }
    };

    class SpriteEntity21 : public Entity21 {
        Texture21& _texture;
    public:
        SpriteEntity21(Texture21& texture) : _texture(texture)
        { }

        virtual void drawSelf(DrawTarget& target, DrawStates drawStates) {
            cout << "SpriteEntity21::drawSelf() with texture " << _texture.getPath() << endl;
        }

        virtual void updateSelf() {
            cout << "SpriteEntity::updateSelf()" << endl;
        }
    };

    class Sprite21 : public Drawable {
        Texture21& _texture;
    public:
        Sprite21(Texture21& texture) : _texture(texture)
        { }

        virtual void draw(DrawTarget& target, DrawStates drawStates) {
            cout << "Sprite21::draw() with texture " << _texture.getPath() << endl;
        }
    };

    void demo21() {
        Window window;
        Scene21 scene;
        Texture21 texture1("path 1");
        Texture21 texture2("path 2");
        scene.createEntity<SpriteEntity21>(texture1);
        scene.createDrawableEntity<Sprite21>(texture2);
        scene.update();
        scene.draw(window);

        cin.get();
    }

    class Entity22;
    class Node22 {
        vector<Entity22*> _children;

    public:
        virtual ~Node22() {
            deleteAll(_children);
        }

        inline vector<Entity22*>& getChildren() { return _children; }

        template <class TEntity, class TArg>
        TEntity& createChild(const TArg& arg) {
            TEntity* entity = new TEntity((TArg&)arg);
            _children.push_back(entity);
            return *entity;
        }
    };

    class Entity22 : public Node22, public Drawable, public Transformable {
    public:
        virtual ~Entity22() { }

        void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            states.transform *= getTransform();
            drawSelf(target, states);

            for (size_t i = 0; i < getChildren().size(); i++)
                getChildren()[i]->draw(target, states);
        }

        virtual void drawSelf(DrawTarget& target, DrawStates drawStates) { }
    };

    class Scene22 : public Node22, public Drawable {
    public:
        virtual void draw(DrawTarget& target, DrawStates drawStates = DrawStates::getDefault()) {
            for (size_t i = 0; i < getChildren().size(); i++)
                getChildren()[i]->draw(target);
        }
    };

    class MyEntity22 : public Entity22 {
        size_t _depth;

    public:
        MyEntity22(size_t depth) : _depth(depth) {
            if (depth == 0) {
                createChild<MyEntity22>(1).setPosition(3, 4);   // expected global: 4, 6
                createChild<MyEntity22>(1).setPosition(5, 6);   // expected global: 6, 8
            }
        }

        Vector2f getGlobalPosition(const Transform& transform) {
            return Vector2f(transform.getMatrix()[6], transform.getMatrix()[7]);
        }

        void drawSelf(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            Vector2f globalPosition = getGlobalPosition(states.transform);
            cout << "MyEntity22: " << " depth: " << _depth << " position: " << globalPosition.x << " " << globalPosition.y << endl;
        }
    };

    class DummyDrawTarget22 : public DrawTarget {
        virtual void draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states) { }
    };

    void demo22() {
        DummyDrawTarget22 drawTarget;
        Scene22 scene;
        scene.createChild<MyEntity22>(0).setPosition(1, 2);
        scene.draw(drawTarget);

        cin.get();
    }

    class Entity23;
    template <class TDrawable> class DrawableEntity23;
    class Component23;

    class Node23 {
        Node23* _parent;
        vector<Entity23*> _children;
        vector<Component23*> _components;

    protected:
        inline void setParent(Node23& parent) { _parent = &parent; }

        template <class TElem, bool isEntity> struct adder { };

        template <class TElem> struct adder <TElem, true> {
            static TElem& add(Node23& that, TElem* elem) {
                that._children.push_back(elem);
                return *elem;
            }
        };

        template <class TElem> struct adder <TElem, false> {
            static TElem& add(Node23& that, TElem* elem) {
                that._components.push_back(elem);
                return *elem;
            }
        };

        template <class TElem>
        TElem& add(TElem* elem) {
            const bool isEntity = is_base_of6<Entity23, TElem>::value;
            TElem& addedElem = adder<TElem, isEntity>::add(*this, elem);
            addedElem.setParent(*this);
            return addedElem;
        }

    public:
        virtual ~Node23() {
            deleteAll(_children);
        }

        Node23() : _parent(NULL) {
        }

        inline Entity23& getParent() { return (Entity23&)(*_parent); }

        inline vector<Entity23*>& getChildren() { return _children; }

        inline vector<Component23*>& getComponents() { return _components; }

        // 0 args
        template <class TElem>
        TElem& create() {
            TElem* entity = new TElem();
            return add(entity);
        }

        // 0 args
        template <class TDrawable>
        DrawableEntity23<TDrawable>& createDrawableEntity() {
            TDrawable* drawable = new TDrawable();
            return create<DrawableEntity23<TDrawable>>(drawable);
        }

        // 1 arg
        template <class TElem, class TArg>
        TElem& create(const TArg& arg) {
            TElem* entity = new TElem((TArg&)arg);
            return add(entity);
        }

        // 1 arg
        template <class TDrawable, class TArg>
        DrawableEntity23<TDrawable>& createDrawableEntity(const TArg& arg) {
            TDrawable* drawable = new TDrawable((TArg&)arg);
            return create<DrawableEntity23<TDrawable>>(drawable);
        }
    };

    class Component23 : public Node23, public Drawable, public Transformable {
    public:
        void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            states.transform *= getTransform();
            drawSelf(target, states);
        }

        virtual void drawSelf(DrawTarget& target, DrawStates drawStates) { }

        virtual void updateSelf() { }
    };

    class Entity23 : public Node23, public Drawable, public Transformable {
    public:
        virtual ~Entity23() { }

        void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            states.transform *= getTransform();
            drawSelf(target, states);

            for (size_t i = 0; i < getChildren().size(); i++)
                getChildren()[i]->draw(target, states);

            for (size_t i = 0; i < getComponents().size(); i++)
                getComponents()[i]->draw(target, states);
        }

        virtual void drawSelf(DrawTarget& target, DrawStates drawStates) { }

        void update() {
            updateSelf();

            for (size_t i = 0; i < getChildren().size(); i++)
                getChildren()[i]->update();

            for (size_t i = 0; i < getComponents().size(); i++)
                getComponents()[i]->updateSelf();
        }

        virtual void updateSelf() { }
    };

    class Scene23 : public Node23, public Drawable {
    public:
        virtual void draw(DrawTarget& target, DrawStates drawStates = DrawStates::getDefault()) {
            for (size_t i = 0; i < getChildren().size(); i++)
                getChildren()[i]->draw(target);
        }

        void update() {
            for (size_t i = 0; i < getChildren().size(); i++)
                getChildren()[i]->update();
        }
    };

    template <class TDrawable>
    class DrawableEntity23 : public Entity23 {
        Drawable* _drawable;

    public:
        virtual ~DrawableEntity23() {
            delete _drawable;
        }

        DrawableEntity23(TDrawable* ownedDrawable) : _drawable(ownedDrawable)
        { }

        virtual void drawSelf(DrawTarget& target, DrawStates drawStates) {
            _drawable->draw(target, drawStates);
        }
    };

    class MyEntity23 : public Entity23 {
    public:
        virtual void drawSelf(DrawTarget& target, DrawStates drawStates) {
            cout << "MyEntity23::draw()" << endl;
        }
    };

    class MyComponent23 : public Component23 {
        int _val;

    public:
        MyComponent23(int val) : _val(val) { }

        virtual void drawSelf(DrawTarget& target, DrawStates drawStates) {
            cout << "MyComponent23::draw(), _val = " << _val << endl;
        }
    };

    class MyDrawable23 : public Drawable {
        int _val;

    public:
        MyDrawable23(int val) : _val(val)
        { }

        virtual void draw(DrawTarget& target, DrawStates drawStates) {
            cout << "MyComponent23::draw(), _val = " << _val << endl;
        }
    };

    void demo23() {
        DummyDrawTarget22 drawTarget;
        Scene23 scene;
        MyEntity23& entity = scene.create<MyEntity23>();
        entity.create<MyComponent23>(42);
        entity.createDrawableEntity<MyDrawable23>(43);
        scene.draw(drawTarget);

        cin.get();
    }

    class Rotator100 : public Component23 {
        float _omega;
    public:
        Rotator100(float omega) : _omega(omega) { }

        void updateSelf() {
            cout << getComponents().size() << endl;
            getParent().rotate(_omega);
        }
    };

    class MyEntity100 : public Entity23 {
        Sprite _sprite;

    public:
        MyEntity100(Texture& texture) : _sprite(texture) { }

        virtual void drawSelf(DrawTarget& target, DrawStates drawStates) {
            target.draw(_sprite, drawStates);
        }
    };

    void demo100() {
        Window window;
        Assets assets;
        Scene23 scene;

        window.setFramerateLimit(65);
        window.getCamera().setCenter(200);
        DrawableEntity23<Sprite>& sprite =
            scene.createDrawableEntity<Sprite>(assets.yellowBlock);
        sprite.setScale(100);
        sprite.setPosition(200);
        sprite.create<Rotator100>(0.1f);
        Entity23& entity = scene.create<MyEntity100>(assets.greenBlock);
        entity.setScale(100);
        entity.setPosition(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            scene.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }

    void run()
    {
        demo100();
        //demo23();
        //demo22();
        //demo21();
        //demo20();
        //demo19();
        //demo18();
        //demo17();
        //demo9();
        //demo16();
        //demo15();
        //demo14();
        //demo13();
        //demo12();
        //demo11();
        //demo10();
        //demo8();
        //demo7();
        //demo6();
        //demo5();
        //demo4();
        //demo3();
        //demo2();
        //demo1();
    }
}