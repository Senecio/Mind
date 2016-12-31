#include "WrapGLRenderer.h"
#include "GLRenderer.h"
#include <cassert>

class GLRenderer* __renderer = 0;
/*
interface Renderer {

    // 清除
    Clear(color? : Color, depth? : number);
    // 呈现 
    Present();

    // 设置视口
    SetViewPort(rect: Rect);
    // 获取视口
    GetViewPort(): Rect;

    // 设置矩阵
    SetMatrix(type, matrix: Matrix);
    // 获取矩阵
    GetMatrix(type, matrix : Matrix);

    // 清除点
    ClearPoints();
    // 添加点
    AddPoint(position: Vec3, color: Color);
    // 绘制已添加的点
    DrawPoints();

    // 清除线
    ClearLines();
    // 添加线
    AddLine(point1: Vec3, point2: Vec3, color: Color);
    // 绘制已添加的线
    DrawLines();

    // 清除实体四边形
    ClearSolidQuad();
    // 添加实体四边形
    AddSolidQuad(p1: Vec3, p2: Vec3);
    // 绘制已添加的实体四边形
    DrawSolidQuad();

    // 清除纹理四边形
    ClearQuads();
    // 添加纹理四边形
    AddQuad(p1: Vec3, p2: Vec3, p3: Vec3, p4: Vec3);
    // 绘制已添加的纹理四边形
    DrawQuads();
}
*/

//#define DUK_GetNumberFromProp(ctx, idx, propName, retVal)

#define ErrorParameter  "error parameter type."
#define ErrorSyntax     "error syntax use."

inline bool DukGetNumberFromProp(duk_context *ctx, int idx, const char* propName, double& retVal)
{
    if (duk_has_prop_string(ctx, idx, propName))
    {
        duk_get_prop_string(ctx, 0, propName);
        retVal = duk_get_number(ctx, -1);
        duk_pop(ctx); // 弹出结果
        return true;
    }
    return false;
}

inline bool DukGetIntFromProp(duk_context *ctx, int idx, const char* propName, int& retVal)
{
    if (duk_has_prop_string(ctx, idx, propName))
    {
        duk_get_prop_string(ctx, 0, propName);
        retVal = duk_get_int(ctx, -1);
        duk_pop(ctx); // 弹出结果
        return true;
    }
    return false;
}

inline bool DukGetNumberFormStack(duk_context *ctx, int idx, double& retVal)
{
    if (duk_is_number(ctx, idx))
    {
         retVal = duk_get_number(ctx, idx);
         return true;
    }
    return false;
}

inline bool DukGetIntFormStack(duk_context *ctx, int idx, int& retVal)
{
    if (duk_is_number(ctx, idx))
    {
        retVal = duk_get_int(ctx, idx);
        return true;
    }
    return false;
}

static duk_ret_t Renderer_prototype_Clear(duk_context *ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    double r,g,b,a,depth;
    if (n == 1){
        if (duk_is_object(ctx, 0)) {

            if (DukGetNumberFromProp(ctx, 0, "a", a) && 
                DukGetNumberFromProp(ctx, 0, "r", r) &&
                DukGetNumberFromProp(ctx, 0, "g", g) &&
                DukGetNumberFromProp(ctx, 0, "b", b)) {

                __renderer->SetClearColor((float)r, (float)g, (float)b, (float)a);
                __renderer->Clear(RenderClearFlag::Color);
            }else
            {
                printf("%s %s\n",__FUNCTION__, ErrorParameter);
                return 0;
            }
        }

    }else if (n == 2) {

        if (DukGetNumberFromProp(ctx, 0, "a", a) && 
            DukGetNumberFromProp(ctx, 0, "r", r) &&
            DukGetNumberFromProp(ctx, 0, "g", g) &&
            DukGetNumberFromProp(ctx, 0, "b", b)) {

            __renderer->SetClearColor((float)r, (float)g, (float)b, (float)a);
        }else
        {
            printf("%s %s\n",__FUNCTION__, ErrorParameter);
            return 0;
        }

        if (DukGetNumberFormStack(ctx, 1, depth))
        {
            __renderer->SetClearDepth((float)depth);
        }else
        {
            printf("%s %s\n",__FUNCTION__, ErrorParameter);
            return 0;
        }
        __renderer->Clear(RenderClearFlag::Color | RenderClearFlag::Depth);
    }
    else{
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }
    assert( duk_get_top(ctx) == n);
    return 0;
}

