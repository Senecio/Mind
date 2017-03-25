/// <reference path="../../engine/Renderer.ts" />
/// <reference path="platform.ts" />

declare function GLRendererPresent();

class GLVertexBuffer extends Mind.VertexBuffer {
    gl : WebGLRenderingContext;
    vb : WebGLBuffer;
    constructor (renderer : GLRenderer, declaration : Mind.IVertexDeclaration, usage : Mind.VertexBuffer.Usage) {
        super(declaration, usage);
        let gl = renderer.GetGLContext();
        this.vb = gl.createBuffer();
        this.gl = gl;
    }
    
    SetData(data) {
        if (typeof data.slice !== 'undefined') {
            this.data = data.slice();
        }
        else if (typeof data.buffer !== 'undefined' && typeof data.buffer.slice !== 'undefined') {
            this.data = data.buffer.slice();
        }
        else {
            GameLog("~~~~~~~~~~~~~~error~~~~~~~~~~~~~");
        }
        
        let gl = this.gl;
        gl.bindBuffer(gl.ARRAY_BUFFER, this.vb);
        gl.bufferData(gl.ARRAY_BUFFER, this.data, this.GetUsage() === Mind.VertexBuffer.Usage.Dynamic ? gl.DYNAMIC_DRAW : gl.STATIC_DRAW);
    }
}


class GLIndexBuffer extends Mind.IndexBuffer {
    gl : WebGLRenderingContext;
    ib : WebGLBuffer;
    constructor (renderer : GLRenderer, usage : Mind.IndexBuffer.Usage) {
        super(usage);
        let gl = renderer.GetGLContext();
        this.ib = gl.createBuffer();
        this.gl = gl;
    }
    
    SetData(data) {
        if (typeof data.slice !== 'undefined') {
            this.data = data.slice();
        }
        else if (typeof data.buffer !== 'undefined' && typeof data.buffer.slice !== 'undefined') {
            this.data = data.buffer.slice();
        }
        else {
            GameLog("~~~~~~~~~~~~~~error~~~~~~~~~~~~~");
        }
        
        let gl = this.gl;
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.ib);
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, this.data, this.GetUsage() === Mind.IndexBuffer.Usage.Dynamic ? gl.DYNAMIC_DRAW : gl.STATIC_DRAW);
    }
}

class GLShader implements Mind.IShader {
    gl : WebGLRenderingContext;
    //vs : WebGLShader;
    //fs : WebGLShader;
    program : WebGLProgram;
    locations : Object = {};
    
    constructor (renderer : GLRenderer) {
        let gl = renderer.GetGLContext();
        this.gl = gl;
    }
    
    Create(vertexShader : string, fragmentShader : string)
    {
        let gl = this.gl;
        let vs = gl.createShader(gl.VERTEX_SHADER);
        gl.shaderSource(vs, vertexShader);
        gl.compileShader(vs);
        if (!gl.getShaderParameter(vs, gl.COMPILE_STATUS)) {
            var info = gl.getShaderInfoLog( vs );
            GameLog(info);
            return false;
        }
        
        let fs = gl.createShader(gl.FRAGMENT_SHADER);
        gl.shaderSource(fs, fragmentShader);
        gl.compileShader(fs);
        if (!gl.getShaderParameter(fs, gl.COMPILE_STATUS)) {
            var info = gl.getShaderInfoLog( fs );
            GameLog(info);
            gl.deleteShader(vs);
            return false;
        }
        
        let program = gl.createProgram();
        gl.attachShader(program, vs);
        gl.attachShader(program, fs);            
        gl.linkProgram(program);
    
        if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
            GameLog("LINK_STATUS", gl.getProgramInfoLog(program));
            gl.deleteShader(vs);
            gl.deleteShader(fs);
            return false;
        }
        
        gl.deleteShader(vs);
        gl.deleteShader(fs);
        
