/// <reference path="duktape.d.ts" />
/// <reference path="GLRenderer.ts" />
/// <reference path="..\..\game\game.d.ts" />

declare var game: Game;

var logger = new Duktape.Logger();  // or new Duktape.Logger('logger name')
var GameLog = function(message?: any, ...optionalParams: any[]) {
    if (message)
        logger.info(message);
    else
        logger.info.apply(logger, optionalParams);
}

Duktape.modSearch = function (id, require, exports, module) {
    /* readFile() reads a file from disk, and returns a string or undefined.
     * 'id' is in resolved canonical form so it only contains terms and
     * slashes, and no '.' or '..' terms.
     */
    var res;
    GameLog('loading module:', id);
    res = readFile(id + '.js');
    if (typeof res === 'string') {
        return res;
    }
    throw new Error('module not found: ' + id);
}

var renderer = new GLRenderer();
declare var opengl: WebGLRenderingContext;