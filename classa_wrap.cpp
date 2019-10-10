extern "C"
{
    #include "cutils.h"
    #include "quickjs-libc.h"
}

#include "classa.h"

static JSClassID js_classa_id;

 void js_class_finalizer(JSRuntime *rt, JSValue val)
{
    printf("js_class_finalizer ...\n");
    ClassA *obj = (ClassA *)JS_GetOpaque(val, js_classa_id);
    if (obj)
    {
        delete obj;
    }
    else
    {
        printf("ERROR: js_class_finalizer: can not get handle to object from opaque data.\n");
    }
}

ClassA *getClassA(JSContext *ctx, JSValueConst js_classa_obj) {
    return (ClassA *)JS_GetOpaque2(ctx, js_classa_obj, js_classa_id);
}


JSValue js_classa_create(JSContext *ctx, JSValueConst this_val,
                         int argc, JSValueConst *argv)
{
    printf("js_classa_create ...\n");

    JSValue jsObj = JS_NewObjectClass(ctx, js_classa_id);
    if (JS_IsException(jsObj))
    {
        return jsObj;
    }

    ClassA *obj = new ClassA();

    JS_SetOpaque(jsObj, obj);
    return jsObj;
}

JSValue js_classa_get_int_param(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    printf("js_classa_get_int_param ...\n");
    ClassA *obj = getClassA (ctx, this_val);
    int result = obj->getIntParam();
    printf("js_classa_get_int_param: result=%d. \n", result);
    JSValue jsResult = JS_NewInt32(ctx, result);
    return jsResult;
}

static JSClassDef js_classa_class = {
    "ClassA",
    .finalizer = js_class_finalizer
};


static const JSCFunctionListEntry js_classa_proto_funcs[] = {
    JS_CFUNC_DEF("getIntParam", 0, js_classa_get_int_param)
};


void js_classa_init(JSContext *ctx, JSModuleDef *m)
{
    printf("js_classa_init ...\n");

    /* C+ ClassA: wrapping
       the class ID is created once */
    JS_NewClassID(&js_classa_id);

    /* the JS class is created once per runtime */
    JS_NewClass(JS_GetRuntime(ctx), js_classa_id, &js_classa_class);


    JSValue proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto, js_classa_proto_funcs,
                               countof(js_classa_proto_funcs));
    JS_SetClassProto(ctx, js_classa_id, proto);


    /* ClassA constructor */
    JSValue construct = JS_NewCFunction2(ctx, js_classa_create, "ClassA", 0, JS_CFUNC_constructor, 0);
    JS_SetModuleExport(ctx, m, "ClassA", construct);

    printf("js_classa_init ... DONE\n");
}
