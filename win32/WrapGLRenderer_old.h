
#pragma once

#include "duktape.h"

#ifdef __cplusplus
extern "C" {
#endif

DUK_EXTERNAL_DECL duk_ret_t duk_open_GLRenderer(duk_context *ctx);

#ifdef __cplusplus
}
#endif

extern class GLRenderer* __renderer;
