
#ifndef CLASS_A_H

#define CLASS_A_H

#include <printf.h>
#include "quickjs.h"

class ClassA {
    public:
        ClassA() {
            printf("constructor ClassA\n");
        }

        virtual ~ClassA() {
            printf("destructor ClassA\n");
        }

        virtual int getIntParam() {
            return 47;
        }
};

#endif