        this.program = program;
        return true;
    }
    
    SetInt(name : string, x : number) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform1i(this.locations[name].location, x);
            return true;
        }
        return false;
    }
    SetIntArray(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            let gl = this.gl;
            gl.uniform1iv(this.locations[name].location, new Int32Array(array));
            return true;
        }
        return false;
    }
    SetInt2(name : string, x : number, y : number) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform2i(this.locations[name].location, x, y);
            return true;
        }
        return false;
    }
    SetInt2Array(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform2iv(this.locations[name].location, new Int32Array(array));
            return true;
        }
        return false;
    }
    SetInt3(name : string, x : number, y : number, z : number) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform3i(this.locations[name].location, x, y, z);
            return true;
        }
        return false;
    }
    SetInt3Array(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform3iv(this.locations[name].location, new Int32Array(array));
            return true;
        }
        return false;
    }
    SetInt4(name : string, x : number, y : number, z : number, w : number) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform4i(this.locations[name].location, x, y, z, w);
            return true;
        }
        return false;
    }
    SetInt4Array(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform4iv(this.locations[name].location, new Int32Array(array));
            return true;
        }
        return false;
    }
    SetFloat(name : string, x : number) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform1f(this.locations[name].location, x);
            return true;
        }
        return false;
    }
    SetFloatArray(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform1fv(this.locations[name].location, new Float32Array(array));
            return true;
        }
        return false;
    }
    SetFloat2(name : string, x : number, y : number) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform2f(this.locations[name].location, x, y);
            return true;
        }
        return false;
    }
    SetFloat2Array(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform2fv(this.locations[name].location, new Float32Array(array));
            return true;
        }
        return false;
    }
    SetFloat3(name : string, x : number, y : number, z : number) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform3f(this.locations[name].location, x, y, z);
            return true;
        }
        return false;
    }
    SetFloat3Array(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform3fv(this.locations[name].location, new Float32Array(array));
            return true;
        }
        return false;
    }
    SetFloat4(name : string, x : number, y : number, z : number, w : number) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform4f(this.locations[name].location, x, y, z, w);
            return true;
        }
        return false;
    }
    SetFloat4Array(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniform4fv(this.locations[name].location, new Float32Array(array));
            return true;
        }
        return false;
    }
    SetMatrix2x2(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniformMatrix2fv(this.locations[name].location, false, new Float32Array(array));
            return true;
        }
        return false;
    }
    SetMatrix3x3(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniformMatrix3fv(this.locations[name].location, false, new Float32Array(array));
            return true;
        }
        return false;
    }
    SetMatrix4x4(name : string, array : ArrayBuffer) {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getUniformLocation(program, name) };
        }
        
        if (this.locations[name].location !== -1) {
            gl.uniformMatrix4fv(this.locations[name].location, false, new Float32Array(array));
            return true;
        }
        return false;
    }
    SetSampler(name : string, index : number) {
        this.SetInt(name, index);
    }
    
    GetVertexAttributeLoaction(name : string) : number {
        let gl = this.gl;
        let program = this.program;
        if (typeof this.locations[name] === 'undefined') {
            this.locations[name] = { 'location' : gl.getAttribLocation(program, name) };
        }
        return  this.locations[name].location;
    }
    
    OnBegin() : void {}
    OnEnd() : void {}
}

class GLRenderer implements Mind.IRenderer {
    private glctx: WebGLRenderingContext;
    private bindVB : Mind.VertexBuffer;
    private bindIB : Mind.IndexBuffer;
    private bindShader : Mind.IShader;
    
    constructor() {
        this.glctx = opengl;
    }
    
    GetGLContext() {
        return this.glctx;
    }
    
    // 清除
    Clear(color?: Mind.Color, depth?: number)
    {
        let gl = this.glctx;
        if (gl) {
            if (color && depth) {
                gl.clearColor(color.r, color.g, color.b, color.a);
                gl.clearDepth(depth);
                gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
            }
            else if (color) {
                gl.clearColor(color.r, color.g, color.b, color.a);
                gl.clear(gl.COLOR_BUFFER_BIT);
            }
            else if (depth) {
                gl.clearDepth(depth);
                gl.clear(gl.DEPTH_BUFFER_BIT);
            }
        }
    }
    
    // 呈现 
    Present()
    {
        let gl = this.glctx;
        if (gl) {
            gl.flush();
            GLRendererPresent();
        }
    }
   
    SetViewPort(rect: Mind.Rect)
    {
    }
    
    GetViewPort(): Mind.Rect
    {
        return new Mind.Rect();
    }

