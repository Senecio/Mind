#include "GLRenderer.h"
#include <cstdio>
#include <cassert>


#if defined(_DEBUG)
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


GLVertexBuffer::GLVertexBuffer()
{
    glGenBuffers(1, &vbo_vertexbufferID);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &vbo_vertexbufferID);
}

void GLVertexBuffer::SetData( const void* data, uint32 length, int32 usage)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexbufferID);
    glBufferData(GL_ARRAY_BUFFER, length, data, (VertexBufferUsage::Enum)usage == VertexBufferUsage::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

GLIndexBuffer::GLIndexBuffer()
{
    glGenBuffers(1, &vbo_indexbufferID);
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &vbo_indexbufferID);
}

void GLIndexBuffer::SetData( const void* data, uint32 length, int32 usage )
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indexbufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, length, data, (IndexBufferUsage::Enum)usage == IndexBufferUsage::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

GLShader::GLShader()
{

}

GLShader::~GLShader()
{

}

bool GLShader::Create( const char* vs, const char* fs )
{
    GLint compileResult;
    int infoLogLength;
    const GLchar* shaderCodeCst;
    GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
    shaderCodeCst = vs;
    GL_ASSERT( glShaderSource(vsId, 1, &shaderCodeCst, NULL) );
    GL_ASSERT( glCompileShader(vsId) );
    // Check Vertex Shader
    GL_ASSERT( glGetShaderiv(vsId, GL_COMPILE_STATUS, &compileResult) );
    if(compileResult != GL_TRUE)
    {
        printf("Compile fail. compileResult=%d\n", compileResult);
        GL_ASSERT( glGetShaderiv(vsId, GL_INFO_LOG_LENGTH, &infoLogLength) );
        if ( infoLogLength > 0 ){
            GLchar* errorInfo = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(vsId, infoLogLength, NULL, errorInfo);
            printf("%s\n", errorInfo);
            delete [] errorInfo;
        }
        return false;
    }

    GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);
    shaderCodeCst = fs;
    GL_ASSERT( glShaderSource(fsId, 1, &shaderCodeCst, NULL) );
    GL_ASSERT( glCompileShader(fsId) );
    // Check Fragment Shader
    GL_ASSERT( glGetShaderiv(fsId, GL_COMPILE_STATUS, &compileResult) );
    if(compileResult != GL_TRUE)
    {
        printf("Compile fail. compileResult=%d\n", compileResult);
        GL_ASSERT( glGetShaderiv(fsId, GL_INFO_LOG_LENGTH, &infoLogLength) );
        if ( infoLogLength > 0 ){
            GLchar* errorInfo = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(fsId, infoLogLength, NULL, errorInfo);
            printf("%s\n", errorInfo);
            delete [] errorInfo;
        }
        return false;
    }

    GL_ASSERT( programId = glCreateProgram() );
    GL_ASSERT( glAttachShader(programId, vsId) );
    GL_ASSERT( glAttachShader(programId, fsId) );
    GL_ASSERT( glLinkProgram(programId) );
    // Check the program
    GLint linkResult;
    GL_ASSERT( glGetProgramiv(programId, GL_LINK_STATUS, &linkResult) );
    if(linkResult != GL_TRUE)
    {
        printf("_impl->programID=%d, linkResult=%d\n",programId, linkResult);
        GL_ASSERT( glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength) );
        if ( infoLogLength > 0 ){
            GLchar* errorInfo = new GLchar[infoLogLength + 1];
            GL_ASSERT( glGetProgramInfoLog(programId, infoLogLength, NULL, errorInfo) );
            printf("%s\n", errorInfo);
            delete [] errorInfo;
        }
        return false;
    }

    return true;
}

GLint GLShader::GetVertexAttributeLoaction( const char* attributeName )
{
    return glGetAttribLocation(programId, attributeName);
}


GLRenderer::GLRenderer(void)
{
}


GLRenderer::~GLRenderer(void)
{
}

void GLRenderer::SetClearColor( float r, float g, float b, float a )
{
    GL_ASSERT( glClearColor(r, g, b, a) );
}

void GLRenderer::SetClearDepth( float depth )
{
    GL_ASSERT( glClearDepth(depth) );
}

void GLRenderer::Clear(int renderClearFlag)
{
    GLbitfield mask = 0;
    if ((renderClearFlag & RenderClearFlag::Color) == RenderClearFlag::Color)
    {
        mask |= GL_COLOR_BUFFER_BIT;
    }

    if ((renderClearFlag & RenderClearFlag::Depth) == RenderClearFlag::Depth)
    {
        mask |= GL_DEPTH_BUFFER_BIT;
    }

    if (mask > 0) {
        glClear( mask );
    }
}


GLenum PrimitiveConvertToGL(RendererPrimitiveType::Enum type)
{
    switch (type)
    {
    case RendererPrimitiveType::None:
        return GL_NONE;
        break;
    case RendererPrimitiveType::Points:
        return GL_POINT;
        break;
    case RendererPrimitiveType::Lines:
        return GL_LINE;
        break;
    case RendererPrimitiveType::LineStrip:
        return GL_LINE_STRIP;
        break;
    case RendererPrimitiveType::LineLoop:
        return GL_LINE_LOOP;
        break;
    case RendererPrimitiveType::Triangles:
        return GL_TRIANGLES;
        break;
    case RendererPrimitiveType::TriangleStrip:
        return GL_TRIANGLE_STRIP;
        break;
    case RendererPrimitiveType::TriangleFan:
        return GL_TRIANGLE_FAN;
        break;
    default:
        break;
    }
    return GL_NONE;
}

