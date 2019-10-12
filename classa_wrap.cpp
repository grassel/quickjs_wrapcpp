extern "C"
{
#include "cutils.h"
#include "quickjs-libc.h"

    // in quickjs.c
    JSValueConst JS_NewGlobalCConstructor(JSContext *ctx, const char *name,
                                          JSCFunction *func, int length,
                                          JSValueConst proto);

    typedef struct JSObject JSObject;
}

#include "classa.h"

JSClassID js_classa_id;
JSClassID js_classb_id;

void js_classa_finalizer(JSRuntime *rt, JSValue val)
{
    printf("js_classa_finalizer ...\n");
    void* data = JS_GetOpaque(val, js_classa_id);
    if (data)
    {
        ClassA *obj = (ClassA *) data;
        printf("   ... deleting obja [%d]\n", obj->id);
        delete obj;
    }
    else
    {
        printf("ERROR: js_classa_finalizer: can not get handle to object from opaque data.\n");
    }
}

void js_object_data_mark(JSRuntime *rt, JSValueConst val,
                         JS_MarkFunc *mark_func)
{
    printf("js_object_data_mark ...FIXME \n");

   // FIXME
   // JSObject *p = JS_VALUE_GET_OBJ(val);
   // JS_MarkValue(rt, p->u.object_data, mark_func);
}

ClassA *getClassA(JSContext *ctx, JSValueConst js_classa_obj)
{
    return (ClassA *) JS_GetOpaque3(js_classa_obj, js_classa_id, js_classb_id);
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
    ClassA *obj = getClassA(ctx, this_val);
    int result = obj->getIntParam();
    // printf("js_classa_get_int_param: result=%d. \n", result);
    JSValue jsResult = JS_NewInt32(ctx, result);
    return jsResult;
}

static JSClassDef js_classa_class = {
    "ClassA",
    .finalizer = js_classa_finalizer,
    .gc_mark = js_object_data_mark
};

static const JSCFunctionListEntry js_classa_proto_funcs[] = {
    JS_CFUNC_DEF("getIntParam", 0, js_classa_get_int_param)
};

void js_classa_init(JSContext *ctx)
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

    JS_NewGlobalCConstructor(ctx, "ClassA", js_classa_create, 0, proto);

    printf("js_classa_init ... DONE\n");
}
