namespace Mind {

//> 定义图形引擎坐标系。
//> 图形引擎使用右手坐标系
//     +y
//     |_ +x
//    /
//   +z
//
//>正交基: right.cross(up) = forward
//>正交基: up.cross(forward) = right
//>正交基: forward.cross(right) = up
//>图形引擎使用的向量以列为主. 所以向量与矩阵相乘的规则为
//>Matrix * Vector 而不是 Vector * Matrix.

export const TWO_PI = Math.PI * 2;
export const PI = Math.PI;
export const HALF_PI = Math.PI * 0.5;
export const EPSILON = 0.00001;

export function FloatEqual(a : number, b : number, deviation? : number) : boolean
{
    deviation = (typeof deviation === 'undefined') ? EPSILON : deviation
    if (Math.abs(a - b) < deviation) return true;
    return false;
}

// 角度到弧度
export function DegreesToRadians(degrees : number) : number
{
    return degrees * (Mind.PI/180.0);
}

// 弧度到角度
export function RadiansToDegrees(radians : number) : number
{
    return radians / (Mind.PI/180.0);
}

// 映射某角度在-180和180度之间
export function Warp180(degrees : number) {
    degrees += 180;
    degrees -= Math.floor(degrees / 360.0) * 360;
    degrees -= 180;
    return degrees;
}

// 映射某弧度在-PI和PI弧之间
export function WarpPI(radians : number) {
    radians += Mind.PI;
    radians -= Math.floor(radians / TWO_PI) * TWO_PI;
    radians -= Mind.PI;
    return radians;
}

export function Clamp(x : number, a : number, b : number) : number 
{
    if (a > b) {
        let t = a;
        a = b;
        b = t;
    }
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

export class Rect {
    offsetX: number;
    offsetY: number;
    height: number;
    width: number;
}

export class Vec3 {
    x : number;
    y : number;
    z : number;
    
    static Up       = new Vec3(0, 1, 0);
    static Right    = new Vec3(1, 0, 0);
    static Forward  = new Vec3(0, 0, 1);
    static Down     = new Vec3(0, -1, 0);
    static Left     = new Vec3(-1, 0, 0);
    static Back     = new Vec3(0, 0, -1);
    static One      = new Vec3(1, 1, 1);
    static Zero     = new Vec3(0, 0, 0);
    
    constructor(x?: number, y?: number, z?: number)
    constructor(other : Vec3)
    constructor(xOrOther?: number | Vec3, y?: number, z?: number) {
        if (typeof xOrOther === 'undefined') {
            this.x = 0;
            this.y = 0;
            this.z = 0;
        }
        else if (typeof xOrOther === 'number') {
            this.x = xOrOther;
            this.y = y === undefined ? 0 : y;
            this.z = z === undefined ? 0 : z;
        }
        else {
            this.Copy(xOrOther);
        }
    }
    
    Copy(other : Vec3) {
        this.x = other.x;
        this.y = other.y;
        this.z = other.z;
        return this;
    }
    
    Set(x: number, y: number, z: number){
        this.x = x;
        this.y = y;
        this.z = z;
        return this;
    }
    
    Add(other : Vec3) {
        return new Vec3(this.x + other.x,
                        this.y + other.y,
                        this.z + other.z);
    }
    
    AddSelf(other : Vec3) {
        this.x += other.x;
        this.y += other.y;
        this.z += other.z;
        return this;
    }
    
    Sub(other : Vec3) {
        return new Vec3(this.x - other.x,
                        this.y - other.y,
                        this.z - other.z);
    }
    
    SubSelf(other : Vec3) {
        this.x -= other.x;
        this.y -= other.y;
        this.z -= other.z;
        return this;
    }
    
    Mul(scale : number) {
        return new Vec3(this.x * scale, this.y * scale, this.z * scale);
    }
    
    MulSelf(scale : number) {
        this.x *= scale;
        this.y *= scale;
        this.z *= scale;
        return this;
    }
    
    Div(scale : number) {
        if (scale === 0) return null;
        return new Vec3(this.x / scale, this.y / scale, this.z / scale);
    }
    
    DivSelf(scale : number) {
        if (scale === 0) return this;
        this.x /= scale;
        this.y /= scale;
        this.z /= scale;
        return this;
    }
    
    Length(){
        return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
    }
    
    Dot(other : Vec3) {
        return this.x * other.x + this.y * other.y + this.z * other.z;
    }
   
    Cross(other : Vec3) {
        return new Vec3(this.y * other.z - this.z * other.y,
                        this.z * other.x - this.x * other.z,
                        this.x * other.y - this.y * other.x);
    }
    
    Normalize()
    {
        let len = this.Length();
        if (len === 0) return null;
        if (len === 1.0) return new Vec3();
        return this.Mul(1.0 / len);
    }

    NormalizeSelf()
    {
        let len = this.Length();
        if (len === 0) return this;
        if (len === 1.0) return this;
        return this.MulSelf(1.0 / len);
    }
}

export class Matrix {
    m : Float32Array;
    constructor(mat?: Matrix) {
        if (mat) {
            this.Copy(mat);
        }
        else {
            this.m = new Float32Array(16);
            this.Identity();
        }
    }
    
    Copy(other : Matrix) {
        if (typeof other.m.slice !== 'undefined') {
            this.m = other.m.slice();
        } else {
            this.m = new Float32Array(16);
            for (let i = 0; i < 16; ++i)
                this.m[i] = other.m[i];
        }
    }
    
    // 单位化
    Identity () {
        for (let i = 0; i < 16; ++i)
            this.m[i] = 0;
            
        this.m[0] = 1;
        this.m[5] = 1;
        this.m[10] = 1;
        this.m[15] = 1;
        return this;
    }
    
    // 设置列
    SetColumn3(index : number, vector : Vec3) {
        if (index < 4) {
            let mat = this.m;
            mat[index] = vector.x;
            mat[4+index] = vector.y;
            mat[8+index] = vector.z;
        }
    }
    
    // 取得列
    GetColumn3(index : number) {
        if (index < 4) {
            let mat = this.m;
            return new Vec3(mat[index], mat[4+index], mat[8+index]);
        }

        return null;
    }
    
    // 矩阵转置
    Transpose()
    {
        let newMat = new Matrix(this);
        let mat1 = newMat.m, mat2 = this.m;
        for (let i = 0; i < 4; ++i)
            for(let j = 0; j < 4; ++j)
                mat2[i*4+j] = mat1[j*4+i];
            
        return this;
    }
    
    // 矩阵迹
    Trace() : number
    {
        let trace = 0;
        let mat4x4 = this.m;
        for (let i = 0; i < 4; ++i){
            trace += mat4x4[i*4+i];
        }
        return trace;
    }

    // 矩阵乘
    Multiply(other : Matrix) : Matrix
    {
        let mat1 = this.m, mat2 = other.m;
        let newMat = new Matrix();
        let mat3 = newMat.m;
        let row = 0;
        for (let i = 0; i < 4; ++i){
            for(let j = 0; j < 4; ++j){
                row = i * 4
                mat3[row+j] = mat1[row  ] * mat2[j  ] +
                              mat1[row+1] * mat2[4+j] +
                              mat1[row+2] * mat2[8+j] +
                              mat1[row+3] * mat2[12+j];
            }
        }
                     
        return newMat;
    }
    
    // 矩阵自我乘
    MultiplySelf(other : Matrix)
    {
        let newMat = new Matrix(this);
        let mat1 = newMat.m, mat2 = other.m, mat3 = this.m;
        let row = 0;
        for (let i = 0; i < 4; ++i){
            for(let j = 0; j < 4; ++j){
                row = i * 4
                mat3[row+j] = mat1[row  ] * mat2[j  ] +
                              mat1[row+1] * mat2[4+j] +
                              mat1[row+2] * mat2[8+j] +
                              mat1[row+3] * mat2[12+j];
            }
        }
                     
        return this;
    }
    
    // 角度为正时是以顺时针旋转
    RotationX(angle : number)
    {
        let rad = DegreesToRadians(angle);
        let cos = Math.cos(rad);
        let sin = Math.sin(rad);
        let mat = this.m;
        mat[0]  = 1.0; mat[1]  = 0.0; mat[2]  = 0.0; mat[3]  = 0.0;
        mat[4]  = 0.0; mat[5]  = cos; mat[6]  =-sin; mat[7]  = 0.0;
        mat[8]  = 0.0; mat[9]  = sin; mat[10] = cos; mat[11] = 0.0;
        mat[12] = 0.0; mat[13] = 0.0; mat[14] = 0.0; mat[15] = 1.0;
        return this;
    }

    RotationY(angle : number)
    {
        let rad = DegreesToRadians(angle);
        let cos = Math.cos(rad);
        let sin = Math.sin(rad);
        let mat = this.m;
        mat[0]  = cos; mat[1]  = 0.0; mat[2]  = sin; mat[3]  = 0.0;
        mat[4]  = 0.0; mat[5]  = 1.0; mat[6]  = 0.0; mat[7]  = 0.0;
        mat[8]  =-sin; mat[9]  = 0.0; mat[10] = cos; mat[11] = 0.0;
        mat[12] = 0.0; mat[13] = 0.0; mat[14] = 0.0; mat[15] = 1.0;
        return this;
    }

    RotationZ(angle : number)
    {
        let rad = DegreesToRadians(angle);
        let cos = Math.cos(rad);
        let sin = Math.sin(rad);
        let mat = this.m;
        mat[0]  = cos; mat[1]  =-sin; mat[2]  = 0.0; mat[3]  = 0.0;
        mat[4]  = sin; mat[5]  = cos; mat[6]  = 0.0; mat[7]  = 0.0;
        mat[8]  = 0.0; mat[9]  = 0.0; mat[10] = 1.0; mat[11] = 0.0;
        mat[12] = 0.0; mat[13] = 0.0; mat[14] = 0.0; mat[15] = 1.0;
        return this;
    }
    
    // 通过欧拉角转换
    // Roll : rotation about the Z-axis. Range(-PI/2, PI/2)
    // Pitch : rotation about the X-axis. Range(-PI, PI)
    // Head : rotation about the Y-axis. Range(-PI, PI)
    // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
    FromEulerOrderZXY(roll : number, pitch : number, head : number) {
        let x = DegreesToRadians(Mind.Clamp(pitch, -90, 90));
        let y = DegreesToRadians(head), z = DegreesToRadians(roll);
        let cb = Math.cos( x ), sb = Math.sin( x );
        let cc = Math.cos( y ), sc = Math.sin( y );
        let ca = Math.cos( z ), sa = Math.sin( z );
        
        let mat = this.m;
        
        mat[0] = cc*ca - sc*sb*sa;
        mat[1] = cc*-sa - sc*sb*ca;
        mat[2] = -sc*cb;

        mat[4] = cb*sa;
        mat[5] = cb*ca;
        mat[6] = -sb;

        mat[8] = sc*ca + cc*sb*sa;
        mat[9] = sc*-sa+cc*sb*ca;
        mat[10] = cc*cb;
     
        mat[3] = 0;
        mat[7] = 0;
        mat[11] = 0;

        mat[12] = 0;
        mat[13] = 0;
        mat[14] = 0;
        mat[15] = 1;
    }
    
    ToEulerOrderZXY() : Array<number> {
        let mat = this.m;
        let m11 = mat[0], m12 = mat[1], m13 = mat[2];
        let m21 = mat[4], m22 = mat[5], m23 = mat[6];
        let m31 = mat[8], m32 = mat[9], m33 = mat[10];
        
        let roll, pitch = Math.asin( Mind.Clamp( -m23, -1, 1 ) ), head;

        if ( Math.abs( m23 ) < 0.99999 ) {

            head = Math.atan2( -m13, m33 );
            roll = Math.atan2( m21, m22 );

        } else {

            head = 0;
            roll = Math.atan2( -m12, m11 );
        }
        return [RadiansToDegrees(roll), RadiansToDegrees(pitch), RadiansToDegrees(head)];
    }
    
    // 观察矩阵
    LookAtRH(eyePos : Vec3, lookAt : Vec3, up : Vec3) {
        let zaxis;
        let yaxis;
        let xaxis;

        zaxis = eyePos.Sub(lookAt);
        zaxis.NormalizeSelf();
        yaxis = up;
        yaxis.NormalizeSelf();
        xaxis = yaxis.Cross(zaxis);
        xaxis.NormalizeSelf();
        yaxis = zaxis.Cross(xaxis);
        yaxis.NormalizeSelf();

        let mat4x4 = this.m;

        mat4x4[0] = xaxis.x; mat4x4[1] = xaxis.y; mat4x4[2] = xaxis.z;
        mat4x4[4] = yaxis.x; mat4x4[5] = yaxis.y; mat4x4[6] = yaxis.z;
        mat4x4[8] = zaxis.x; mat4x4[9] = zaxis.y; mat4x4[10] = zaxis.z;

        mat4x4[3] = -xaxis.Dot(eyePos);
        mat4x4[7] = -yaxis.Dot(eyePos);
        mat4x4[11] = -zaxis.Dot(eyePos);

        mat4x4[12] = 0.0;
        mat4x4[13] = 0.0;
        mat4x4[14] = 0.0;
        mat4x4[15] = 1.0;
        
        return this;
    }
    
    // 投影矩阵
    PerspectiveRH(fov_y : number, aspect : number, zNear : number, zFar : number) {
        if (zNear > zFar) {
            let temp = zNear;
            zNear = zFar;
            zFar = temp;
        }

        let mat4x4 = this.m;
        for (let i = 0; i < 16; ++i)
            mat4x4[i] = 0;

        let n_f = 1.0 / (zNear - zFar);
        let yScale = 1.0/ Math.tan(DegreesToRadians(fov_y) * 0.5);
        let xScale = yScale / aspect;

        mat4x4[0] = xScale;
        mat4x4[5] = yScale;
        mat4x4[10] = (zFar + zNear) * n_f;
        mat4x4[14] = -1.0;
        mat4x4[11] = 2.0 * zFar * zNear * n_f;
        return this;
    }
}

export class Quaternion {
    m : Float32Array = new Float32Array(4);
    
    constructor(x? : number, y? : number, z? : number, w? : number)
    constructor(other : Quaternion)
    constructor(xOrQuat? : number | Quaternion, y? : number, z? : number, w? : number) {
        if (typeof xOrQuat === 'undefined' ) {
            this.Identity();
        }
        else if (typeof xOrQuat === 'number') {
            let m = this.m;
            m[0] = xOrQuat;
            m[1] = y === undefined ? 0 : y;
            m[2] = z === undefined ? 0 : z;
            m[3] = w === undefined ? 1 : w;
        }else {
            this.Copy(xOrQuat);
        }
    }
    
    Set(x : number, y : number, z : number, w : number) {
        let m = this.m;
        m[0] = x;
        m[1] = y;
        m[2] = z;
        m[3] = w;
    }
    
    Copy(other : Quaternion) {
        let m1 = this.m;
        let m2 = other.m;
        m1[0] = m2[0];
        m1[1] = m2[1];
        m1[2] = m2[2];
        m1[3] = m2[3];
    }
    
    Identity() {
        let m = this.m;
        m[0] = 0;
        m[1] = 0;
        m[2] = 0;
        m[3] = 1;
    }
    
    Length() : number
    {
        let m = this.m;
        let x = m[0], y = m[1], z = m[2], w = m[3];
        let len = x * x + y * y + z * z + w * w;
        return Math.sqrt(len);
    }
    
    Conjuqate() : Quaternion
    {
        let m = this.m;
        return new Quaternion(-m[0], -m[1], -m[2], m[3]);
    }
    
    ConjuqateSelf()
    {
        let m = this.m;
        m[0] = -m[0];
        m[1] = -m[1];
        m[2] = -m[2];
        
        return this;
    }
    
    Add(other : Quaternion) : Quaternion
    {
        let m1 = this.m;
        let m2 = other.m;
        let x = m1[0] + m2[0];
        let y = m1[1] + m2[1];
        let z = m1[2] + m2[2];
        let w = m1[3] + m2[3];
        
        return new Quaternion(x, y , z, w);
    }
    
    AddSelf(other : Quaternion)
    {
        let m1 = this.m;
        let m2 = other.m;
        
        m1[0] += m2[0];
        m1[1] += m2[1];
        m1[2] += m2[2];
        m1[3] += m2[3];
        
        return this;
    }
    
    Multiply(other : Quaternion)
    {
        let q1 = this.m, q2 = other.m;
        let quat = new Quaternion(0,0,0,0);
        let q3 = quat.m;
        
        q3[0] = q1[3]*q2[0] + q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1];
        q3[1] = q1[3]*q2[1] - q1[0]*q2[2] + q1[1]*q2[3] + q1[2]*q2[0];
        q3[2] = q1[3]*q2[2] + q1[0]*q2[1] - q1[1]*q2[0] + q1[2]*q2[3];
        q3[3] = q1[3]*q2[3] - q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2];

        return quat;
    }
    