static duk_ret_t Renderer_prototype_SetVertexBuffer(duk_context *ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1) {
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    if (duk_is_object(ctx, 0))
    {
        duk_get_global_string(ctx, "GLRenderer");
        duk_dup(ctx, 0);
        duk_put_prop_string(ctx, -2, "bindVB");
        duk_pop(ctx);
    }
    return 0;
}

static duk_ret_t Renderer_prototype_SetIndexBuffer(duk_context *ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1) {
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    if (duk_is_object(ctx, 0))
    {
        duk_push_global_object(ctx);
        duk_push_string(ctx, "GLRenderer");
        duk_get_prop(ctx, -1);
        duk_dup(ctx, 0);
        duk_put_prop_string(ctx, -2, "bindIB");
        duk_pop(ctx);
    }
    assert( duk_get_top(ctx) == n);
    return 0;
}


static duk_ret_t Renderer_prototype_SetShader(duk_context *ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1) {
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    if (duk_is_object(ctx, 0))
    {
        duk_get_global_string(ctx, "GLRenderer");
        duk_dup(ctx, 0);
        duk_put_prop_string(ctx, -2, "bindShader");
        duk_pop(ctx);
    }
    assert( duk_get_top(ctx) == n);
    return 0;
}

static duk_ret_t Renderer_prototype_DrawPrimitive(duk_context *ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 4) {
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int primitiveType;
    int primitveCount;
    int verticesCount;
    int offset;
    
    if (DukGetIntFormStack(ctx, 0, primitiveType) &&
        DukGetIntFormStack(ctx, 1, primitveCount) &&
        DukGetIntFormStack(ctx, 2, verticesCount) &&
        DukGetIntFormStack(ctx, 3, offset))
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "bindVB");
        duk_get_prop_string(ctx, -1, "__glvb");
        GLVertexBuffer *pVB = (GLVertexBuffer *)duk_get_pointer(ctx, -1);
        duk_pop(ctx);

        VertexDeclaration* pVDecl;
        duk_get_prop_string(ctx, -1, "declaration");
        duk_get_prop_string(ctx, -1, "__c_declaration");
        if (duk_is_undefined(ctx, -1))
        {
            duk_pop(ctx);
            pVDecl = new VertexDeclaration();
            VertexDeclaration::Element element(VertexDeclarationAttribute::None,0,VertexDeclarationDataType::None);
            duk_push_string(ctx, "GetNumberElement");
            duk_call_prop(ctx, -2, 0);
            int count = duk_get_int(ctx, -1);
            duk_pop(ctx);
            for (int i = 0; i < count; ++i)
            {
                duk_push_string(ctx, "GetIndexElement");
                duk_push_int(ctx, i);
                duk_call_prop(ctx, -3, 1);

                duk_get_prop_string(ctx, -1, "attribute");
                element.attribute = (VertexDeclarationAttribute::Enum)duk_get_int(ctx, -1);
                duk_pop(ctx);                
                duk_get_prop_string(ctx, -1, "attributeName");
                element.attributeName = duk_get_string(ctx, -1);
                duk_pop(ctx);  
                duk_get_prop_string(ctx, -1, "dataType");
                element.dataType = (VertexDeclarationDataType::Enum)duk_get_int(ctx, -1);
                duk_pop(ctx);

                duk_pop(ctx);
                pVDecl->AppendElement(element);
            }
            duk_push_pointer(ctx, pVDecl);
            duk_put_prop_string(ctx, -2, "__c_declaration");
        }else {
            pVDecl = (VertexDeclaration*)duk_get_pointer(ctx, -1);
            duk_pop(ctx);
        }
        duk_pop(ctx);


        duk_pop(ctx);

        duk_get_prop_string(ctx, -1, "bindShader");
        duk_get_prop_string(ctx, -1, "__glshader");
        GLShader *pShader = (GLShader *)duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        duk_pop(ctx);

        duk_pop(ctx);
        assert( duk_get_top(ctx) == n);
         __renderer->DrawPrimitive(primitiveType, primitveCount, verticesCount, offset, pVB, pShader, *pVDecl);
    }
    else {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
    }
    
    return 0;
}

