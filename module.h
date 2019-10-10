

#ifndef MODULE_H
#define MODULE_H

extern "C"
{
    #include "cutils.h"
    #include "quickjs-libc.h"
}

JSModuleDef *js_init_mymodule_module(JSContext *ctx);


#endif