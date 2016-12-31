/// <reference path="platform.ts" />

//var fun = require('test');
//fun();

game.onInit = function()
{
    GameLog('game.onInit');
    
    let vbDecl = new Mind.VertexDeclaration();
    vbDecl.AppendElement(new Mind.IVertexDeclaration.Element(Mind.IVertexDeclaration.Attribute.Position,
                                                            "aPosition",
                                                             Mind.IVertexDeclaration.DataType.Float4));
    vbDecl.AppendElement(new Mind.IVertexDeclaration.Element(Mind.IVertexDeclaration.Attribute.Color,
                                                            "aVertexColor",
                                                             Mind.IVertexDeclaration.DataType.Float3));
    let vb = renderer.CreateVertexBuffer(vbDecl, Mind.VertexBuffer.Usage.Static);
    let buffer = new ArrayBuffer(3*4*7);
    let view = new Float32Array(buffer);
    view.set([ 0.0,  1.0, 0.0, 1.0,1,0,0,//上顶点
              -1.0, -1.0, 0.0, 1.0,0,1,0,//左顶点
               1.0,  0.0, 0.0, 1.0,0,0,1]//右顶点
             ,0);
        
    vb.SetData(buffer);
    renderer.SetVertexBuffer(vb);
    
    let vsString = "attribute vec4 aPosition; " +
                   "attribute vec3 aVertexColor;" +
                   "varying vec3 vVertexColor;" +
                   "void main(void){" +
                   "gl_Position = aPosition;" +
                   "vVertexColor = aVertexColor;" +
                   "}";
    let psString = "precision highp float; varying vec3 vVertexColor; void main(void) { gl_FragColor = vec4(vVertexColor, 1.0); }";
    let shader = renderer.CreateShader(vsString, psString);
    renderer.SetShader(shader);
}

game.onFrame = function()
{
    renderer.Clear(new Mind.Color(0, 0.141, 0.141, 1.0), 1.0);
    renderer.DrawPrimitive(Mind.IRenderer.Primitive.Triangles,1,3,0);
    renderer.Present();
}

game.onShut = function()
{
    GameLog('game.onShut');
}