static duk_ret_t Renderer_prototype_CreateVertexBuffer(duk_context *ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2) {
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    duk_eval_string(ctx, "GLVertexBuffer");
    duk_dup(ctx, 0);
    duk_dup(ctx, 1);
    duk_ret_t rc = duk_pnew(ctx, 2);
    if (rc != 0) {
        printf("failed: %s\n", duk_safe_to_string(ctx, -1));
        return 0;
    }
    assert( duk_get_top(ctx) == n + 1);
    return 1;
}

static duk_ret_t Renderer_prototype_CreateShader(duk_context *ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2) {
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }
    
    const char* vs = duk_get_string(ctx, 0);
    const char* fs = duk_get_string(ctx, 1);

    duk_eval_string(ctx, "GLShader");
    duk_ret_t rc = duk_pnew(ctx, 0);
    if (rc != 0) {
        printf("failed: %s\n", duk_safe_to_string(ctx, -1));
        return 0;
    }

    duk_push_string(ctx, "Create");
    duk_push_string(ctx, vs);
    duk_push_string(ctx, fs);
    duk_call_prop(ctx, -4, 2);
    if (duk_get_boolean(ctx, -1) == false)
    {
        duk_pop(ctx);
        duk_push_null(ctx);
    }else {
        duk_pop(ctx);
    }

    assert( duk_get_top(ctx) == n + 1);
    return 1;
}

static duk_ret_t Renderer_prototype_Present(duk_context *ctx)
{
    __renderer->Present();
    return 0;
}

static duk_ret_t ConstructorVertexBuffer(duk_context *ctx)
{
    int n = duk_get_top(ctx);
    duk_push_this(ctx); 
    duk_dup(ctx, 0);
    duk_put_prop_string(ctx, -2, "declaration");
    duk_dup(ctx, 1);
    duk_put_prop_string(ctx, -2, "usage");
    duk_push_pointer(ctx, new GLVertexBuffer());
    duk_put_prop_string(ctx, -2, "__glvb");
    duk_pop(ctx);
    assert( duk_get_top(ctx) == n);
    return 0;
}

static duk_ret_t VertexBuffer_prototype_GetDeclaration(duk_context *ctx)
{
    int n = duk_get_top(ctx);
    duk_push_this(ctx);
    duk_push_string(ctx, "declaration");
    duk_get_prop(ctx, -2);
    duk_swap(ctx, -1, -2);
    duk_pop(ctx); // pop this.
    assert( duk_get_top(ctx) == n + 1);
    return 1;
}

static duk_ret_t VertexBuffer_prototype_GetUsage(duk_context *ctx)
{
    int n = duk_get_top(ctx);
    duk_push_this(ctx);
    duk_push_string(ctx, "usage");
    duk_get_prop(ctx, -2);
    assert( duk_get_top(ctx) == n + 1);
    return 1;
}

static duk_ret_t VertexBuffer_prototype_SetData(duk_context *ctx)
{
    int n = duk_get_top(ctx);
    if (n != 1) {
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    if (duk_is_object(ctx, 0)) {
        duk_size_t len;
        float *buf;
        buf = (float *)duk_get_buffer_data(ctx, 0, &len);
        duk_push_this(ctx);
        duk_push_string(ctx, "usage");
        duk_get_prop(ctx, -2);
        int usage = duk_get_int(ctx, -1);
        duk_pop(ctx); // pop result
        duk_push_string(ctx, "__glvb");
        duk_get_prop(ctx, -2);
        GLVertexBuffer* pVb = (GLVertexBuffer*)duk_get_pointer(ctx, -1);
        duk_pop(ctx); // pop result 
        pVb->SetData(buf, len, usage);
        duk_pop(ctx); // pop this
        assert( duk_get_top(ctx) == n);
    }
    assert( duk_get_top(ctx) == n);
    return 0;
}

duk_ret_t duk_open_GLVertexBuffer(duk_context *ctx)
{
    const duk_function_list_entry my_module_funcs[] = {
        { "GetDeclaration", VertexBuffer_prototype_GetDeclaration, 0 },
        { "GetUsage", VertexBuffer_prototype_GetUsage, 0 },
        { "SetData", VertexBuffer_prototype_SetData, 1 },
        { NULL, NULL, 0 }
    };

    int top = duk_get_top(ctx);

    duk_push_global_object(ctx);
    duk_push_c_function(ctx, ConstructorVertexBuffer, 2 /*nargs*/);
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, my_module_funcs);
    duk_put_prop_string(ctx, -2, "prototype");
    duk_put_global_string(ctx, "GLVertexBuffer");
    duk_pop(ctx); /* pop global */

    // 断言堆栈正常
    assert( duk_get_top(ctx) == top );

    return 0;
}

