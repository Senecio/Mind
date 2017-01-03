var Mind;
(function (Mind) {
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
    Mind.TWO_PI = Math.PI * 2;
    Mind.PI = Math.PI;
    Mind.HALF_PI = Math.PI * 0.5;
    // 角度到弧度
    function DegreesToRadians(degrees) {
        return degrees * (Mind.PI / 180.0);
    }
    Mind.DegreesToRadians = DegreesToRadians;
    // 弧度到角度
    function RadiansToDegrees(radians) {
        return radians / (Mind.PI / 180.0);
    }
    Mind.RadiansToDegrees = RadiansToDegrees;
    var Rect = (function () {
        function Rect() {
        }
        return Rect;
    }());
    Mind.Rect = Rect;
    var Vec3 = (function () {
        function Vec3(xOrOther, y, z) {
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
        Vec3.prototype.Copy = function (other) {
            this.x = other.x;
            this.y = other.y;
            this.z = other.z;
            return this;
        };
        Vec3.prototype.Set = function (x, y, z) {
            this.x = x;
            this.y = y;
            this.z = z;
            return this;
        };
        Vec3.prototype.Add = function (other) {
            return new Vec3(this.x + other.x, this.y + other.y, this.z + other.z);
        };
        Vec3.prototype.AddSelf = function (other) {
            this.x += other.x;
            this.y += other.y;
            this.z += other.z;
            return this;
        };
        Vec3.prototype.Sub = function (other) {
            return new Vec3(this.x - other.x, this.y - other.y, this.z - other.z);
        };
        Vec3.prototype.SubSelf = function (other) {
            this.x -= other.x;
            this.y -= other.y;
            this.z -= other.z;
            return this;
        };
        Vec3.prototype.Mul = function (scale) {
            return new Vec3(this.x * scale, this.y * scale, this.z * scale);
        };
        Vec3.prototype.MulSelf = function (scale) {
            this.x *= scale;
            this.y *= scale;
            this.z *= scale;
            return this;
        };
        Vec3.prototype.Div = function (scale) {
            if (scale === 0)
                return null;
            return new Vec3(this.x / scale, this.y / scale, this.z / scale);
        };
        Vec3.prototype.DivSelf = function (scale) {
            if (scale === 0)
                return this;
            this.x /= scale;
            this.y /= scale;
            this.z /= scale;
            return this;
        };
        Vec3.prototype.Length = function () {
            return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
        };
        Vec3.prototype.Dot = function (other) {
            return this.x * other.x + this.y * other.y + this.z * other.z;
        };
        Vec3.prototype.Cross = function (other) {
            return new Vec3(this.y * other.z - this.z * other.y, this.z * other.x - this.x * other.z, this.x * other.y - this.y * other.x);
        };
        Vec3.prototype.Normalize = function () {
            var len = this.Length();
            if (len === 0)
                return null;
            if (len === 1.0)
                return new Vec3();
            return this.Mul(1.0 / len);
        };
        Vec3.prototype.NormalizeSelf = function () {
            var len = this.Length();
            if (len === 0)
                return this;
            if (len === 1.0)
                return this;
            return this.MulSelf(1.0 / len);
        };
        return Vec3;
    }());
    Vec3.Up = new Vec3(0, 1, 0);
    Vec3.Right = new Vec3(1, 0, 0);
    Vec3.Forward = new Vec3(0, 0, 1);
    Vec3.Down = new Vec3(0, -1, 0);
    Vec3.Left = new Vec3(-1, 0, 0);
    Vec3.Back = new Vec3(0, 0, -1);
    Vec3.One = new Vec3(1, 1, 1);
    Vec3.Zero = new Vec3(0, 0, 0);
    Mind.Vec3 = Vec3;
    var Matrix = (function () {
        function Matrix(mat) {
            if (mat) {
                this.m = mat.m.slice();
            }
            else {
                this.m = new Float32Array(16);
                this.Identity();
            }
        }
        // 单位化
        Matrix.prototype.Identity = function () {
            for (var i = 0; i < 16; ++i)
                this.m[i] = 0;
            this.m[0] = 1;
            this.m[5] = 1;
            this.m[10] = 1;
            this.m[15] = 1;
            return this;
        };
        // 矩阵转置
        Matrix.prototype.Transpose = function (mat4x4) {
            var newMat = new Matrix(this);
            var mat1 = newMat.m, mat2 = this.m;
            for (var i = 0; i < 4; ++i)
                for (var j = 0; j < 4; ++j)
                    mat2[i * 4 + j] = mat1[j * 4 + i];
            return this;
        };
        // 矩阵迹
        Matrix.prototype.Trace = function () {
            var trace = 0;
            var mat4x4 = this.m;
            for (var i = 0; i < 4; ++i) {
                trace += mat4x4[i * 4 + i];
            }
            return trace;
        };
        // 矩阵乘
        Matrix.prototype.Multiply = function (other) {
            var mat1 = this.m, mat2 = other.m;
            var newMat = new Matrix();
            var mat3 = newMat.m;
            var row = 0;
            for (var i = 0; i < 4; ++i) {
                for (var j = 0; j < 4; ++j) {
                    row = i * 4;
                    mat3[row + j] = mat1[row] * mat2[j] +
                        mat1[row + 1] * mat2[4 + j] +
                        mat1[row + 2] * mat2[8 + j] +
                        mat1[row + 3] * mat2[12 + j];
                }
            }
            return newMat;
        };
        // 矩阵自我乘
        Matrix.prototype.MultiplySelf = function (other) {
            var newMat = new Matrix(this);
            var mat1 = newMat.m, mat2 = other.m, mat3 = this.m;
            var row = 0;
            for (var i = 0; i < 4; ++i) {
                for (var j = 0; j < 4; ++j) {
                    row = i * 4;
                    mat3[row + j] = mat1[row] * mat2[j] +
                        mat1[row + 1] * mat2[4 + j] +
                        mat1[row + 2] * mat2[8 + j] +
                        mat1[row + 3] * mat2[12 + j];
                }
            }
            return this;
        };
        Matrix.prototype.RotationX = function (angle) {
            var rad = DegreesToRadians(angle);
            var cos = Math.cos(rad);
            var sin = Math.sin(rad);
            var mat = this.m;
            mat[0] = 1.0;
            mat[1] = 0.0;
            mat[2] = 0.0;
            mat[3] = 0.0;
            mat[4] = 0.0;
            mat[5] = cos;
            mat[6] = -sin;
            mat[7] = 0.0;
            mat[8] = 0.0;
            mat[9] = sin;
            mat[10] = cos;
            mat[11] = 0.0;
            mat[12] = 0.0;
            mat[13] = 0.0;
            mat[14] = 0.0;
            mat[15] = 1.0;
            return this;
        };
        Matrix.prototype.RotationY = function (angle) {
            var rad = DegreesToRadians(angle);
            var cos = Math.cos(rad);
            var sin = Math.sin(rad);
            var mat = this.m;
            mat[0] = cos;
            mat[1] = 0.0;
            mat[2] = sin;
            mat[3] = 0.0;
            mat[4] = 0.0;
            mat[5] = 1.0;
            mat[6] = 0.0;
            mat[7] = 0.0;
            mat[8] = -sin;
            mat[9] = 0.0;
            mat[10] = cos;
            mat[11] = 0.0;
            mat[12] = 0.0;
            mat[13] = 0.0;
            mat[14] = 0.0;
            mat[15] = 1.0;
            return this;
        };
        Matrix.prototype.RotationZ = function (angle) {
            var rad = DegreesToRadians(angle);
            var cos = Math.cos(rad);
            var sin = Math.sin(rad);
            var mat = this.m;
            mat[0] = cos;
            mat[1] = -sin;
            mat[2] = 0.0;
            mat[3] = 0.0;
            mat[4] = sin;
            mat[5] = cos;
            mat[6] = 0.0;
            mat[7] = 0.0;
            mat[8] = 0.0;
            mat[9] = 0.0;
            mat[10] = 1.0;
            mat[11] = 0.0;
            mat[12] = 0.0;
            mat[13] = 0.0;
            mat[14] = 0.0;
            mat[15] = 1.0;
            return this;
        };
        // 观察矩阵
        Matrix.prototype.LookAtRH = function (eyePos, lookAt, up) {
            var zaxis;
            var yaxis;
            var xaxis;
            zaxis = eyePos.Sub(lookAt);
            zaxis.NormalizeSelf();
            yaxis = up;
            yaxis.NormalizeSelf();
            xaxis = yaxis.Cross(zaxis);
            xaxis.NormalizeSelf();
            yaxis = zaxis.Cross(xaxis);
            yaxis.NormalizeSelf();
            var mat4x4 = this.m;
            mat4x4[0] = xaxis.x;
            mat4x4[1] = xaxis.y;
            mat4x4[2] = xaxis.z;
            mat4x4[4] = yaxis.x;
            mat4x4[5] = yaxis.y;
            mat4x4[6] = yaxis.z;
            mat4x4[8] = zaxis.x;
            mat4x4[9] = zaxis.y;
            mat4x4[10] = zaxis.z;
            mat4x4[3] = -xaxis.Dot(eyePos);
            mat4x4[7] = -yaxis.Dot(eyePos);
            mat4x4[11] = -zaxis.Dot(eyePos);
            mat4x4[12] = 0.0;
            mat4x4[13] = 0.0;
            mat4x4[14] = 0.0;
            mat4x4[15] = 1.0;
            return this;
        };
        // 投影矩阵
        Matrix.prototype.PerspectiveRH = function (fov_y, aspect, zNear, zFar) {
            if (zNear > zFar) {
                var temp = zNear;
                zNear = zFar;
                zFar = temp;
            }
            var mat4x4 = this.m;
            for (var i = 0; i < 16; ++i)
                mat4x4[i] = 0;
            var n_f = 1.0 / (zNear - zFar);
            var yScale = 1.0 / Math.tan(DegreesToRadians(fov_y) * 0.5);
            var xScale = yScale / aspect;
            mat4x4[0] = xScale;
            mat4x4[5] = yScale;
            mat4x4[10] = (zFar + zNear) * n_f;
            mat4x4[14] = -1.0;
            mat4x4[11] = 2.0 * zFar * zNear * n_f;
            return this;
        };
        return Matrix;
    }());
    Mind.Matrix = Matrix;
    var Quaternion = (function () {
        function Quaternion(xOrQuat, y, z, w) {
            this.m = new Float32Array(4);
            if (typeof xOrQuat === 'undefined') {
                this.Unitization();
            }
            else if (typeof xOrQuat === 'number') {
                var m = this.m;
                m[0] = xOrQuat;
                m[1] = y === undefined ? 0 : y;
                m[2] = z === undefined ? 0 : z;
                m[3] = w === undefined ? 1 : w;
            }
            else {
                this.Copy(xOrQuat);
            }
        }
        Quaternion.prototype.Set = function (x, y, z, w) {
            var m = this.m;
            m[0] = x;
            m[1] = y;
            m[2] = z;
            m[3] = w;
        };
        Quaternion.prototype.Copy = function (other) {
            var m1 = this.m;
            var m2 = other.m;
            m1[0] = m2[0];
            m1[1] = m2[1];
            m1[2] = m2[2];
            m1[3] = m2[3];
        };
        Quaternion.prototype.Unitization = function () {
            var m = this.m;
            m[0] = 0;
            m[1] = 0;
            m[2] = 0;
            m[3] = 1;
        };
        Quaternion.prototype.Length = function () {
            var m = this.m;
            var x = m[0], y = m[1], z = m[2], w = m[3];
            var len = x * x + y * y + z * z + w * w;
            return Math.sqrt(len);
        };
        Quaternion.prototype.Conjuqate = function () {
            var m = this.m;
            return new Quaternion(-m[0], -m[1], -m[2], m[3]);
        };
        Quaternion.prototype.ConjuqateSelf = function () {
            var m = this.m;
            m[0] = -m[0];
            m[1] = -m[1];
            m[2] = -m[2];
            return this;
        };
        Quaternion.prototype.Add = function (other) {
            var m1 = this.m;
            var m2 = other.m;
            var x = m1[0] + m2[0];
            var y = m1[1] + m2[1];
            var z = m1[2] + m2[2];
            var w = m1[3] + m2[3];
            return new Quaternion(x, y, z, w);
        };
        Quaternion.prototype.AddSelf = function (other) {
            var m1 = this.m;
            var m2 = other.m;
            m1[0] += m2[0];
            m1[1] += m2[1];
            m1[2] += m2[2];
            m1[3] += m2[3];
            return this;
        };
        Quaternion.prototype.Multiply = function (other) {
            var q1 = this.m, q2 = other.m;
            var quat = new Quaternion(0, 0, 0, 0);
            var q3 = quat.m;
            q3[0] = q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1];
            q3[1] = q1[3] * q2[1] - q1[0] * q2[2] + q1[1] * q2[3] + q1[2] * q2[0];
            q3[2] = q1[3] * q2[2] + q1[0] * q2[1] - q1[1] * q2[0] + q1[2] * q2[3];
            q3[3] = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
            return quat;
        };
        Quaternion.prototype.MultiplySelf = function (other) {
            var q3 = this.m;
            var quat = new Quaternion(q3[0], q3[1], q3[2], q3[3]);
            var q1 = quat.m, q2 = other.m;
            q3[0] = q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1];
            q3[1] = q1[3] * q2[1] - q1[0] * q2[2] + q1[1] * q2[3] + q1[2] * q2[0];
            q3[2] = q1[3] * q2[2] + q1[0] * q2[1] - q1[1] * q2[0] + q1[2] * q2[3];
            q3[3] = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
            return this;
        };
        // 通过Tait Bryan Angles转换
        // Tait–Bryan angles
        // Roll – {\displaystyle \phi } \phi : rotation about the X-axis. Range(-PI, PI)
        // Pitch – {\displaystyle \theta } \theta : rotation about the Y-axis. Range(-PI/2, PI/2)
        // Yaw – {\displaystyle \psi } \psi : rotation about the Z-axis. Range(-PI, PI)
        // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
        Quaternion.prototype.FromTaitBryanAngles = function (roll, pitch, yaw) {
            var angle, sy, cy, sp, cp, sr, cr;
            var q = this.m;
            angle = DegreesToRadians(yaw * 0.5);
            sy = Math.sin(angle);
            cy = Math.cos(angle);
            angle = DegreesToRadians(pitch * 0.5);
            sp = Math.sin(angle);
            cp = Math.cos(angle);
            angle = DegreesToRadians(roll * 0.5);
            sr = Math.sin(angle);
            cr = Math.cos(angle);
            q[0] = (sr * cp * cy - cr * sp * sy);
            q[1] = (cr * sp * cy + sr * cp * sy);
            q[2] = (cr * cp * sy - sr * sp * cy);
            q[3] = (cr * cp * cy + sr * sp * sy);
        };
        // 转换成Tait Bryan Angles
        Quaternion.prototype.ToTaitBryanAngles = function () {
            var q = this.m;
            var t0 = 2 * (q[3] * q[0] + q[1] * q[2]);
            var t1 = 1 - 2 * (q[0] * q[0] + q[1] * q[1]);
            var roll = Math.atan2(t0, t1);
            var t2 = 2 * (q[3] * q[1] - q[2] * q[0]);
            t2 = t2 > 1 ? 1 : t2;
            t2 = t2 < -1 ? -1 : t2;
            var pitch = Math.asin(t2);
            var t3 = 2 * (q[3] * q[2] + q[0] * q[1]);
            var t4 = 1 - 2 * (q[1] * q[1] + q[2] * q[2]);
            var yaw = Math.atan2(t3, t4);
            return [RadiansToDegrees(roll), RadiansToDegrees(pitch), RadiansToDegrees(yaw)];
        };
        // 通过矩阵转换
        Quaternion.prototype.FromMatrix = function (matrix) {
            var temp = (new Quaternion()).m;
            var mat = matrix.m;
            var diag = mat[0] + mat[5] + mat[10] + 1;
            if (diag > 0.0) {
                var scale = Math.sqrt(diag) * 2.0; // get scale from diagonal
                // speed this up
                temp[0] = (mat[9] - mat[6]) / scale;
                temp[1] = (mat[2] - mat[8]) / scale;
                temp[2] = (mat[4] - mat[1]) / scale;
                temp[3] = 0.25 * scale;
            }
            else {
                if (mat[0] > mat[5] && mat[0] > mat[10]) {
                    // 1st element of diag is greatest value
                    // find scale according to 1st element, and double it
                    var scale = Math.sqrt(1.0 + mat[0] - mat[5] - mat[10]) * 2.0;
                    // speed this up
                    temp[0] = 0.25 * scale;
                    temp[1] = (mat[1] + mat[4]) / scale;
                    temp[2] = (mat[8] + mat[2]) / scale;
                    temp[3] = (mat[9] - mat[6]) / scale;
                }
                else if (mat[5] > mat[10]) {
                    // 2nd element of diag is greatest value
                    // find scale according to 2nd element, and double it
                    var scale = Math.sqrt(1.0 + mat[5] - mat[0] - mat[10]) * 2.0;
                    // speed this up
                    temp[0] = (mat[1] + mat[4]) / scale;
                    temp[1] = 0.25 * scale;
                    temp[2] = (mat[6] + mat[9]) / scale;
                    temp[3] = (mat[2] - mat[8]) / scale;
                }
                else {
                    // 3rd element of diag is greatest value
                    // find scale according to 3rd element, and double it
                    var scale = Math.sqrt(1.0 + mat[10] - mat[0] - mat[5]) * 2.0;
                    // speed this up
                    temp[0] = (mat[2] + mat[8]) / scale;
                    temp[1] = (mat[6] + mat[9]) / scale;
                    temp[2] = 0.25 * scale;
                    temp[3] = (mat[4] - mat[1]) / scale;
                }
            }
            return this.Set(temp[0], temp[1], temp[2], temp[3]);
        };
        // 转换到矩阵
        Quaternion.prototype.ToMatrix = function () {
            var q = this.m;
            var x = q[0], y = q[1], z = q[2], w = q[3];
            var s = 2.0;
            var newMat = new Matrix();
            var mat4x4 = newMat.m;
            mat4x4[0] = 1.0 - s * (y * y + z * z);
            mat4x4[1] = s * (x * y - w * z);
            mat4x4[2] = s * (w * y + x * z);
            mat4x4[3] = 0.0;
            mat4x4[4] = s * (x * y + w * z), mat4x4[5] = 1.0 - s * (x * x + z * z);
            mat4x4[6] = s * (y * z - w * x);
            mat4x4[7] = 0.0;
            mat4x4[8] = s * (x * z - w * y);
            mat4x4[9] = s * (y * z + w * x);
            mat4x4[10] = 1.0 - s * (x * x + y * y);
            mat4x4[11] = 0.0;
            mat4x4[12] = 0.0;
            mat4x4[13] = 0.0;
            mat4x4[14] = 0.0;
            mat4x4[15] = 1.0;
            return newMat;
        };
        Quaternion.prototype.GetColumn0 = function () {
            var q = this.m;
            var x = q[0], y = q[1], z = q[2], w = q[3];
            return new Vec3(1.0 - 2.0 * (y * y + z * z), 2.0 * (x * y + w * z), 2.0 * (x * z - w * y));
        };
        Quaternion.prototype.GetColumn1 = function () {
            var q = this.m;
            var x = q[0], y = q[1], z = q[2], w = q[3];
            return new Vec3(2.0 * (x * y - w * z), 1.0 - 2.0 * (x * x + z * z), 2.0 * (y * z + w * x));
        };
        Quaternion.prototype.GetColumn2 = function () {
            var q = this.m;
            var x = q[0], y = q[1], z = q[2], w = q[3];
            return new Vec3(2.0 * (x * z + w * y), 2.0 * (y * z - w * x), 1.0 - 2.0 * (x * x + y * y));
        };
        Quaternion.prototype.GetRow0 = function () {
            var q = this.m;
            var x = q[0], y = q[1], z = q[2], w = q[3];
            return new Vec3(1.0 - 2.0 * (y * y + z * z), 2.0 * (x * y - w * z), 2.0 * (w * y + x * z));
        };
        Quaternion.prototype.GetRow1 = function () {
            var q = this.m;
            var x = q[0], y = q[1], z = q[2], w = q[3];
            return new Vec3(2.0 * (x * y + w * z), 1.0 - 2.0 * (x * x + z * z), 2.0 * (y * z - w * x));
        };
        Quaternion.prototype.GetRow2 = function () {
            var q = this.m;
            var x = q[0], y = q[1], z = q[2], w = q[3];
            return new Vec3(2.0 * (x * z - w * y), 2.0 * (y * z + w * x), 1.0 - 2.0 * (x * x + y * y));
        };
        Quaternion.prototype.Slerp = function (other, t) {
            var q1 = this.m, q2 = other.m;
            var newQuat = new Quaternion(0, 0, 0, 0);
            var q3 = newQuat.m;
            if (t === 0.0) {
                q3[0] = q1[0];
                q3[1] = q1[1];
                q3[2] = q1[2];
                q3[3] = q1[3];
                return newQuat;
            }
            else if (t === 1.0) {
                q3[0] = q2[0];
                q3[1] = q2[1];
                q3[2] = q2[2];
                q3[3] = q2[3];
                return newQuat;
            }
            var p = q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3];
            if (Math.abs(p) >= 1.0) {
                q3[0] = q2[0];
                q3[1] = q2[1];
                q3[2] = q2[2];
                q3[3] = q2[3];
                return newQuat;
            }
            if (p < 0.0) {
                q3[0] = -q2[0];
                q3[1] = -q2[1];
                q3[2] = -q2[2];
                q3[3] = -q2[3];
                p = -p;
            }
            else {
                q3[0] = q2[0];
                q3[1] = q2[1];
                q3[2] = q2[2];
                q3[3] = q2[3];
            }
            var delta = Math.acos(p);
            var sinDelta = Math.sqrt(1.0 - p * p);
            var r1 = Math.sin((1 - t) * delta) / sinDelta;
            var r2 = Math.sin(t * delta) / sinDelta;
            q3[0] = (q1[0] * r1 + q3[0] * r2);
            q3[1] = (q1[1] * r1 + q3[1] * r2);
            q3[2] = (q1[2] * r1 + q3[2] * r2);
            q3[3] = (q1[3] * r1 + q3[3] * r2);
            return newQuat;
        };
        Quaternion.prototype.Rotate = function (position) {
            if (1) {
                // normal method
                var q1 = this, q2 = new Quaternion(position.x, position.y, position.z, 0), q3 = new Quaternion(this.Conjuqate());
                var result = q1.Multiply(q2);
                result.MultiplySelf(q3);
                return new Vec3(result.m[0], result.m[1], result.m[2]);
            }
            else {
                // nVidia SDK implementation
                var m = this.m;
                var qvec = new Vec3(m[0], m[1], m[2]);
                var uv = qvec.Cross(position);
                var uuv = qvec.Cross(uv);
                var halfW = 2.0 * m[3];
                return new Vec3(position.x + uv.x * halfW + uuv.x * 2.0, position.y + uv.y * halfW + uuv.y * 2.0, position.z + uv.z * halfW + uuv.z * 2.0);
            }
        };
        return Quaternion;
    }());
    Mind.Quaternion = Quaternion;
})(Mind || (Mind = {})); // end Mind
