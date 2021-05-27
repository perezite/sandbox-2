#include <iostream>
#include <vector>

using namespace std;

namespace t1 {
    namespace my {
        template<bool B, typename T = void> struct enable_if {
        };

        template<typename T> struct enable_if<true, T> {
            typedef T type; };
    }

    template<bool B = false> struct print_boolean {
        static void run() {
            cout << "false" << endl;
        }
    };

    template<> struct print_boolean<true> {
        static void run() {
            cout << "true" << endl;
        }
    };

    void demo() {
        my::enable_if<1 == 1, int>::type x = 1;
        cout << x << endl;
        const bool test = false;
        print_boolean<test>::run();
        const bool test2 = true;
        print_boolean<true>::run();
        cin.get();
    }
}

namespace t2 {
    class Base {};
    class Derived : public Base {};

    template <class Base, class Derived> struct is_base_of
    {
        typedef char Yes[1];
        typedef char No[2];

        static Yes& test(Base*) { }
        static No& test(...) { }

        static Derived* get() { };

        static const bool value = sizeof(test(get())) == sizeof(Yes);

    };

    void demo() {
        cout << is_base_of<Base, Derived>::value << endl;
        cout << is_base_of<Derived, Base>::value << endl;
        cout << is_base_of<int, float>::value << endl;

    }
}

namespace t3 {
    void func() {
        cout << "func()" << endl;
    }

    template <class Func>
    void execute(Func f) {
        f();
    }

    void demo() {
        execute(func);
    }
}

namespace t4 {
    void func(int x) { }

    int func2(float x) { return 42; }

    int func3() { return 42; }

    typedef char Yes[1];
    typedef char No[2];

    template<typename Ret, typename Arg1> Yes& check(Ret(func)(Arg1)) { }
    template<class Func> No& check(Func f) { }

    template <class Func> static const bool has_one_argument_func(Func f) {
        return sizeof(check(f)) == sizeof(Yes);
    }

    template <bool> void compileTimeConstantCheck() { }

    static const bool myTest() { return false; }

    struct test_struct {
        static const bool compileTimeConstant = true;
    };

    template <typename Ret, typename Arg1, Ret(*)(Arg1)> struct has_one_argument {
        static const bool value = true;
    };

    //template <typename Func> struct has_one_argument {
    //    static const bool value = true;
    //};

#define T4_YES(expression) (sizeof(expression) == sizeof(Yes))

    void demo()
    {
        //compileTimeConstantCheck<rand()% 3 == 5>();                   // will not compile

        //compileTimeConstantCheck<has_one_argument_func(func)>();      // will not compile

        //compileTimeConstantCheck<myTest()>();                         // will not compile
 
        const bool myBool2 = true;
        compileTimeConstantCheck<myBool2>();

        compileTimeConstantCheck<test_struct::compileTimeConstant>();

        const bool myBool3 = (sizeof(check(func2)) == sizeof(Yes));
        compileTimeConstantCheck<myBool3>();

        cout << T4_YES(check(func)) << endl;
        cout << (sizeof(check(func2)) == sizeof(Yes)) << endl;
        cout << (sizeof(check(func3)) == sizeof(Yes)) << endl;

         //has_one_argument<func>::value;
    }
}

namespace t5 {
    template <class T> struct MyTemplate { };

    template <template<class> class H, class S>
    void f(const H<S>& var) {
    }

    template <template<class> class H, class S>
    struct has_one_argument { };

    //template <template<class, class> void(*)(Arg1), class Ret, class Arg1>
    //struct has_one_argument_2 { };

    int func(int) { return 0; }

    void demo() {
        //MyTemplate<int> test;
        //has_one_argument<MyTemplate, int> test2;
        //has_one_argument_2<func> test3;
    }
}

namespace t6 {
    template <class Ret, class Arg1> struct has_one_argument {
        has_one_argument(Ret(func)(Arg1)) { }
        //static const bool value = true;
    };

    //template <class Any = void, class Empty = void> class has_one_argument {
    //    has_one_argument(class Any) { }
    //    static const bool value = true;
    //};

    void func(int x) { }

    void demo() {
        //has_one_argument::temp(func);
    }
}

namespace t7 {
    template <class Func> struct has_one_argument {
        template<typename Ret, typename Arg1, Ret(*)(Arg1)> struct Sfinae {};
        //template<typename U> static char check(Sfinae<U, &U::used_memory>*);
        //static const bool value = sizeof(check<Func>(0) == sizeof(char));
    };

    void demo() {

    }
}

template <bool> struct is_compiled_boolean {
    static const bool value = true;
};

typedef char Yes[1];
typedef char No[2];

namespace d8 {

    namespace has_one_arg {

        template<typename Ret, typename Arg1> Yes& check(Ret(func)(Arg1)) { }
        template<class Func> No& check(Func f) { }
    }

#define T8_EVAL(expression) (sizeof(expression) == sizeof(Yes))
#define D8_HAS_ONE_ARG(func) (T8_EVAL(has_one_arg::check(func)))

    void func(int x) { }
    int func2(int x) { return 42; }
    int func3() { return 42; }
    float func4(float x, float y) { return 42; }


    void demo() {
        const bool constantExpression = true;
        const bool nonConstantExpression = rand() % 3 == 0;
        is_compiled_boolean<constantExpression>::value;
        // is_compiled_boolean<nonConstantExpression>::value;      // will not compile

        const bool result1 = D8_HAS_ONE_ARG(func);
        const bool result2 = D8_HAS_ONE_ARG(func2);
        const bool result3 = D8_HAS_ONE_ARG(func3);
        const bool result4 = D8_HAS_ONE_ARG(func4);
        is_compiled_boolean<result1>::value;
        is_compiled_boolean<result2>::value;
        is_compiled_boolean<result3>::value;
        is_compiled_boolean<result4>::value;
        
        cout << boolalpha;
        cout << result1 << endl;
        cout << result2 << endl;
        cout << result3 << endl;
        cout << result4 << endl;
    }
}

int main()  
{
    d8::demo();
    //t7::demo();
    //t6::demo();
    //t5::demo();
    //t4::demo();
    //t3::demo();
    //t2::demo();
    //t1::demo();
    cin.get();
}

// Sources
// https://eli.thegreenplace.net/2014/sfinae-and-enable_if/
// https://jguegant.github.io/blogs/tech/sfinae-introduction.html
// https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature
// https://people.eecs.berkeley.edu/~brock/blog/detection_idiom.php
// https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function
// https://isocpp.org/wiki/faq/templates