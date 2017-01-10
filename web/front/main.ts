/// <reference path="platform.ts" />

//var fun = require('test');
//fun();

game.onInit = function()
{
    GameLog('game.onInit');
    
    let camera = new Mind.Camera();
    this.camera = camera;
    
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
    renderer.SetVertexBuffer(vb);
    
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
    renderer.SetShader(shader);
    
    shader.OnBegin = function () : void {
        //shader.SetMatrix4x4("MVPMatrix", (new Mind.Matrix()).m);
        shader.SetMatrix4x4("MVPMatrix", (camera.GetProj().Multiply(camera.GetView())).Transpose().m);
    }
    
    this.shader = shader;
}

game.onFrame = function()
{
    var Input = Mind.Input;
    let camera = this.camera;
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
        //GameLog(pos.x, pos.y, pos.z);
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
        //GameLog(pos.x, pos.y, pos.z);
    }
    
    if (input.IsMouseDown(Input.MouseButton.Right)) {
        if (typeof this.oldMX === 'undefined') {
            let p = input.GetMousePosition();
            this.oldMX = p[0];
            this.oldMY = p[1];
        } 
        let p = input.GetMousePosition();
        this.dx = p[0] - this.oldMX;
        this.dy = p[1] - this.oldMY;

        let op = camera.GetPitch();
        let or = camera.GetRoll();
        
        GameLog(op, or, camera.GetYaw());
        
        op += this.dx * 0.1;
        or += this.dy * 0.1;
        
        or = Mind.Clamp(-90, 90, or);
       
        camera.SetPitch(op%360);
        camera.SetRoll(or);
        
        this.oldMX = p[0];
        this.oldMY = p[1];
    }else {
        this.oldMX = undefined;
        this.oldMY = undefined;
    }
    
    this.camera.Update();

    renderer.Clear(new Mind.Color(0, 0.141, 0.141, 1.0), 1.0);
    renderer.DrawPrimitive(Mind.IRenderer.Primitive.Triangles,1,3,0);
    renderer.Present();
}

game.onShut = function()
{
    GameLog('game.onShut');
}
