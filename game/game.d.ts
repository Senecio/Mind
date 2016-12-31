declare interface Game {
    onInit(): any;
    onFrame(deltaTime:number): any;
    onShut(): any;
}