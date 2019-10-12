

#ifndef CLASS_A_WRAP_H
#define CLASS_A_WRAP_H

extern "C"
{
    #include "../cutils.h"
    #include "../quickjs-libc.h"
}

#include "classa.h"


class classA;

void js_classa_finalizer(JSRuntime *rt, JSValue val);
void js_object_data_mark(JSRuntime *rt, JSValueConst val,
                         JS_MarkFunc *mark_func);

void js_classa_init(JSContext *ctx);
JSValue js_classa_create(JSContext *ctx, JSValueConst this_val,
                         int argc, JSValueConst *argv);
ClassA *getClassA(JSContext *ctx, JSValueConst js_classa_obj);
JSValue js_classa_get_int_param(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv);

#endif