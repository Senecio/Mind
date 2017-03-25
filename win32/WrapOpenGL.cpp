#include "WrapOpenGL.h"

#include <gl/glew.h>
#include <gl/wglew.h>
#include <cassert>


#if defined(_DEBUG) && defined(_CHECK_GL_ERROR)
    #define GL_ASSERT(expression) do \
    { \
    expression; \
    GLenum error = glGetError(); \
    if(error != GL_NO_ERROR) \
        { \
        printf("call opengl's function has error. file(%s),func(%s),line(%d),error(%x)\n", __FILE__, __FUNCTION__, __LINE__, error); \
        assert(0); \
        } \
    }while(0)
#else
    #define GL_ASSERT(expression) expression
#endif

#define ErrorParameter  "error parameter type."
#define ErrorSyntax     "error syntax use."

inline bool DukGetNumberFormStack(duk_context *ctx, int idx, double& retVal)
{
    if (duk_is_number(ctx, idx))
    {
        retVal = duk_get_number(ctx, idx);
        return true;
    }
    return false;
}

inline bool DukGetFloatFormStack(duk_context *ctx, int idx, float& retVal)
{
    if (duk_is_number(ctx, idx))
    {
        retVal = (float)duk_get_number(ctx, idx);
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

inline bool DukGetUIntFormStack(duk_context *ctx, int idx, unsigned int& retVal)
{
    if (duk_is_number(ctx, idx))
    {
        retVal = duk_get_uint(ctx, idx);
        return true;
    }
    return false;
}


inline bool DukGetBoolFormStack(duk_context *ctx, int idx, bool& retVal)
{
    if (duk_is_boolean(ctx, idx))
    {
        retVal = duk_get_boolean(ctx, idx) != 0;
        return true;
    }
    return false;
}

inline bool DukGetStringFormStack(duk_context *ctx, int idx, const char** retVal)
{
    if (duk_is_string(ctx, idx))
    {
        *retVal = duk_get_string(ctx, idx);
        return true;
    }
    return false;
}


duk_ret_t GL_prototype_ClearColor(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 4){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    float r,g,b,a;
    if (DukGetFloatFormStack(ctx, 0, r) &&
        DukGetFloatFormStack(ctx, 1, g) &&
        DukGetFloatFormStack(ctx, 2, b) &&
        DukGetFloatFormStack(ctx, 3, a))
    {
        GL_ASSERT( glClearColor((GLclampf)r, (GLclampf)g, (GLclampf)b, (GLclampf)a) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }
    
    return 0;
}

duk_ret_t GL_prototype_ClearDepth(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    float depth;
    if (DukGetFloatFormStack(ctx, 0, depth))
    {
        GL_ASSERT( glClearDepth((GLclampf)depth) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Clear(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int bitmask;
    if (DukGetUIntFormStack(ctx, 0, bitmask))
    {
        GL_ASSERT( glClear((GLbitfield)bitmask) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_GenBuffers(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int count;
    if (DukGetUIntFormStack(ctx, 0, count))
    {
        GLuint* ids = new GLuint(count);
        GL_ASSERT( glGenBuffers(count, ids) );
        duk_idx_t arr_idx = duk_push_array(ctx);
        for (unsigned int i = 0; i < count; ++i)
        {
            duk_push_uint(ctx, ids[i]);
            duk_put_prop_index(ctx, arr_idx, i);
        }
        duk_pop(ctx);
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    assert( duk_get_top(ctx) == n + 1 );
    return 1;
}

duk_ret_t GL_prototype_DeleteBuffers(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    if (duk_is_array(ctx, 0))
    {
        duk_get_prop_string(ctx, 0, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);
        GLuint* ids = new GLuint(length);

        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 0, i);
            ids[i] = duk_get_uint(ctx, -1);
            duk_pop(ctx);
        }
        
        glDeleteBuffers(length, ids);
        delete [] ids;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    assert( duk_get_top(ctx) == n + 1 );

    return 1;
}


duk_ret_t GL_prototype_CreateBuffer(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    unsigned int id;
    GL_ASSERT( glGenBuffers(1, &id) );
    duk_push_uint(ctx, id);
    assert( duk_get_top(ctx) == n + 1 );

    return 1;
}

duk_ret_t GL_prototype_BindBuffer(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int target, id;
    if (DukGetUIntFormStack(ctx, 0, target) && 
        DukGetUIntFormStack(ctx, 1, id))
    {
        GL_ASSERT( glBindBuffer(target, id) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_BufferData(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 3){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int target, usage;
    if (duk_is_object(ctx, 1) && 
        DukGetUIntFormStack(ctx, 0, target) &&
        DukGetUIntFormStack(ctx, 2, usage))
    {
        duk_size_t len;
        float *buf;
        buf = (float *)duk_get_buffer_data(ctx, 1, &len);
        GL_ASSERT( glBufferData(target, len, (void*)buf, usage) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_CreateShader(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int type;
    if (DukGetUIntFormStack(ctx, 0, type))
    {
        GLuint id;
        GL_ASSERT( id = glCreateShader(type) );
        duk_push_uint(ctx, id);
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    assert( duk_get_top(ctx) == n + 1 );
    return 1;
}

duk_ret_t GL_prototype_DeleteShader(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int sahderId;
    if (DukGetUIntFormStack(ctx, 0, sahderId))
    {
        GL_ASSERT( glDeleteShader(sahderId) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    assert( duk_get_top(ctx) == n );
    return 0;
}

duk_ret_t GL_prototype_ShaderSource(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int shaderId;
    const char* shaderText;
    if (DukGetUIntFormStack(ctx, 0, shaderId) &&
        DukGetStringFormStack(ctx, 1, &shaderText))
    {
        GL_ASSERT( glShaderSource(shaderId, 1, &shaderText, 0) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_CompileShader(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int shaderId;
    if (DukGetUIntFormStack(ctx, 0, shaderId))
    {
         GL_ASSERT( glCompileShader(shaderId) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }
   
    return 0;
}

duk_ret_t GL_prototype_GetShaderiv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int shaderId, name;
    if (DukGetUIntFormStack(ctx, 0, shaderId) &&
        DukGetUIntFormStack(ctx, 1, name))
    {
        GLint result;
        GL_ASSERT( glGetShaderiv(shaderId, name, &result) );
        duk_push_int(ctx, result);
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }
    
    assert( duk_get_top(ctx) == n + 1 );
    return 1;
}

duk_ret_t GL_prototype_GetShaderInfoLog(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int shaderId;
    if (DukGetUIntFormStack(ctx, 0, shaderId))
    {
        GLint length;
        GL_ASSERT( glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length) );
        char* errorInfo = new char[length + 1];
        GL_ASSERT( glGetShaderInfoLog(shaderId, length, 0, errorInfo) );
        duk_push_string(ctx, errorInfo);
        delete [] errorInfo;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    assert( duk_get_top(ctx) == n + 1 );
    return 1;
}

duk_ret_t GL_prototype_CreateProgram(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    unsigned int id;
    GL_ASSERT( id = glCreateProgram() );
    duk_push_uint(ctx, id);
    assert( duk_get_top(ctx) == n + 1 );
    return 1;
}

duk_ret_t GL_prototype_AttachShader(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int programId, shaderId;
    if (DukGetUIntFormStack(ctx, 0, programId) &&
        DukGetUIntFormStack(ctx, 1, shaderId))
    {
        GL_ASSERT( glAttachShader(programId, shaderId) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }
    
    return 0;
}

duk_ret_t GL_prototype_LinkProgram(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int programId;
    if (DukGetUIntFormStack(ctx, 0, programId))
    {
        GL_ASSERT( glLinkProgram(programId) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_GetProgramiv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int programId, name;
    if (DukGetUIntFormStack(ctx, 0, programId) &&
        DukGetUIntFormStack(ctx, 1, name))
    {
        GLint result;
        GL_ASSERT( glGetProgramiv(programId, name, &result) );
        duk_push_int(ctx, result);
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    assert( duk_get_top(ctx) == n + 1 );
    return 1;
}

duk_ret_t GL_prototype_GetProgramInfoLog(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int programId;
    if (DukGetUIntFormStack(ctx, 0, programId))
    {
        GLint length;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
        char* errorInfo = new char[length + 1];
        glGetProgramInfoLog(programId, length, 0, errorInfo);
        duk_push_string(ctx, errorInfo);
        delete [] errorInfo;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    assert( duk_get_top(ctx) == n + 1 );
    return 1;
}

duk_ret_t GL_prototype_GetAttribLocation(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int programId;
    const char* name;
    if (DukGetUIntFormStack(ctx, 0, programId) &&
        DukGetStringFormStack(ctx, 1, &name))
    {
        int location;
        GL_ASSERT( location = glGetAttribLocation(programId, name) );
        duk_push_int(ctx, location);
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    assert( duk_get_top(ctx) == n + 1 );
    return 1;
}

duk_ret_t GL_prototype_GetUniformLocation(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int programId;
    const char* name;
    if (DukGetUIntFormStack(ctx, 0, programId) &&
        DukGetStringFormStack(ctx, 1, &name))
    {
        int location;
        GL_ASSERT( location = glGetUniformLocation(programId, name) );
        duk_push_int(ctx, location);
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    assert( duk_get_top(ctx) == n + 1 );
    return 1;
}

duk_ret_t GL_prototype_UseProgram(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int programId;
    if (DukGetUIntFormStack(ctx, 0, programId))
    {
        GL_ASSERT( glUseProgram(programId) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform1f(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    float x;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetFloatFormStack(ctx, 1, x))
    {
        GL_ASSERT( glUniform1f(location, x) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }
    
    return 0;
}

duk_ret_t GL_prototype_Uniform1fv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    if (DukGetIntFormStack(ctx, 0, location) &&
        duk_is_object(ctx, 1))
    {
        duk_get_prop_string(ctx, 1, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        float* array = new float[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 1, i);
            array[i] = (float)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }
        
        GL_ASSERT( glUniform1fv(location, length, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform1i(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    int x;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetIntFormStack(ctx, 1, x))
    {
        GL_ASSERT( glUniform1i(location, x) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform1iv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    if (DukGetIntFormStack(ctx, 0, location) &&
        duk_is_object(ctx, 1))
    {
        duk_get_prop_string(ctx, 1, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        int* array = new int[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 1, i);
            array[i] = (int)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }

        GL_ASSERT( glUniform1iv(location, length, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform2f(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 3){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    float x, y;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetFloatFormStack(ctx, 1, x) &&
        DukGetFloatFormStack(ctx, 2, y))
    {
        GL_ASSERT( glUniform2f(location, x, y) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform2fv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    if (DukGetIntFormStack(ctx, 0, location) &&
        duk_is_object(ctx, 1))
    {
        duk_get_prop_string(ctx, 1, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        float* array = new float[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 1, i);
            array[i] = (float)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }

        GL_ASSERT( glUniform2fv(location, length/2, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform2i(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 3){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    int x, y;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetIntFormStack(ctx, 1, x) &&
        DukGetIntFormStack(ctx, 2, y))
    {
        GL_ASSERT( glUniform2i(location, x, y) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}


duk_ret_t GL_prototype_Uniform2iv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    if (DukGetIntFormStack(ctx, 0, location) &&
        duk_is_object(ctx, 1))
    {
        duk_get_prop_string(ctx, 1, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        int* array = new int[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 1, i);
            array[i] = (int)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }

        GL_ASSERT( glUniform2iv(location, length/2, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}


duk_ret_t GL_prototype_Uniform3f(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 4){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    float x, y, z;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetFloatFormStack(ctx, 1, x) &&
        DukGetFloatFormStack(ctx, 2, y) && 
        DukGetFloatFormStack(ctx, 3, z))
    {
        GL_ASSERT( glUniform3f(location, x, y, z) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform3fv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    if (DukGetIntFormStack(ctx, 0, location) &&
        duk_is_object(ctx, 1))
    {
        duk_get_prop_string(ctx, 1, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        float* array = new float[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 1, i);
            array[i] = (float)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }

        GL_ASSERT( glUniform3fv(location, length/3, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform3i(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 4){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    int x, y, z;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetIntFormStack(ctx, 1, x) &&
        DukGetIntFormStack(ctx, 2, y) &&
        DukGetIntFormStack(ctx, 3, z))
    {
        GL_ASSERT( glUniform3i(location, x, y, z) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform3iv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    if (DukGetIntFormStack(ctx, 0, location) &&
        duk_is_object(ctx, 1))
    {
        duk_get_prop_string(ctx, 1, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        int* array = new int[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 1, i);
            array[i] = (int)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }

        GL_ASSERT( glUniform3iv(location, length/3, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform4f(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 5){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    float x, y, z, w;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetFloatFormStack(ctx, 1, x) &&
        DukGetFloatFormStack(ctx, 2, y) && 
        DukGetFloatFormStack(ctx, 3, z) &&
        DukGetFloatFormStack(ctx, 4, w))
    {
        GL_ASSERT( glUniform4f(location, x, y, z, w) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform4fv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    if (DukGetIntFormStack(ctx, 0, location) &&
        duk_is_object(ctx, 1))
    {
        duk_get_prop_string(ctx, 1, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        float* array = new float[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 1, i);
            array[i] = (float)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }

        GL_ASSERT( glUniform4fv(location, length/4, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform4i(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 5){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    int x, y, z, w;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetIntFormStack(ctx, 1, x) &&
        DukGetIntFormStack(ctx, 2, y) &&
        DukGetIntFormStack(ctx, 3, z) &&
        DukGetIntFormStack(ctx, 4, w))
    {
        GL_ASSERT( glUniform4i(location, x, y, z, w) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Uniform4iv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 2){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    if (DukGetIntFormStack(ctx, 0, location) &&
        duk_is_object(ctx, 1))
    {
        duk_get_prop_string(ctx, 1, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        int* array = new int[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 1, i);
            array[i] = (int)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }

        GL_ASSERT( glUniform4iv(location, length/4, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_UniformMatrix2fv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 3){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    bool transpose;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetBoolFormStack(ctx, 1, transpose) &&
        duk_is_object(ctx, 2))
    {
        duk_get_prop_string(ctx, 2, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        float* array = new float[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 2, i);
            array[i] = (float)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }

        GL_ASSERT( glUniformMatrix2fv(location, length/4, transpose, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_UniformMatrix3fv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 3){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    bool transpose;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetBoolFormStack(ctx, 1, transpose) &&
        duk_is_object(ctx, 2))
    {
        duk_get_prop_string(ctx, 2, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        float* array = new float[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 1, i);
            array[i] = (float)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }

        GL_ASSERT( glUniformMatrix3fv(location, length/9, transpose, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_UniformMatrix4fv(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 3){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    bool transpose;
    auto type = duk_get_type(ctx, 2);
    auto a = DUK_BUFOBJ_CREATE_ARRBUF;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetBoolFormStack(ctx, 1, transpose) &&
        duk_is_object(ctx, 2))
    {
        duk_get_prop_string(ctx, 2, "length");
        unsigned int length = duk_get_int(ctx, -1);
        duk_pop(ctx);

        float* array = new float[length];
        for (unsigned int i = 0; i < length; ++i)
        {
            duk_get_prop_index(ctx, 2, i);
            array[i] = (float)duk_get_number(ctx, -1);
            duk_pop(ctx);
        }

        GL_ASSERT( glUniformMatrix4fv(location, length/16, transpose, array) );
        delete [] array;
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_EnableVertexAttribArray(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    if (DukGetIntFormStack(ctx, 0, location))
    {
        GL_ASSERT( glEnableVertexAttribArray(location) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_DisableVertexAttribArray(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 1){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location;
    if (DukGetIntFormStack(ctx, 0, location))
    {
        GL_ASSERT( glDisableVertexAttribArray(location) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_VertexAttribPointer(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 6){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    int location, size;
    unsigned int dataType, stride, offset;
    bool normalized;
    if (DukGetIntFormStack(ctx, 0, location) &&
        DukGetIntFormStack(ctx, 1, size) && 
        DukGetUIntFormStack(ctx, 2, dataType) &&
        DukGetBoolFormStack(ctx, 3, normalized) &&
        DukGetUIntFormStack(ctx, 4, stride) &&
        DukGetUIntFormStack(ctx, 5, offset))
    {
        GL_ASSERT( glVertexAttribPointer(location, size, dataType, normalized, stride, (void*)offset) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_DrawArrays(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 3){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int mode, first, count;
    if (DukGetUIntFormStack(ctx, 0, mode) &&
        DukGetUIntFormStack(ctx, 1, first) && 
        DukGetUIntFormStack(ctx, 2, count))
    {
        GL_ASSERT( glDrawArrays(mode, first, count) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}


duk_ret_t GL_prototype_DrawElements(duk_context* ctx)
{
    int n = duk_get_top(ctx);  /* #args */
    if (n != 5){
        printf("%s %s\n",__FUNCTION__, ErrorSyntax);
        return 0;
    }

    unsigned int mode, count, type, offset;
    if (DukGetUIntFormStack(ctx, 0, mode) &&
        DukGetUIntFormStack(ctx, 1, count) && 
        DukGetUIntFormStack(ctx, 2, type) &&
        DukGetUIntFormStack(ctx, 3, offset))
    {
        GL_ASSERT( glDrawElements(mode, count, type, (void*)offset) );
    }
    else
    {
        printf("%s %s\n",__FUNCTION__, ErrorParameter);
        return 0;
    }

    return 0;
}

duk_ret_t GL_prototype_Flush(duk_context* ctx)
{
    glFlush();
    return 0;
}

duk_ret_t duk_open_OpenGL(duk_context *ctx)
{
    const duk_function_list_entry my_module_funcs[] = {
        { "clearColor", GL_prototype_ClearColor, 4 /* 4 args */ },
        { "clearDepth", GL_prototype_ClearDepth, 1},
        { "clear", GL_prototype_Clear, 1},
        { "genBuffers", GL_prototype_GenBuffers, 1},
        { "deleteBuffers", GL_prototype_DeleteBuffers, 1},
        { "createBuffer", GL_prototype_CreateBuffer, 0},
        { "bindBuffer", GL_prototype_BindBuffer, 2},
        { "bufferData", GL_prototype_BufferData, 3},
        { "createShader", GL_prototype_CreateShader, 1},
        { "deleteShader", GL_prototype_DeleteShader, 1},
        { "shaderSource", GL_prototype_ShaderSource, 2},
        { "compileShader", GL_prototype_CompileShader, 1},
        { "getShaderiv", GL_prototype_GetShaderiv, 2},
        { "getShaderParameter", GL_prototype_GetShaderiv, 2},
        { "getShaderInfoLog", GL_prototype_GetShaderInfoLog, 1},
        { "createProgram", GL_prototype_CreateProgram, 0},
        { "attachShader", GL_prototype_AttachShader, 2},
        { "linkProgram", GL_prototype_LinkProgram, 1},
        { "getProgramiv", GL_prototype_GetProgramiv, 2},
        { "getProgramParameter", GL_prototype_GetProgramiv, 2},
        { "getProgramInfoLog", GL_prototype_GetProgramInfoLog, 1},
        { "getAttribLocation", GL_prototype_GetAttribLocation, 2},
        { "getUniformLocation", GL_prototype_GetUniformLocation, 2 },
        { "useProgram", GL_prototype_UseProgram, 1},
        { "uniform1f", GL_prototype_Uniform1f, 2},
        { "uniform1fv", GL_prototype_Uniform1fv, 2},
        { "uniform1i", GL_prototype_Uniform1i, 2},
        { "uniform1iv", GL_prototype_Uniform1iv, 2},
        { "uniform2f", GL_prototype_Uniform2f, 3},
        { "uniform2fv", GL_prototype_Uniform2fv, 2},
        { "uniform2i", GL_prototype_Uniform2i, 3},
        { "uniform2i", GL_prototype_Uniform2iv, 2},
        { "uniform3f", GL_prototype_Uniform3f, 4},
        { "uniform3fv", GL_prototype_Uniform3fv, 2},
        { "uniform3i", GL_prototype_Uniform3i, 4},
        { "uniform3iv", GL_prototype_Uniform3iv, 2},
        { "uniform4f", GL_prototype_Uniform4f, 5},
        { "uniform4fv", GL_prototype_Uniform4fv, 2},
        { "uniform4i", GL_prototype_Uniform4i, 5},
        { "uniform4iv", GL_prototype_Uniform4iv, 2},
        { "uniformMatrix2fv", GL_prototype_UniformMatrix2fv, 3},
        { "uniformMatrix3fv", GL_prototype_UniformMatrix3fv, 3},
        { "uniformMatrix4fv", GL_prototype_UniformMatrix4fv, 3},
        { "enableVertexAttribArray", GL_prototype_EnableVertexAttribArray, 1},
        { "disableVertexAttribArray", GL_prototype_DisableVertexAttribArray, 1},
        { "vertexAttribPointer", GL_prototype_VertexAttribPointer, 6},
        { "drawArrays", GL_prototype_DrawArrays, 3},
        { "drawElements", GL_prototype_DrawElements, 5},
        { "flush", GL_prototype_Flush, 0},
        { NULL, NULL, 0 }
    };

    const duk_number_list_entry my_module_consts[] = {
        { "COLOR_BUFFER_BIT", (double) (GL_COLOR_BUFFER_BIT) },
        { "DEPTH_BUFFER_BIT", (double) (GL_DEPTH_BUFFER_BIT) },
        { "STENCIL_BUFFER_BIT", (double) (GL_STENCIL_BUFFER_BIT) },
        { "NONE", (double) (GL_NONE) },
        { "NO_ERROR", (double) (GL_NO_ERROR) },
        { "POINT", (double) (GL_POINT) },
        { "LINE", (double) (GL_LINE) },
        { "LINE_STRIP", (double) (GL_LINE_STRIP) },
        { "LINE_LOOP", (double) (GL_LINE_LOOP) },
        { "TRIANGLES", (double) (GL_TRIANGLES) },
        { "TRIANGLE_STRIP", (double) (GL_TRIANGLE_STRIP) },
        { "TRIANGLE_FAN", (double) (GL_TRIANGLE_FAN) },
        { "ARRAY_BUFFER", (double) (GL_ARRAY_BUFFER) },
        { "ELEMENT_ARRAY_BUFFER", (double) (GL_ELEMENT_ARRAY_BUFFER) },
        { "DYNAMIC_DRAW", (double) (GL_DYNAMIC_DRAW) },
        { "STATIC_DRAW", (double) (GL_STATIC_DRAW) },
        { "STREAM_DRAW", (double) (GL_STREAM_DRAW) },
        { "VERTEX_SHADER", (double) (GL_VERTEX_SHADER) },
        { "FRAGMENT_SHADER", (double) (GL_FRAGMENT_SHADER) },
        { "COMPILE_STATUS", (double) (GL_COMPILE_STATUS) },
        { "LINK_STATUS", (double) (GL_LINK_STATUS) },
        { "INFO_LOG_LENGTH", (double) (GL_INFO_LOG_LENGTH) },
        { "BYTE", (double) (GL_BYTE) },
        { "UNSIGNED_BYTE", (double) (GL_UNSIGNED_BYTE) },
        { "SHORT", (double) (GL_SHORT) },
        { "UNSIGNED_SHORT", (double) (GL_UNSIGNED_SHORT) },
        { "INT", (double) (GL_INT) },
        { "UNSIGNED_INT", (double) (GL_UNSIGNED_INT) },
        { "FLOAT", (double) (GL_FLOAT) },
        { "DOUBLE", (double) (GL_DOUBLE) },
        { NULL, 0.0 }
    };

    int top = duk_get_top(ctx);

    duk_push_global_object(ctx);
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, my_module_funcs);
    duk_put_number_list(ctx, -1, my_module_consts);
    duk_put_global_string(ctx, "opengl");
    duk_pop(ctx); /* pop global */

    // ¶ÏÑÔ¶ÑÕ»Õý³£
    assert( duk_get_top(ctx) == top );
    return 0;
}