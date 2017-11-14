/// <reference path="../../engine/Renderer.ts" />
/// <reference path="../../engine/Camera.ts" />
/// <reference path="../../engine/Input.ts" />
/// <reference path="../../game/game.d.ts" />
/// <reference path="../../game/binaryreader.ts" />
/// <reference path="../../game/mdxParser.ts" />

var GameLog = function (...optionalParams: any[]) {
    console.log.apply(console, optionalParams);
}


var AsyncReadFile = function (file, callback) {
    /**
     * Sends an XHR2 request.
     *
     * @param {string} path The url to request.
     * @param {boolean} binary If true, the request type will be arraybuffer.
     * @param {function} onload onload callback.
     * @param {function} onerror onerror callback.
     * @param {function} onprogress onprogress callback.
     */
    function getRequest(path, binary, onload, onerror, onprogress) {
        var xhr = new XMLHttpRequest();

        if (onload) {
            xhr.addEventListener("load", onload);
        }

        if (onerror) {
            xhr.addEventListener("error", onerror);
        }

        if (onprogress) {
            xhr.addEventListener("progress", onprogress);
        }

        xhr.open("GET", path, true);

        if (binary) {
            xhr.responseType = "arraybuffer";
        }

        xhr.send();

        return xhr;
    }

    getRequest(file, true, function (event) {
        if (event.target.status === 200) {
            callback(event.target.response);
        }
        else {
            callback(null);
        }
    }, null, null);
}

declare var renderer: Mind.IRenderer;
declare var input: Mind.Input;

class GameInst implements Game {
    onInit() { }
    onFrame(deltaTime: number) { }
    onShut() { }
}

var game = new GameInst();
