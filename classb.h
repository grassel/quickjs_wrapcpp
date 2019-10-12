
#ifndef CLASS_B_H

#define CLASS_B_H

#include <printf.h>
#include "quickjs.h"
#include "classa.h"

class ClassB : public ClassA {
    public:
        ClassB()  {
            printf("[%i] constructor ClassB\n", id);
        }

        virtual ~ClassB() {
            printf("[%i] destructor ClassB\n", id);
        }

        virtual int getIntParam() {
            printf("[%i] ClassB.getIntParam() returns 48\n", id);
            return 48;
        }
        
        // implemented in the wrapping
        // int getIntParamA();
            
        JSValue refObj;
};

#endif