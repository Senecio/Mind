/// <reference path="Math.ts" />

namespace Mind {

// 颜色
export class Color {
    a: number;
    r: number;
    g: number;
    b: number;
    constructor(r: number, g: number, b: number, a?: number ) {
        this.a = a || 1.0;
        this.r = r;
        this.g = g;
        this.b = b;
    }
}

// 顶点声明
export interface IVertexDeclaration {
    GetNumberElement():number;
    GetElementsStride():number; 
    AppendElement(element : IVertexDeclaration.Element);
    GetIndexElement(index : number) : IVertexDeclaration.Element;
}

export module IVertexDeclaration {
    export enum DataType {
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
    }
    
    export enum DataTypeStride {
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
    }
    
    export const enum Attribute {
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
    }
    
    export class Element {
        attribute : IVertexDeclaration.Attribute;
        attributeName : string;
        dataType : IVertexDeclaration.DataType;
        ////////////////////////////////////////////////////////////////////////////////
        constructor(attribute : IVertexDeclaration.Attribute, attributeName : string, dataType : IVertexDeclaration.DataType) {
            this.attribute = attribute;
            this.attributeName = attributeName.slice();
            this.dataType = dataType;
        }
    }
}

// 顶点声明实现
export class VertexDeclaration implements IVertexDeclaration {
    private elements : Array<IVertexDeclaration.Element> = [];
    ////////////////////////////////////////////////////////////////////////////////
    constructor() { }
    
    GetNumberElement() {
        return this.elements.length;
    }
    GetElementsStride() {
        var stride = 0;
        for (let i = 0; i < this.elements.length; ++i) {
            let e = this.elements[i];
            stride += IVertexDeclaration.DataTypeStride[IVertexDeclaration.DataType[e.dataType]];
        }
        return stride;
    }
    AppendElement(element : IVertexDeclaration.Element) {
        this.elements.push(element);
    }
    GetIndexElement(index : number) : IVertexDeclaration.Element {
        if (this.elements.length > index) {
            return this.elements[index];
        }
        return null;
    }
}

// 顶点缓存
export class VertexBuffer {
    private declaration : IVertexDeclaration;
    private usage : VertexBuffer.Usage;
    public data : ArrayBuffer;
    ////////////////////////////////////////////////////////////////////////////////
    constructor (declaration : IVertexDeclaration, usage : VertexBuffer.Usage) {
        this.declaration = declaration;
        this.usage = usage;
    }
    
    GetDeclaration() : IVertexDeclaration {
        return this.declaration;
    }
    
    GetUsage() : VertexBuffer.Usage {
        return this.usage;
    }
    
    SetData(data : ArrayBuffer) {
        this.data = data.slice(0);
    }
}

export module VertexBuffer {
    export const enum Usage {
        None,
        Static,
        Dynamic,
    }
}

// 索引缓存
export class IndexBuffer {
    private usage : IndexBuffer.Usage;
    public data : ArrayBuffer;
    ////////////////////////////////////////////////////////////////////////////////
    constructor (usage : IndexBuffer.Usage) {
        this.usage = usage;
    }
    
    GetUsage() : IndexBuffer.Usage {
        return this.usage;
    }
    
    SetData(data : ArrayBuffer) {
        this.data = data.slice(0);
    }
}

export module IndexBuffer {
    export const enum Usage {
        None,
        Static,
        Dynamic,
    }
}


export interface IShader {
    
    ////////////////////////////////////////////////////////////////////////////////
    Create(vertexShader : string, fragmentShader : string) : boolean;
    SetInt(name : string, x : number) : boolean;
    SetIntArray(name : string, array : ArrayBuffer) : boolean;
    SetInt2(name : string, x : number, y : number) : boolean;
    SetInt2Array(name : string, array : ArrayBuffer) : boolean;
    SetInt3(name : string, x : number, y : number, z : number) : boolean;
    SetInt3Array(name : string, array : ArrayBuffer) : boolean;
    SetInt4(name : string, x : number, y : number, z : number, w : number) : boolean;
    SetInt4Array(name : string, array : ArrayBuffer) : boolean;
    SetFloat(name : string, x : number) : boolean;
    SetFloatArray(name : string, array : ArrayBuffer) : boolean;
    SetFloat2(name : string, x : number, y : number) : boolean;
    SetFloat2Array(name : string, array : ArrayBuffer) : boolean;
    SetFloat3(name : string, x : number, y : number, z : number) : boolean;
    SetFloat3Array(name : string, array : ArrayBuffer) : boolean;
    SetFloat4(name : string, x : number, y : number, z : number, w : number) : boolean;
    SetFloat4Array(name : string, array : ArrayBuffer) : boolean;
    SetMatrix2x2(name : string, array : ArrayBuffer) : boolean;
    SetMatrix3x3(name : string, array : ArrayBuffer) : boolean;
    SetMatrix4x4(name : string, array : ArrayBuffer) : boolean;
    SetSampler(name : string, index : number);
}

export interface IRenderer {

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
    
    // 设置顶点缓存
    SetVertexBuffer(vertexBuffer : VertexBuffer);
    // 设置索引缓存
    SetIndexBuffer(indexBuffer : IndexBuffer);
    // 设置着色器
    SetShader(shader : IShader);

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
    
    // 通过顶点缓存绘制图元
    DrawPrimitive(primitive : IRenderer.Primitive, primitveCount : number, verticesCount : number, offset : number);
    // 通过索引顶点缓存绘制图元
    DrawIndexedPrimitive(primitive : IRenderer.Primitive, primitveCount : number, indicesCount : number, verticesCount : number, offset : number);
    
    ////////////////////////////////////////////////////////////////////////////////
    // 工厂对象创建
    CreateVertexBuffer(declaration : IVertexDeclaration, usage : VertexBuffer.Usage) : VertexBuffer;
    CreateIndexBuffer(usage : IndexBuffer.Usage) : IndexBuffer;
    CreateShader(vertexShader : string, fragmentShader : string) : IShader;
}

export module IRenderer {
    export const enum Primitive {
        None,
        Points,
        Lines,
        LineStrip,
        LineLoop,
        Triangles,
        TriangleStrip,
        TriangleFan
    }
}

} // end Mind
