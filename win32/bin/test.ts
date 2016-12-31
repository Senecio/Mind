/// <reference path="duktape.d.ts" />

var logger = new Duktape.Logger();  // or new Duktape.Logger('logger name')
var text = "call 'test.js' module's function!";
module.exports = function() {
    logger.info(text);
}