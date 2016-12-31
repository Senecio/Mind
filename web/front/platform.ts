/// <reference path="..\..\engine\Renderer.ts" />
/// <reference path="..\..\game\game.d.ts" />

var GameLog = function(message?: any, ...optionalParams: any[]) {
    if (message)
        console.log(message);
    else
        console.log.apply(console, optionalParams);
}

declare var renderer: Mind.IRenderer;

class GameInst implements Game {
    onInit() {}
    onFrame(deltaTime:number) {}
    onShut() {}
}

var game = new GameInst();