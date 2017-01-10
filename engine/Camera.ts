/// <reference path="Math.ts" />

namespace Mind {

export class Camera {
    
    private _needUpdateView : boolean;
    private _updateViewByAngle : boolean;
    private _needUpdateProj : boolean;
    
    private _position : Vec3 = Vec3.Zero;
    private _forward  : Vec3 = Vec3.Forward;
    private _up       : Vec3 = Vec3.Up;
    
    private _roll     : number = 0;
    private _pitch    : number = 0;
    private _yaw      : number = 0;
    
    private _viewMat  : Matrix = new Matrix();
    private _projMat  : Matrix = new Matrix();
    
    private _fov_y : number = 90.0;
    private _aspect : number = 4.0 / 3.0;;
    private _zNear : number = 0.1;
    private _zFar : number = 1000;
    
    constructor() {
        
        this._needUpdateView = true;
        this._updateViewByAngle = false;
        this._needUpdateProj = true;

        this._position.Set(0.0, 100.0, 100.0);

        this._forward.Copy(this._position.Mul(-1));
        this._forward.NormalizeSelf();
        
        this.SetForward(this._forward);

        this.Update(0);
    }
    
    private _UpdateView() {
        if (this._updateViewByAngle || this._needUpdateView) {
            if (this._updateViewByAngle) {
                var q = new Quaternion(); 
                q.FromTaitBryanAngles(this._roll, this._pitch, this._yaw);

                this._up.Copy(q.GetColumn1());
                this._forward.Copy(q.GetColumn2());
                this._updateViewByAngle = false;
            }
            else {
                let mat = new Matrix();
                mat.SetColumn3(0, this.GetRight());
                mat.SetColumn3(1, this._up);
                mat.SetColumn3(2, this._forward);

                let q = new Quaternion();
                q.FromMatrix(mat);
                let a = q.ToTaitBryanAngles();
                this._roll = a[0];
                this._pitch = a[1];
                this._yaw = a[2];
                //GameLog(this.GetRight(), this._up, this._forward);
                //GameLog(a[0], a[1], a[2]);
            }
            
            let at = this._position.Add(this._forward);
            this._viewMat.LookAtRH(this._position, at, this._up);
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
        this._pitch = pitch;
        
        this._needUpdateView = true;
        this._updateViewByAngle = true;
    }
    
    GetPitch() : number {
        return this._pitch;
    }
    
    SetYaw(yaw : number) {
        this._yaw = yaw;

        this._needUpdateView = true;
        this._updateViewByAngle = true;
    }
    
    GetYaw() : number {
        return this._yaw;
    }

    SetRotation(rotation : Quaternion) {

    }
    
    GetRotation() : Quaternion {
        return null;
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
    
    SetAspect(aspect : number) {
        this._aspect    = aspect;
    
        this._needUpdateProj = true;
    }
    
    SetClipNear(zNear : number) {
        this._zNear     = zNear;
    
        this._needUpdateProj = true;
    }
    
    SetClipFar(zFar : number) {
        this._zFar      = zFar;
    
        this._needUpdateProj = true;
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