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
    var Rect = (function () {
        function Rect() {
        }
        return Rect;
    }());
    Mind.Rect = Rect;
    var Vec3 = (function () {
        function Vec3(x, y, z) {
            this.x = x || 0;
            this.y = y || 0;
            this.z = z || 0;
        }
        return Vec3;
    }());
    Mind.Vec3 = Vec3;
    var Matrix = (function () {
        function Matrix() {
        }
        return Matrix;
    }());
    Mind.Matrix = Matrix;
})(Mind || (Mind = {}));
