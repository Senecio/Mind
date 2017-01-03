/// <reference path="..\..\engine\Renderer.ts" />
/// <reference path="..\..\engine\Camera.ts" />
/// <reference path="..\..\engine\Input.ts" />
/// <reference path="..\..\game\game.d.ts" />

var GameLog = function(...optionalParams: any[]) {
    console.log.apply(console, optionalParams);
}

declare var renderer: Mind.IRenderer;
declare var input : Mind.Input;

class GameInst implements Game {
    onInit() {}
    onFrame(deltaTime:number) {}
    onShut() {}
}

var game = new GameInst();
