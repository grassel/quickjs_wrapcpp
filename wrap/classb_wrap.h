

#ifndef CLASS_B_WRAP_H
#define CLASS_B_WRAP_H

extern "C"
{
    #include "../cutils.h"
    #include "../quickjs-libc.h"
}

#include "classb.h"
#include "classa_wrap.h"


void js_classb_init(JSContext *ctx);
ClassB *getClassB(JSContext *ctx, JSValueConst js_classb_obj);
JSValue js_classb_create(JSContext *ctx, JSValueConst this_val,
                         int argc, JSValueConst *argv);

#endif