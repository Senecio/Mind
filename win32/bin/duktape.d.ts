// Duktape built-ins

// extracted from lib.d.ts
//declare interface Console {
//    log(message?: any, ...optionalParams: any[]): void;
//}

//declare var console: Console;

// Duktape require isn't recognized as a function, but can be used as one
declare function require(filename: string): any;
declare function readFile(filename: string): string;
declare interface DuktapeModule {
    modSearch(id: string, require, exports, module);
    Logger() : void;
}
declare var Duktape: DuktapeModule;

// module exports
declare interface ModuleExports {
    exports : any;
}

declare var module: ModuleExports;