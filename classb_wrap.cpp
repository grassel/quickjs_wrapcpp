
extern "C"
{
  #include "cutils.h"
  #include "quickjs-libc.h"

  // in quickjs.c
    JSValueConst JS_NewGlobalCConstructor(JSContext *ctx, const char *name,
                                          JSCFunction *func, int length,
                                          JSValueConst proto);
}

#include "classb.h"
#include "classa_wrap.h"
#include "classb_wrap.h"


extern JSClassID js_classb_id;

ClassB *getClassB(JSContext *ctx, JSValueConst js_classb_obj) {
    return (ClassB *)JS_GetOpaque2(ctx, js_classb_obj, js_classb_id);
}

JSValue js_classb_create(JSContext *ctx, JSValueConst this_val,
                         int argc, JSValueConst *argv)
{
    printf("js_classb_create argc=%i ...\n", argc);

    JSValue jsObj = JS_NewObjectClass(ctx, js_classb_id);
    if (JS_IsException(jsObj))
    {
        return jsObj;
    }

    ClassB *objB = new ClassB();
    JS_SetOpaque(jsObj, objB);

    if ((argc > 0) &&
        (JS_IsObject(argv[0]))) {
        ClassA *obj = getClassA (ctx, argv[0]);
        printf("js_classb_create found param ObjA, intParam=%i \n", obj->getIntParam());
        objB->refObj = JS_DupValue(ctx, argv[0]);
    } else {

    }
    return jsObj;
}


JSValue js_classb_get_int_param_obj_a(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    printf("js_classa_get_int_param_obj_a ...\n");
    ClassB *objB = getClassB (ctx, this_val);
    ClassA *objA = getClassA (ctx, objB->refObj);
    int result = objA->getIntParam();
    
    printf("js_classb_get_int_param_obj_a: result=%d. \n", result);
    JSValue jsResult = JS_NewInt32(ctx, result);
    return jsResult;
}


static JSClassDef js_classb_class = {
    "ClassB",
    .finalizer = js_class_finalizer,
    .gc_mark = js_object_data_mark
};


static const JSCFunctionListEntry js_classb_proto_funcs[] = {
    JS_CFUNC_DEF("getIntParam", 0, js_classa_get_int_param),
    JS_CFUNC_DEF("getIntParamObjA", 0, js_classb_get_int_param_obj_a)
};


void js_classb_init(JSContext *ctx)
{
    printf("js_classb_init ...\n");

    /* C+ ClassB: wrapping
       the class ID is created once */
    JS_NewClassID(&js_classb_id);

    /* the JS class is created once per runtime */
    JS_NewClass(JS_GetRuntime(ctx), js_classb_id, &js_classb_class);
    JSValue proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto, js_classb_proto_funcs,
                               countof(js_classb_proto_funcs));
    JS_SetClassProto(ctx, js_classb_id, proto);

    JS_NewGlobalCConstructor(ctx, "ClassB", js_classb_create, 1, proto);

    printf("js_classb_init ... DONE\n");
}