    SetMatrix(type, matrix: Mind.Matrix)
    {
    }
    GetMatrix(type): Mind.Matrix
    {
        return new Mind.Matrix();
    }
    // 设置顶点缓存
    SetVertexBuffer(vertexBuffer : Mind.VertexBuffer)
    {
        this.bindVB = vertexBuffer;
    }
    // 设置索引缓存
    SetIndexBuffer(indexBuffer : Mind.IndexBuffer)
    {
        this.bindIB = indexBuffer;
    }
    // 设置着色器
    SetShader(shader : Mind.IShader)
    {
        this.bindShader = shader;
    }
    ClearPoints()
    {
    }
    AddPoint(position: Mind.Vec3, color: Mind.Color)
    {
    }
    DrawPoints()
    {
    }

    ClearLines()
    {
    }
    AddLine(point1: Mind.Vec3, point2: Mind.Vec3, color: Mind.Color)
    {
    }
    DrawLines()
    {
    }

    ClearSolidQuad()
    {
    }
    AddSolidQuad(p1: Mind.Vec3, p2: Mind.Vec3)
    {
    }
    DrawSolidQuad()
    {
    }

    ClearQuads()
    {
    }
    AddQuad(p1: Mind.Vec3, p2: Mind.Vec3, p3: Mind.Vec3, p4: Mind.Vec3)
    {
    }
    DrawQuads()
    {
    }
    
