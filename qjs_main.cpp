
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#if defined(__APPLE__)
#include <malloc/malloc.h>
#elif defined(__linux__)
#include <malloc.h>
#endif

extern "C"
{
#include "cutils.h"
#include "quickjs-libc.h"
}

#include "module.h"
#include "classa_wrap.h"
#include "classb_wrap.h"

extern const uint8_t qjsc_repl[];
extern const uint32_t qjsc_repl_size;

JSModuleDef *js_init_mymodule_module(JSContext *ctx);

static int eval_buf(JSContext *ctx, const char *buf, int buf_len,
                    const char *filename, int eval_flags)
{
    JSValue val;
    int ret;

    if ((eval_flags & JS_EVAL_TYPE_MASK) == JS_EVAL_TYPE_MODULE)
    {
        /* for the modules, we compile then run to be able to set
           import.meta */
        val = JS_Eval(ctx, buf, buf_len, filename,
                      eval_flags | JS_EVAL_FLAG_COMPILE_ONLY);
        if (!JS_IsException(val))
        {
            js_module_set_import_meta(ctx, val, TRUE, TRUE);
            val = JS_EvalFunction(ctx, val);
        }
    }
    else
    {
        val = JS_Eval(ctx, buf, buf_len, filename, eval_flags);
    }
    if (JS_IsException(val))
    {
        js_std_dump_error(ctx);
        ret = -1;
    }
    else
    {
        ret = 0;
    }
    JS_FreeValue(ctx, val);
    return ret;
}

#if defined(__APPLE__)
#define MALLOC_OVERHEAD 0
#else
#define MALLOC_OVERHEAD 8
#endif

int main(int argc, char **argv)
{

    JSRuntime *rt = JS_NewRuntime();
    if (!rt)
    {
        fprintf(stderr, "qjs: cannot allocate JS runtime\n");
        exit(2);
    }
    JSContext *ctx = JS_NewContext(rt);
    if (!ctx)
    {
        fprintf(stderr, "qjs: cannot allocate JS context\n");
        exit(2);
    }

    /* loader for ES6 modules */
    JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);

    js_std_add_helpers(ctx, 0, NULL);
    JSModuleDef *m = js_init_mymodule_module(ctx);

    printf("importing mymod ... \n ");

    const char *str = "import * as mymod from 'mymod';\n"
                      "globalThis.mymod = mymod;\n"
                      "var global = globalThis;\n";
    eval_buf(ctx, str, strlen(str), "<input>", JS_EVAL_TYPE_MODULE);

    printf("importing mymod done, executing some test JS next. \n");

    str = "console.log('[JS] starting ....');\n"
         "var obj = mymod.createClassA();\n"    // works
         "console.log('[JS] obj.getIntParam() ', obj.getIntParam());\n"
         "var obj2 = new mymod.ClassA();\n";    // "not a function error" here
         // "var obj2 = mymod.ClassA();\n";    //  also "not a function error" here
         /*
         "obj = obj2;\n"
         "console.log('[JS] obj2 -> obj.getIntParam() ', obj.getIntParam());\n"
         "var objb = new mymod.ClassB();\n"
         "console.log('[JS] objb.getIntParam() ', objb.getIntParam());\n"
        "var objab = new mymod.ClassB(obj2);\n"
         "console.log('[JS] objab.getIntParam() ', objab.getIntParam());\n"
         "console.log('[JS] objab.getIntParamObjA() ', objab.getIntParamObjA());\n";
    */
    eval_buf(ctx, str, strlen(str), "<input>", JS_EVAL_TYPE_MODULE);
    
    js_std_loop(ctx);

    JSMemoryUsage stats;
    JS_ComputeMemoryUsage(rt, &stats);
    JS_DumpMemoryUsage(stdout, &stats, rt);

    js_std_free_handlers(rt);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    return 0;
}
