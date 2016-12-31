#include "WrapGLRenderer.h"
#include "GLRenderer.h"
#include <cassert>

duk_ret_t RendererPresent(duk_context *ctx)
{
    __renderer->Present();
    return 0;
}

duk_ret_t duk_open_GLRenderer(duk_context *ctx)
{
    int top = duk_get_top(ctx);

    duk_push_global_object(ctx);
    duk_push_c_function(ctx, RendererPresent, 0);
    duk_put_prop_string(ctx, -2, "GLRendererPresent");
    duk_pop(ctx);

    // ¶ÏÑÔ¶ÑÕ»Õý³£
    assert( duk_get_top(ctx) == top );

    return 0;
}