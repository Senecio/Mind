/// <reference path="Math.ts" />

namespace Mind {

export class Camera {
    
    private _needUpdateView : boolean;
    private _updateViewByAngle : boolean;
    private _needUpdateProj : boolean;
    
    private _position : Vec3 = new Vec3(Vec3.Zero);
    private _forward  : Vec3 = new Vec3(Vec3.Forward);
    private _up       : Vec3 = new Vec3(Vec3.Up);
    
    private _roll     : number = 0;
    private _pitch    : number = 0;
    private _head     : number = 0;
    
    private _viewMat  : Matrix = new Matrix();
    private _projMat  : Matrix = new Matrix();
    
    private _fov_y : number = 90.0;
    private _aspect : number = 4.0 / 3.0;
    private _zNear : number = 0.1;
    private _zFar : number = 1000;
    
    constructor() {
        
        this._needUpdateView = true;
        this._updateViewByAngle = false;
        this._needUpdateProj = true;

        this._position.Set(0.0, 100.0, 100.0);

        this._forward.Set(0.0, -100.0, -100.0);
        this._forward.NormalizeSelf();
        
        this.SetForward(this._forward);

        this.Update(0);
    }
    
    private _UpdateView() {
        if (this._updateViewByAngle || this._needUpdateView) {
            if (this._updateViewByAngle) {
                let mat = new Matrix();
                mat.FromEulerOrderZXY(this._roll, this._pitch, this._head);
                this._up.Copy(mat.GetColumn3(1));
                this._forward.Copy(mat.GetColumn3(2));
                //GameLog(this._roll, this._pitch, this._head);
                this._updateViewByAngle = false;
            }
            else {
                let mat = new Matrix();
                mat.SetColumn3(0, this.GetRight());
                mat.SetColumn3(1, this._up);
                mat.SetColumn3(2, this._forward);

                let a = mat.ToEulerOrderZXY();
                this._roll = a[0];
                this._pitch = a[1];
                this._head = a[2];
                GameLog(this._roll, this._pitch, this._head);
            }
            
            let at = this._position.Add(this._forward);
            this._viewMat.LookAtRH(this._position, at, Vec3.Up);
            this._needUpdateView = false;
        }
    }
    
    private _UpdateProj() {
        if (this._needUpdateProj === true) {
            this._projMat.PerspectiveRH(this._fov_y, this._aspect, this._zNear, this._zFar);
            this._needUpdateProj = false;
        }
    }
    
    SetLookAt(position : Vec3, focus : Vec3, upAxis : Vec3)
    {
        let forward = this._forward;
        let up = this._up;
        
        this._position.Copy(position);
        
        forward.Copy(focus.Sub(position));
        forward.NormalizeSelf();

        up.Copy(upAxis);
        up.NormalizeSelf();
        
        let right = new Vec3();
        right.Copy(up.Cross(forward));
        right.NormalizeSelf();

        up.Copy(forward.Cross(right));
        up.NormalizeSelf();

        this._needUpdateView = true;
        this._updateViewByAngle = false;
    }
    
    SetPosition(position : Vec3){
        this._position.Copy(position);
        
        this._needUpdateView = true;
    }
    
    GetPosition() : Vec3 { 
        return new Vec3(this._position); 
    }
    
    SetForward(vForward : Vec3){
        let forward = this._forward;
        let up = this._up;
        forward.Copy(vForward);

        let right = new Vec3();
        right.Copy(up.Cross(forward));
        right.NormalizeSelf();

        up.Copy(forward.Cross(right));
        up.NormalizeSelf();

        this._needUpdateView = true;
        this._updateViewByAngle = false;
    }
    
    
    GetForward() : Vec3 { 
        return new Vec3(this._forward);
    }
    
    GetRight() : Vec3 { 
        let forward = this._forward;
        let up = this._up;
        
        let right = new Vec3();
        right.Copy(up.Cross(forward));
        right.NormalizeSelf();
        return right;
    }
    
    SetUp(upAxis : Vec3){
        let forward = this._forward;
        let up = this._up;
        
        up.Copy(upAxis);
        up.NormalizeSelf();

        let right = new Vec3();
        right.Copy(up.Cross(forward));
        right.NormalizeSelf();

        
        forward.Copy(right.Cross(up));
        forward.NormalizeSelf();

        this._needUpdateView = true;
        this._updateViewByAngle = false;
    }
    
    GetUp() : Vec3 { 
        return this._up; 
    }
    
    SetRoll(roll : number) {
        this._roll = roll;
        
        this._needUpdateView = true;
        this._updateViewByAngle = true;
    }
    
    GetRoll() : number {
        return this._roll;
    }
    
    SetPitch(pitch : number) {
        pitch = Mind.Clamp(pitch, -89.5, 89.5);
        this._pitch = pitch;
        
        this._needUpdateView = true;
        this._updateViewByAngle = true;
    }
    
    GetPitch() : number {
        return this._pitch;
    }
    
    SetHead(head : number) {
        this._head = head;

        this._needUpdateView = true;
        this._updateViewByAngle = true;
    }
    
    GetHead() : number {
        return this._head;
    }
    
    SetProjection(fov_y : number, aspect : number, zNear : number, zFar : number){
    
        this._fov_y     = fov_y;
        this._aspect    = aspect;
        this._zNear     = zNear;
        this._zFar      = zFar;
    
        this._needUpdateProj = true;
    }
    
    SetFov(fov_y : number) {
        this._fov_y     = fov_y;
    
        this._needUpdateProj = true;
    }
    
    GetFov() {
        return this._fov_y;
    }
    
    SetAspect(aspect : number) {
        this._aspect    = aspect;
    
        this._needUpdateProj = true;
    }
    
    GetAspect() {
        return this._aspect;
    }
    
    SetClipNear(zNear : number) {
        this._zNear     = zNear;
    
        this._needUpdateProj = true;
    }
    
    GetClipNear() {
        return this._zNear;
    }
    
    SetClipFar(zFar : number) {
        this._zFar      = zFar;
    
        this._needUpdateProj = true;
    }
    
    GetClipFar() {
        return this._zFar;
    }
    
    GetView() : Matrix { 
        if(this._needUpdateView) { 
            this.Update(0);
        }
        return new Matrix(this._viewMat); 
    }
    
    GetProj() : Matrix { 
        if(this._needUpdateProj) { 
            this.Update(0); 
        } 
        return new Matrix(this._projMat);
    }
    
    Update(dt : number) {
        this._UpdateView();
        this._UpdateProj();
    }
}


} // end Mind