static duk_ret_t ConstructorGLShader(duk_context *ctx)
{
    int n = duk_get_top(ctx);
    duk_push_this(ctx); 
    duk_push_pointer(ctx, new GLShader());
    duk_put_prop_string(ctx, -2, "__glshader");
    duk_pop(ctx);
    assert( duk_get_top(ctx) == n);
    return 0;
}

static duk_ret_t Shader_prototype_Create(duk_context *ctx)
{
    int n = duk_get_top(ctx);
    if (n != 2) {
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    const char* vs = duk_get_string(ctx, 0);
    const char* fs = duk_get_string(ctx, 1);

    duk_push_this(ctx); 
    duk_push_string(ctx, "__glshader");
    duk_get_prop(ctx, -2);
    GLShader* pShader = (GLShader*)duk_get_pointer(ctx, -1);
    duk_pop(ctx); // pop result
    duk_pop(ctx); // pop this
    if(!pShader->Create(vs, fs))
    {
        duk_push_false(ctx); 
    }else {
        duk_push_true(ctx);
    }
    assert( duk_get_top(ctx) == n + 1);
    return 1;
}

duk_ret_t duk_open_GLShader(duk_context *ctx)
{
    const duk_function_list_entry my_module_funcs[] = {
        { "Create", Shader_prototype_Create, 2 },
        /*{ "GetUsage", VertexBuffer_prototype_GetUsage, 0 },
        { "SetData", VertexBuffer_prototype_SetData, 1 },*/
        { NULL, NULL, 0 }
    };

    int top = duk_get_top(ctx);

    duk_push_global_object(ctx);
    duk_push_c_function(ctx, ConstructorGLShader, 0 /*nargs*/);
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, my_module_funcs);
    duk_put_prop_string(ctx, -2, "prototype");
    duk_put_global_string(ctx, "GLShader");
    duk_pop(ctx); /* pop global */

    // 断言堆栈正常
    assert( duk_get_top(ctx) == top );

    return 0;
}

duk_ret_t duk_open_GLRenderer(duk_context *ctx)
{
    const duk_function_list_entry my_module_funcs[] = {
        { "Clear", Renderer_prototype_Clear, DUK_VARARGS /* no args */ },
        { "SetVertexBuffer", Renderer_prototype_SetVertexBuffer, 1},
        { "SetIndexBuffer", Renderer_prototype_SetIndexBuffer, 1},
        { "SetShader", Renderer_prototype_SetShader, 1},
        { "DrawPrimitive", Renderer_prototype_DrawPrimitive, 4},
        { "CreateVertexBuffer", Renderer_prototype_CreateVertexBuffer, 2 /* 2 args */ },
        { "CreateShader", Renderer_prototype_CreateShader, 2 /* 2 args */ },
        { "Present", Renderer_prototype_Present, 0 /* 0 args */ },
        { NULL, NULL, 0 }
    };

    const duk_number_list_entry my_module_consts[] = {
      /*{ "FLAG_FOO", (double) (1 << 0) },
        { "FLAG_BAR", (double) (1 << 1) },
        { "FLAG_QUUX", (double) (1 << 2) },
        { "DELAY", 300.0 },*/
        { NULL, 0.0 }
    };

    int top = duk_get_top(ctx);

    duk_push_global_object(ctx);
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, my_module_funcs);
    duk_put_number_list(ctx, -1, my_module_consts);
    duk_put_global_string(ctx, "GLRenderer");
    duk_pop(ctx); /* pop global */

    // 断言堆栈正常
    assert( duk_get_top(ctx) == top );

    duk_open_GLVertexBuffer(ctx);
    duk_open_GLShader(ctx);

    return 0;
}