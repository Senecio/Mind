/// <reference path="duktape.d.ts" />
/// <reference path="GLRenderer.ts" />
/// <reference path="../../engine/Camera.ts" />
/// <reference path="../../engine/Input.ts" />
/// <reference path="../../game/game.d.ts" />
/// <reference path="../../game/binaryreader.ts" />
/// <reference path="../../game/mdxParser.ts" />

declare var game: Game;

var logger = new Duktape.Logger();  // or new Duktape.Logger('logger name')
var GameLog = function (...optionalParams: any[]) {
    logger.info.apply(logger, optionalParams);
}

Duktape.modSearch = function (id, require, exports, module) {
    /* SyncReadFile() reads a file from disk, and returns a string or undefined.
     * 'id' is in resolved canonical form so it only contains terms and
     * slashes, and no '.' or '..' terms.
     */
    var res;
    GameLog('loading module:', id);
    res = SyncReadFile(id + '.js');
    if (typeof res === 'string') {
        return res;
    }
    throw new Error('module not found: ' + id);
}

var renderer = new GLRenderer();
var input = new Mind.Input();
declare var opengl: WebGLRenderingContext;

declare var input: Mind.Input;