bool VertexDeclarationTypeConvertToGL(VertexDeclaration::Element& element, GLint& byteSize, GLenum& glType, GLuint& elementStride)
{
    switch (element.dataType)
    {
    case VertexDeclarationDataType::Byte : { byteSize = 1, glType = GL_BYTE, elementStride = VertexDeclarationDataTypeStride::Byte; } break;
    case VertexDeclarationDataType::Byte2 : { byteSize = 2, glType = GL_BYTE, elementStride = VertexDeclarationDataTypeStride::Byte2; } break;
    case VertexDeclarationDataType::Byte3 : { byteSize = 3, glType = GL_BYTE, elementStride = VertexDeclarationDataTypeStride::Byte3; } break;
    case VertexDeclarationDataType::Byte4 : { byteSize = 4, glType = GL_BYTE, elementStride = VertexDeclarationDataTypeStride::Byte4; } break;
    case VertexDeclarationDataType::UByte : { byteSize = 1, glType = GL_UNSIGNED_BYTE, elementStride = VertexDeclarationDataTypeStride::UByte; } break;
    case VertexDeclarationDataType::UByte2 : { byteSize = 2, glType = GL_UNSIGNED_BYTE, elementStride = VertexDeclarationDataTypeStride::UByte2; } break;
    case VertexDeclarationDataType::UByte3 : { byteSize = 3, glType = GL_UNSIGNED_BYTE, elementStride = VertexDeclarationDataTypeStride::UByte3; } break;
    case VertexDeclarationDataType::UByte4 : { byteSize = 4, glType = GL_UNSIGNED_BYTE, elementStride = VertexDeclarationDataTypeStride::UByte4; } break;
    case VertexDeclarationDataType::Short : { byteSize = 1, glType = GL_SHORT, elementStride = VertexDeclarationDataTypeStride::Short; } break;
    case VertexDeclarationDataType::Short2 : { byteSize = 2, glType = GL_SHORT, elementStride = VertexDeclarationDataTypeStride::Short2; } break;
    case VertexDeclarationDataType::Short3 : { byteSize = 3, glType = GL_SHORT, elementStride = VertexDeclarationDataTypeStride::Short3; } break;
    case VertexDeclarationDataType::Short4 : { byteSize = 4, glType = GL_SHORT, elementStride = VertexDeclarationDataTypeStride::Short4; } break;
    case VertexDeclarationDataType::UShort : { byteSize = 1, glType = GL_UNSIGNED_SHORT, elementStride = VertexDeclarationDataTypeStride::UShort; } break;
    case VertexDeclarationDataType::UShort2 : { byteSize = 2, glType = GL_UNSIGNED_SHORT, elementStride = VertexDeclarationDataTypeStride::UShort2; } break;
    case VertexDeclarationDataType::UShort3 : { byteSize = 3, glType = GL_UNSIGNED_SHORT, elementStride = VertexDeclarationDataTypeStride::UShort3; } break;
    case VertexDeclarationDataType::UShort4 : { byteSize = 4, glType = GL_UNSIGNED_SHORT, elementStride = VertexDeclarationDataTypeStride::UShort4; } break;
    case VertexDeclarationDataType::Float : { byteSize = 1, glType = GL_FLOAT, elementStride = VertexDeclarationDataTypeStride::Float; } break;
    case VertexDeclarationDataType::Float2 : { byteSize = 2, glType = GL_FLOAT, elementStride = VertexDeclarationDataTypeStride::Float2; } break;
    case VertexDeclarationDataType::Float3 : { byteSize = 3, glType = GL_FLOAT, elementStride = VertexDeclarationDataTypeStride::Float3; } break;
    case VertexDeclarationDataType::Float4 : { byteSize = 4, glType = GL_FLOAT, elementStride = VertexDeclarationDataTypeStride::Float4; } break;
    default:
        return false;
    }
    return true;
}


void GLRenderer::DrawPrimitive( int primitiveType, int primitveCount, int verticesCount, int offset,GLVertexBuffer* pVB, GLShader* pShader, VertexDeclaration& vbDecl)
{

    GLenum glPrimitive = PrimitiveConvertToGL((RendererPrimitiveType::Enum)primitiveType);
    if (glPrimitive != GL_NONE) {

        if (pVB == NULL) {
            printf("not set vertexBuffer.");
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, pVB->vbo_vertexbufferID);

        if (pShader == NULL) {
            printf("not set shader.");
            return;
        }

        glUseProgram(pShader->programId);

        GLuint vaOffset = 0;
        int count = vbDecl.GetNumberElement();
        int stride = vbDecl.GetElementsStride();
        GLint byteSize;
        GLenum glType;
        GLuint elementStride;
        bool rs;
        for (int i = 0; i < count; ++i) {
            VertexDeclaration::Element* declElm = vbDecl.GetIndexElement(i);
            rs = VertexDeclarationTypeConvertToGL(*declElm, byteSize, glType, elementStride);
            GLint location = pShader->GetVertexAttributeLoaction(declElm->attributeName);
            if (rs && location != -1) {
                glEnableVertexAttribArray(location);
                glVertexAttribPointer(location, byteSize, glType, false, stride, (const void*)vaOffset);
            }
            
            if (rs) {
                vaOffset += elementStride;
            }
        }

        glDrawArrays(glPrimitive, offset, verticesCount);
    }
}

void GLRenderer::Present()
{
    glFlush();
    SwapBuffers(_hDC);
}
