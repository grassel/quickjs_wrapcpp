
#ifndef CLASS_B_H

#define CLASS_B_H

#include <printf.h>
#include "quickjs.h"
#include "classa.h"

class ClassB : public ClassA {
    public:
        ClassB()  {
            printf("constructor ClassB\n");
        }

        virtual ~ClassB() {
            printf("destructor ClassB\n");
        }

        virtual int getIntParam() {
            return 48;
        }
        
        // implemented in the wrapping
        // int getIntParamA();
            
        JSValue refObj;
};

#endif