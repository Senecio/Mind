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
    
    constructor(x?: number, y?: number, z?: number) {
        this.x = x || 0;
        this.y = y || 0;
        this.z = z || 0;
    }
}

export class Matrix {

}

}