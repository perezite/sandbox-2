#include <iostream>

using namespace std;

namespace d1 {
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

namespace d2 {
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

int main()
{
    d2::demo();
    //d1::demo();
    cin.get();
}

// Sources
// https://eli.thegreenplace.net/2014/sfinae-and-enable_if/
