For QuickJS see https://bellard.org/quickjs/

main file include simple JS test case is qjs_main.cpp

C++ wrappings: classa_wrap.cpp and classb_wrap.cpp
works partly: Can create objects from JS with mymod.createClassA() works but new mymod.ClassA() or mymod.new ClassB() don't work. Also the C++ holding references to JS objects is stilll under works.