    private IVertexDeclarationTypeConvertToGL(gl : WebGLRenderingContext, element : Mind.IVertexDeclaration.Element)
    {
        let elemntStride = Mind.IVertexDeclaration.DataTypeStride[Mind.IVertexDeclaration.DataType[element.dataType]];
        
        switch (element.dataType) {
            case Mind.IVertexDeclaration.DataType.None : { return null; };
            case Mind.IVertexDeclaration.DataType.Byte : { return [1,gl.BYTE, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Byte2 : { return [2,gl.BYTE, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Byte3 : { return [3,gl.BYTE, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Byte4 : { return [4,gl.BYTE, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.UByte : { return [1,gl.UNSIGNED_BYTE, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.UByte2 : { return [2,gl.UNSIGNED_BYTE,elemntStride]; };
            case Mind.IVertexDeclaration.DataType.UByte3 : { return [3,gl.UNSIGNED_BYTE,elemntStride]; };
            case Mind.IVertexDeclaration.DataType.UByte4 : { return [4,gl.UNSIGNED_BYTE,elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Short : { return [1,gl.SHORT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Short2 : { return [2,gl.SHORT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Short3 : { return [3,gl.SHORT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Short4 : { return [4,gl.SHORT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.UShort : { return [1,gl.UNSIGNED_SHORT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.UShort2 : { return [2,gl.UNSIGNED_SHORT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.UShort3 : { return [3,gl.UNSIGNED_SHORT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.UShort4 : { return [4,gl.UNSIGNED_SHORT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Float : { return [1,gl.FLOAT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Float2 : { return [2,gl.FLOAT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Float3 : { return [3,gl.FLOAT, elemntStride]; };
            case Mind.IVertexDeclaration.DataType.Float4 : { return [4,gl.FLOAT, elemntStride]; };
        }
    }
    
    private PrimitiveConvertToGL(gl : WebGLRenderingContext, primitive : Mind.IRenderer.Primitive) : number
    {
        switch (primitive) {
            case Mind.IRenderer.Primitive.None : { return gl.NONE; };
            case Mind.IRenderer.Primitive.Points : { return gl.POINTS; };
            case Mind.IRenderer.Primitive.Lines : { return gl.LINES; };
            case Mind.IRenderer.Primitive.LineStrip : { return gl.LINE_STRIP; };
            case Mind.IRenderer.Primitive.LineLoop : { return gl.LINE_LOOP; };
            case Mind.IRenderer.Primitive.Triangles : { return gl.TRIANGLES; };
            case Mind.IRenderer.Primitive.TriangleStrip : { return gl.TRIANGLE_STRIP; };
            case Mind.IRenderer.Primitive.TriangleFan : { return gl.TRIANGLE_FAN; };
        }
    }
    
    // 通过顶点缓存绘制图元
    DrawPrimitive(primitive : Mind.IRenderer.Primitive, primitveCount : number, verticesCount : number, offset : number)
    {
        let gl = this.glctx;
        let glPrimitive = this.PrimitiveConvertToGL(gl, primitive);
        if (glPrimitive !== gl.NONE) {
            
            if (typeof this.bindVB === 'undefined' || this.bindVB === null) {
                GameLog("not set vertexBuffer.")
                return;
            }
            
            gl.bindBuffer(gl.ARRAY_BUFFER, (<GLVertexBuffer>this.bindVB).vb);
        
            
            if (typeof this.bindShader === 'undefined' || this.bindShader === null) {
                GameLog("not set shader.")
                return;
            }
            
            let shader = (<GLShader>this.bindShader);
            gl.useProgram(shader.program);
            
            let vbDecl = this.bindVB.GetDeclaration();
            let vaOffset = 0;
            let count = vbDecl.GetNumberElement();
            let stride = vbDecl.GetElementsStride();
            for (let i = 0; i < count; ++i) {
                let declElm = vbDecl.GetIndexElement(i);
                let data = this.IVertexDeclarationTypeConvertToGL(gl, declElm);
                let location = shader.GetVertexAttributeLoaction(declElm.attributeName);
                if (data && location !== -1) {
                    gl.enableVertexAttribArray(location);
                    gl.vertexAttribPointer(location, data[0], data[1], false, stride, vaOffset);
                }
                if (data) {
                    vaOffset += data[2];
                }
            }
            
            shader.OnBegin();
            gl.drawArrays(glPrimitive, offset, verticesCount);
            shader.OnEnd();
        }
    }
    // 通过索引顶点缓存绘制图元
    DrawIndexedPrimitive(primitive : Mind.IRenderer.Primitive, primitveCount : number, indicesCount : number, verticesCount : number, offset : number)
    {
        let gl = this.glctx;
        let glPrimitive = this.PrimitiveConvertToGL(gl, primitive);
        if (glPrimitive !== gl.NONE) {
        
            if (typeof this.bindVB === 'undefined' || this.bindVB === null) {
                GameLog("not set vertexBuffer.")
                return;
            }
            gl.bindBuffer(gl.ARRAY_BUFFER, (<GLVertexBuffer>this.bindVB).vb);
            
            if (typeof this.bindIB === 'undefined' || this.bindIB === null) {
                GameLog("not set indexBuffer.")
                return;
            }
            gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, (<GLIndexBuffer>this.bindIB).ib);
            
            if (typeof this.bindShader === 'undefined' || this.bindShader === null) {
                GameLog("not set shader.")
                return;
            }
            
            let shader = (<GLShader>this.bindShader);
            gl.useProgram(shader.program);
            
            let vbDecl = this.bindVB.GetDeclaration();
            let vaOffset = 0;
            let count = vbDecl.GetNumberElement();
            let stride = vbDecl.GetElementsStride();
            for (let i = 0; i < count; ++i) {
                let declElm = vbDecl.GetIndexElement(i);
                let data = this.IVertexDeclarationTypeConvertToGL(gl, declElm);
                let location = shader.GetVertexAttributeLoaction(declElm.attributeName);
                if (data && location !== -1) {
                    gl.enableVertexAttribArray(location);
                    gl.vertexAttribPointer(location, data[0], data[1], false, stride, vaOffset);
                }
                if (data) {
                    vaOffset += data[2];
                }
            }
        
            shader.OnBegin();
            gl.drawElements(glPrimitive, indicesCount, gl.UNSIGNED_SHORT, offset);
            shader.OnEnd();
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    // 工厂对象创建
    CreateVertexBuffer(declaration : Mind.IVertexDeclaration, usage : Mind.VertexBuffer.Usage) : Mind.VertexBuffer
    {
        return new GLVertexBuffer(this, declaration, usage);
    }
    CreateIndexBuffer(usage : Mind.IndexBuffer.Usage) : Mind.IndexBuffer
    {
        return new GLIndexBuffer(this, usage);
    }
    CreateShader(vertexShader : string, fragmentShader : string) : Mind.IShader
    {
        let shader = new GLShader(this);
        if(shader.Create(vertexShader, fragmentShader)) {
            return shader;
        }
        return null;
    }
}