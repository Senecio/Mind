#pragma once

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include "Common.h"
#include <gl/glew.h>
#include <gl/wglew.h>

class VertexDeclarationDataType {
public:
    enum Enum {
        None,
        Byte,
        Byte2,
        Byte3,
        Byte4,
        UByte,
        UByte2,
        UByte3,
        UByte4,
        Short,
        Short2,
        Short3,
        Short4,
        UShort,
        UShort2,
        UShort3,
        UShort4,
        Float,
        Float2,
        Float3,
        Float4
    };
};

class VertexDeclarationDataTypeStride {
public:
    enum Enum {
        None = 0,
        Byte = 1,
        Byte2 = 2,
        Byte3 = 3,
        Byte4 = 4,
        UByte = 1,
        UByte2 = 2,
        UByte3 = 3,
        UByte4 = 4,
        Short = 2,
        Short2 = 4,
        Short3 = 6,
        Short4 = 8,
        UShort = 2,
        UShort2 = 4,
        UShort3 = 6,
        UShort4 = 8,
        Float = 4,
        Float2 = 8,
        Float3 = 12,
        Float4 = 16
    };
};

class VertexDeclarationAttribute
{
public:
    enum Enum {
        None,
        Position,
        Position2,
        Position3,
        Position4,
        Normal,
        Normal2,
        Normal3,
        Normal4,
        Binormal,
        Binormal2,
        Binormal3,
        Binormal4,
        Tangent,
        Tangent2,
        Tangent3,
        Tangent4,
        Color,
        Color2,
        Color3,
        Color4,
        TextureCoord,
        TextureCoord2,
        TextureCoord3,
        TextureCoord4,
        TextureCoord5,
        TextureCoord6,
        TextureCoord7,
        TextureCoord8,
        BlendWeight,
        BlendWeight2,
        BlendWeight3,
        BlendWeight4,
        BlendIndex,
        BlendIndex2,
        BlendIndex3,
        BlendIndex4,
        PointSize,
        PointSize2,
        PointSize3,
        PointSize4,
        TransformedPosition
    };
};

class VertexDeclaration
{
public:
    struct Element {
        VertexDeclarationAttribute::Enum attribute;
        const char* attributeName;
        VertexDeclarationDataType::Enum dataType;

        Element(VertexDeclarationAttribute::Enum attribute, const char* attributeName, VertexDeclarationDataType::Enum dataType)
            :attribute(attribute),attributeName(attributeName),dataType(dataType)
        {
        }
    };

    std::vector<Element> elements;

    VertexDeclaration(){}

    uint32 GetNumberElement() {
        return elements.size();
    }

    uint32 GetElementsStride() {
        int stride = 0;
        for (size_t i = 0; i < elements.size(); ++i) {
            Element& e = elements[i];
            switch (e.dataType)
            {
            case VertexDeclarationDataType::None : { stride += VertexDeclarationDataTypeStride::None; } break;
            case VertexDeclarationDataType::Byte : { stride += VertexDeclarationDataTypeStride::Byte; } break;
            case VertexDeclarationDataType::Byte2 : { stride += VertexDeclarationDataTypeStride::Byte2; } break;
            case VertexDeclarationDataType::Byte3 : { stride += VertexDeclarationDataTypeStride::Byte3; } break;
            case VertexDeclarationDataType::Byte4 : { stride += VertexDeclarationDataTypeStride::Byte4; } break;
            case VertexDeclarationDataType::UByte : { stride += VertexDeclarationDataTypeStride::UByte; } break;
            case VertexDeclarationDataType::UByte2 : { stride += VertexDeclarationDataTypeStride::UByte2; } break;
            case VertexDeclarationDataType::UByte3 : { stride += VertexDeclarationDataTypeStride::UByte3; } break;
            case VertexDeclarationDataType::UByte4 : { stride += VertexDeclarationDataTypeStride::UByte4; } break;
            case VertexDeclarationDataType::Short : { stride += VertexDeclarationDataTypeStride::Short; } break;
            case VertexDeclarationDataType::Short2 : { stride += VertexDeclarationDataTypeStride::Short2; } break;
            case VertexDeclarationDataType::Short3 : { stride += VertexDeclarationDataTypeStride::Short3; } break;
            case VertexDeclarationDataType::Short4 : { stride += VertexDeclarationDataTypeStride::Short4; } break;
            case VertexDeclarationDataType::UShort : { stride += VertexDeclarationDataTypeStride::UShort; } break;
            case VertexDeclarationDataType::UShort2 : { stride += VertexDeclarationDataTypeStride::UShort2; } break;
            case VertexDeclarationDataType::UShort3 : { stride += VertexDeclarationDataTypeStride::UShort3; } break;
            case VertexDeclarationDataType::UShort4 : { stride += VertexDeclarationDataTypeStride::UShort4; } break;
            case VertexDeclarationDataType::Float : { stride += VertexDeclarationDataTypeStride::Float; } break;
            case VertexDeclarationDataType::Float2 : { stride += VertexDeclarationDataTypeStride::Float2; } break;
            case VertexDeclarationDataType::Float3 : { stride += VertexDeclarationDataTypeStride::Float3; } break;
            case VertexDeclarationDataType::Float4 : { stride += VertexDeclarationDataTypeStride::Float4; } break;
            default:
                break;
            }
        }
        return stride;
    }

    void AppendElement(Element element) {
        elements.push_back(element);
    }

    Element* GetIndexElement(uint32 index) {
        if (elements.size() > index) {
            return &elements[index];
        }
        return 0;
    }
};


class VertexBufferUsage {
public : 
    enum Enum {
        None,
        Static,
        Dynamic,
    };
};

class IndexBufferUsage {
public : 
    enum Enum {
        None,
        Static,
        Dynamic,
    };
};

class GLVertexBuffer
{
public:
    GLuint vbo_vertexbufferID;
public:
    GLVertexBuffer();
    ~GLVertexBuffer();
    void SetData(const void* data, uint32 length, int32 usage);
};

class GLIndexBuffer
{
public:
    GLuint vbo_indexbufferID;
public:
    GLIndexBuffer();
    ~GLIndexBuffer();
    void SetData(const void* data, uint32 length, int32 usage);
};

class GLShader 
{
public:
    GLuint programId;
public:
    GLShader();
    ~GLShader();
    bool Create(const char* vs, const char* fs);
    GLint GetVertexAttributeLoaction(const char* attributeName);
};

class RendererPrimitiveType
{
public:
    enum Enum{
        None,
        Points,
        Lines,
        LineStrip,
        LineLoop,
        Triangles,
        TriangleStrip,
        TriangleFan,
    };
};

class RenderClearFlag
{
public:
    enum Enum {
        None,
        Color,
        Depth = Color << 1,
    };
};

class GLRenderer
{
public:
    GLRenderer(void);
    ~GLRenderer(void);

    void SetClearColor(float r, float g, float b, float a);
    void SetClearDepth(float depth);
    void Clear(int renderClearFlag);
    void DrawPrimitive(int primitiveType, int primitveCount, int verticesCount, int offset,GLVertexBuffer* pVB, GLShader* pShader, VertexDeclaration& vbDecl);
    void Present();

    void        SetHDC(HDC hDC) { _hDC = hDC; }
    HDC         GetHDC() { return _hDC; }
private:
    HDC         _hDC;
};

