extern "C"
{
    #include "cutils.h"
    #include "quickjs-libc.h"
}

#include "classa_wrap.h"
#include "classb_wrap.h"

//const JSCFunctionListEntry js_my_module_funcs[] = {
//    JS_CFUNC_DEF("createClassA", 0, js_classa_create),
//};

int js_mymodule_init(JSContext *ctx, JSModuleDef *m)
{
   js_classa_init(ctx, m);
   js_classb_init(ctx, m);
   return 0;
}

JSModuleDef *js_init_mymodule_module(JSContext *ctx)
{
    JSModuleDef *m = JS_NewCModule(ctx, "mymod", js_mymodule_init);
    if (!m)
        return NULL;
  
    JS_AddModuleExport(ctx, m, "ClassA");
    JS_AddModuleExport(ctx, m, "ClassB");

    return m;
}