    MultiplySelf(other : Quaternion)
    {
        let q3 = this.m;
        let quat = new Quaternion(q3[0],q3[1],q3[2],q3[3]);
        let q1 = quat.m, q2 = other.m;

        q3[0] = q1[3]*q2[0] + q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1];
        q3[1] = q1[3]*q2[1] - q1[0]*q2[2] + q1[1]*q2[3] + q1[2]*q2[0];
        q3[2] = q1[3]*q2[2] + q1[0]*q2[1] - q1[1]*q2[0] + q1[2]*q2[3];
        q3[3] = q1[3]*q2[3] - q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2];
        
        return this;
    }
    /*
    // 暂时未推导验证. 如果碰到需求,请先把欧拉角转换成矩阵.然后把矩阵转成四元素
    // 通过欧拉角转换
    // Roll : rotation about the Z-axis.
    // Pitch : rotation about the X-axis.
    // Head : rotation about the Y-axis.
    // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
    FromEulerOrderZXY(roll : number, pitch : number, head : number) {
        // z->x->y
        let q = this.m;
        let angle = DegreesToRadians(roll * 0.5);
        let c1 = Math.cos( angle );
        let s1 = Math.sin( angle );
        
        angle = DegreesToRadians( Mind.Clamp(pitch, -90, 90) * 0.5);
        let c2 = Math.cos( angle );
        let s2 = Math.sin( angle );
        
        angle = DegreesToRadians(head * 0.5);
        let c3 = Math.cos( angle );
        let s3 = Math.sin( angle );
        
        q[0] = s1 * c2 * c3 - c1 * s2 * s3;
        q[1] = c1 * s2 * c3 + s1 * c2 * s3;
        q[2] = c1 * c2 * s3 + s1 * s2 * c3;
        q[3] = c1 * c2 * c3 - s1 * s2 * s3;
    }
    
    // 暂时未推导验证. 如果碰到需求,请先把四元素转换成矩阵.然后把矩阵转成欧拉角
    // 转换成欧拉角
    ToEulerOrderZXY() : Array<number> {
        // z->x->y
        let q = this.m;
        let qx = q[0], qy = q[1], qz = q[2], qw = q[3];
        let sqx = qx * qx;
        let sqy = qy * qy;
        let sqz = qz * qz;
        let sqw = qw * qw;
        
        let roll = Math.asin(  Mind.Clamp( 2 * ( qx * qw + qy * qz ), - 1, 1 ) );
        let pitch = Math.atan2( 2 * ( qy * qw - qz * qx ), ( sqw - sqx - sqy + sqz ) );
        let head = Math.atan2( 2 * ( qz * qw - qx * qy ), ( sqw - sqx + sqy - sqz ) );
       
        return [RadiansToDegrees(roll), RadiansToDegrees(pitch), RadiansToDegrees(head)];
    }
    */
    
    // 通过矩阵转换
    FromMatrix(matrix : Matrix) {
        let temp = (new Quaternion()).m;
        let mat = matrix.m;
        let diag = mat[0] + mat[5] + mat[10] + 1;

        if( diag > 0.0 )
        {
            let scale = Math.sqrt(diag) * 2.0; // get scale from diagonal

            // speed this up
            temp[0] = (mat[9] - mat[6]) / scale;
            temp[1] = (mat[2] - mat[8]) / scale;
            temp[2] = (mat[4] - mat[1]) / scale;
            temp[3] = 0.25 * scale;
        }
        else
        {
            if (mat[0] > mat[5] && mat[0] > mat[10])
            {
                // 1st element of diag is greatest value
                // find scale according to 1st element, and double it
                let scale = Math.sqrt(1.0 + mat[0] - mat[5] - mat[10]) * 2.0;

                // speed this up
                temp[0] = 0.25 * scale;
                temp[1] = (mat[1] + mat[4]) / scale;
                temp[2] = (mat[8] + mat[2]) / scale;
                temp[3] = (mat[9] - mat[6]) / scale;
            }
            else if (mat[5] > mat[10])
            {
                // 2nd element of diag is greatest value
                // find scale according to 2nd element, and double it
                let scale = Math.sqrt(1.0 + mat[5] - mat[0] - mat[10]) * 2.0;

                // speed this up
                temp[0] = (mat[1] + mat[4]) / scale;
                temp[1] = 0.25 * scale;
                temp[2] = (mat[6] + mat[9]) / scale;
                temp[3] = (mat[2] - mat[8]) / scale;
            }
            else
            {
                // 3rd element of diag is greatest value
                // find scale according to 3rd element, and double it
                let scale = Math.sqrt(1.0 + mat[10] - mat[0] - mat[5]) * 2.0;

                // speed this up
                temp[0] = (mat[2] + mat[8]) / scale;
                temp[1] = (mat[6] + mat[9]) / scale;
                temp[2] = 0.25 * scale;
                temp[3] = (mat[4] - mat[1]) / scale;
            }
        }

        return this.Set(temp[0], temp[1], temp[2], temp[3]);
    }
    
    // 转换到矩阵
    ToMatrix() : Matrix
    {
        let q = this.m;
        let x = q[0], y = q[1], z = q[2], w = q[3];
    
        let s = 2.0;
        let newMat = new Matrix();
        let mat4x4 = newMat.m;
        mat4x4[0] = 1.0 - s*(y*y + z*z);    mat4x4[1] = s*(x*y - w*z);          mat4x4[2] = s*(w*y + x*z);         mat4x4[3] = 0.0;
        mat4x4[4] = s*(x*y + w*z),          mat4x4[5] = 1.0 - s*(x*x + z*z);    mat4x4[6] = s*(y*z - w*x);         mat4x4[7] = 0.0;
        mat4x4[8] = s*(x*z - w*y);          mat4x4[9] = s*(y*z + w*x);          mat4x4[10] = 1.0 - s*(x*x + y*y);  mat4x4[11] = 0.0;
        mat4x4[12] = 0.0;                   mat4x4[13] = 0.0;                   mat4x4[14] = 0.0;                  mat4x4[15] = 1.0;
    
        return newMat;
    }
    
    GetColumn0() : Vec3 {
        let q = this.m;
        let x = q[0], y = q[1], z = q[2], w = q[3];
        return new Vec3(1.0 - 2.0*(y*y + z*z), 2.0*(x*y + w*z), 2.0*(x*z - w*y)); 
    }
    GetColumn1() : Vec3 {
        let q = this.m;
        let x = q[0], y = q[1], z = q[2], w = q[3];
        return new Vec3(2.0*(x*y - w*z), 1.0 - 2.0*(x*x + z*z), 2.0*(y*z + w*x));
    }
    GetColumn2() : Vec3 {
        let q = this.m;
        let x = q[0], y = q[1], z = q[2], w = q[3];
        return new Vec3(2.0*(x*z + w*y), 2.0*(y*z - w*x), 1.0 - 2.0*(x*x + y*y));
    }

    GetRow0() : Vec3 {
        let q = this.m;
        let x = q[0], y = q[1], z = q[2], w = q[3];
        return new Vec3(1.0 - 2.0*(y*y + z*z), 2.0*(x*y - w*z), 2.0*(w*y + x*z));
    }
    
    GetRow1() : Vec3 {
        let q = this.m;
        let x = q[0], y = q[1], z = q[2], w = q[3];
        return new Vec3(2.0*(x*y + w*z), 1.0 - 2.0*(x*x + z*z), 2.0*(y*z - w*x));
    }
    
    GetRow2() : Vec3{
        let q = this.m;
        let x = q[0], y = q[1], z = q[2], w = q[3];
        return new Vec3(2.0*(x*z - w*y), 2.0*(y*z + w*x), 1.0 - 2.0*(x*x + y*y)); 
    }
    
    Slerp(other, t) : Quaternion
    {
        let q1 = this.m, q2 = other.m;
        let newQuat = new Quaternion(0,0,0,0);
        let q3 = newQuat.m;
        if(t === 0.0){
            q3[0] = q1[0];
            q3[1] = q1[1];
            q3[2] = q1[2];
            q3[3] = q1[3];
            return newQuat;
        }else if(t === 1.0){
            q3[0] = q2[0];
            q3[1] = q2[1];
            q3[2] = q2[2];
            q3[3] = q2[3];
            return newQuat;
        }

        
        let p = q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3];
        if(Math.abs(p) >= 1.0){
            q3[0] = q2[0];
            q3[1] = q2[1];
            q3[2] = q2[2];
            q3[3] = q2[3];
            return newQuat;
        }
        
        if(p < 0.0){
            q3[0] = -q2[0];
            q3[1] = -q2[1];
            q3[2] = -q2[2];
            q3[3] = -q2[3];
            p = -p;
        }else{
            q3[0] = q2[0];
            q3[1] = q2[1];
            q3[2] = q2[2];
            q3[3] = q2[3];
        }
        
        let delta = Math.acos(p)
        let sinDelta = Math.sqrt(1.0 - p*p);
        
        let r1 = Math.sin((1 - t) * delta) / sinDelta;
        let r2 = Math.sin(t * delta) / sinDelta;
        q3[0] = (q1[0] * r1 + q3[0] * r2);
        q3[1] = (q1[1] * r1 + q3[1] * r2);
        q3[2] = (q1[2] * r1 + q3[2] * r2);
        q3[3] = (q1[3] * r1 + q3[3] * r2);
        return newQuat;
    }
    
    Rotate( position : Vec3 )
    {
        if(1)
        {
            // normal method
            let q1 = this, 
                q2 = new Quaternion(position.x, position.y, position.z, 0), 
                q3 = new Quaternion(this.Conjuqate());

            let result = q1.Multiply(q2);
            result.MultiplySelf(q3);            
            return new Vec3(
                    result.m[0],
                    result.m[1],
                    result.m[2]);
        }
        else {
            // copy form irrlicht.
            // nVidia SDK implementation
            
            let m = this.m;
            let qvec = new Vec3(m[0], m[1], m[2]);

            let uv = qvec.Cross(position);
            let uuv = qvec.Cross(uv);
            let halfW = 2.0 * m[3];

            return new Vec3(
                    position.x + uv.x * halfW + uuv.x * 2.0,
                    position.y + uv.y * halfW + uuv.y * 2.0,
                    position.z + uv.z * halfW + uuv.z * 2.0)
        }
    }
    
}

} // end Mind