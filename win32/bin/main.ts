/// <reference path="platform.ts" />

//var fun = require('test');
//fun();

function CreateCamera(game)
{
    let camera = new Mind.Camera();
    game.camera = camera;
}

function UpdateCamera(game)
{
    var Input = Mind.Input;
    let camera = game.camera;
    if (input.IsKeyDown(Input.Scan.W) || input.IsKeyDown(Input.Scan.S)) {
        let moveSpeed;
        if (input.IsKeyDown(Input.Scan.W))
            moveSpeed = 1.5;
        else
            moveSpeed = -1.5;
            
        let forward = camera.GetForward();
        let pos = camera.GetPosition();
        forward.MulSelf(moveSpeed);
        pos.AddSelf(forward);
        camera.SetPosition(pos);
    }
    
    if (input.IsKeyDown(Input.Scan.A) || input.IsKeyDown(Input.Scan.D)) {
        let moveSpeed;
        if (input.IsKeyDown(Input.Scan.A))
            moveSpeed = 1.5;
        else
            moveSpeed = -1.5;
            
        let right = camera.GetRight();
        let pos = camera.GetPosition();
        right.MulSelf(moveSpeed);
        pos.AddSelf(right);
        camera.SetPosition(pos);
    }
    
    if (input.IsMouseDown(Input.MouseButton.Right)) {
        if (typeof game.oldMX === 'undefined') {
            let p = input.GetMousePosition();
            game.oldMX = p[0];
            game.oldMY = p[1];
        } 
        let p = input.GetMousePosition();
        game.dx = p[0] - game.oldMX;
        game.dy = p[1] - game.oldMY;

        let op = camera.GetPitch();
        let oh = camera.GetHead();
        
        oh += game.dx * 0.1;
        op += game.dy * 0.1;
       
        camera.SetPitch(op);
        camera.SetHead(oh);
        
        game.oldMX = p[0];
        game.oldMY = p[1];
    }else {
        game.oldMX = undefined;
        game.oldMY = undefined;
    }
    
    camera.Update();
}

function Test1(game)
{
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
    view.set([ 0.0,  100.0, 0.0, 1.0,1,0,0,//上顶点
              -100.0, -100.0, 0.0, 1.0,0,1,0,//左顶点
               100.0,  0.0, 0.0, 1.0,0,0,1]//右顶点
             ,0);
    
    vb.SetData(buffer);
    game.test1_vb = vb;
    
    let vsString = "attribute vec4 aPosition; " +
                   "attribute vec3 aVertexColor;" +
                   "uniform mat4 MVPMatrix;" +
                   "varying vec3 vVertexColor;" +
                   "void main(void){" +
                   "gl_Position = MVPMatrix * aPosition;" +
                   "vVertexColor = aVertexColor;" +
                   "}";
    let psString = "precision highp float; varying vec3 vVertexColor; void main(void) { gl_FragColor = vec4(vVertexColor, 1.0); }";
    let shader = renderer.CreateShader(vsString, psString);
    shader.OnBegin = function () : void {
        shader.SetMatrix4x4("MVPMatrix", (game.camera.GetProj().Multiply(game.camera.GetView())).Transpose().m);
    }
    
    game.test1_shader = shader;
}

function Test1Draw(game)
{
    renderer.SetVertexBuffer(game.test1_vb);
    renderer.SetShader(game.test1_shader);
    renderer.DrawPrimitive(Mind.IRenderer.Primitive.Triangles,1,3,0);
}

function Test2(game)
{
    AsyncReadFile("golemstatue.mdx", function(data) {
        let result = Mind.MdxParser(new Mind.BinaryReader(data));
        
        (function(u16Array)
        {
            var ib = renderer.CreateIndexBuffer(Mind.IndexBuffer.Usage.Static);
            ib.SetData(u16Array);
            
            game.test2_ib = ib;
        })(result.chunks.GEOS.elements[0].faces);
        
       (function(posFloatArray)
        {
            let mdxVbDecl = new Mind.VertexDeclaration();
            mdxVbDecl.AppendElement(new Mind.IVertexDeclaration.Element(Mind.IVertexDeclaration.Attribute.Position,
                                                            "aPosition",
                                                             Mind.IVertexDeclaration.DataType.Float3));
                                                             
            var vb = renderer.CreateVertexBuffer(mdxVbDecl, Mind.VertexBuffer.Usage.Static);
            vb.SetData(posFloatArray);
            
            game.test2_vb = vb;
        })(result.chunks.GEOS.elements[0].vertices);
        
        game.test2_model = result;
    });
    
    let vsString = "attribute vec3 aPosition; " +
                   "uniform mat4 MVPMatrix;" +
                   "void main(void){" +
                   "gl_Position = MVPMatrix * vec4(aPosition, 1.0);" +
                   "}";
    let psString = "precision highp float; void main(void) { gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }";
    let shader = renderer.CreateShader(vsString, psString);
    shader.OnBegin = function () : void {
        let rotX = new Mind.Matrix();
        rotX.RotationX(-90);
        let rotY = new Mind.Matrix();
        rotY.RotationY(-90);
        let modelMat = rotY.Multiply(rotX);
        let viewMat = game.camera.GetView().Multiply(modelMat);
        shader.SetMatrix4x4("MVPMatrix", (game.camera.GetProj().Multiply(viewMat)).Transpose().m);
    }
    
    game.test2_shader = shader;
}

function Test2Draw(game)
{
    if (typeof game.test2_model !== 'undefined') {
        renderer.SetVertexBuffer(game.test2_vb);
        renderer.SetIndexBuffer(game.test2_ib);
        renderer.SetShader(game.test2_shader);
        renderer.DrawIndexedPrimitive(Mind.IRenderer.Primitive.Triangles, 
            game.test2_model.chunks.GEOS.elements[0].faces.length / 3,
            game.test2_model.chunks.GEOS.elements[0].faces.length, 
            game.test2_model.chunks.GEOS.elements[0].vertices.length,
            0);
    }
}

game.onInit = function()
{
    GameLog('game.onInit');
    
    CreateCamera(this);
    
    Test1(this);
    
    Test2(this);
}

var clearColor = new Mind.Color(0, 0.141, 0.141, 1.0);

game.onFrame = function()
{
    UpdateCamera(this);
    
    renderer.Clear(clearColor, 1.0);
    
    Test1Draw(this);
    
    Test2Draw(this);
    
    renderer.Present();
}

game.onShut = function()
{
    GameLog('game.onShut');
}
