
#ifndef CLASS_A_H

#define CLASS_A_H

#include <printf.h>
#include "quickjs.h"

class ClassA {
    public:
        ClassA() {
            id = ClassA::counter++;
            printf("[%i] constructor ClassA\n", id);
        }

        virtual ~ClassA() {
            printf("[%i] destructor ClassA\n", id);
        }

        virtual int getIntParam() {
            printf("[%i] ClassA.getIntParam() returns 47\n", id);
            return 47;
        }
        int id;

        static int counter;
};

#endif