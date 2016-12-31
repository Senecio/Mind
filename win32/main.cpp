
#include "System.h"
#include "duktape.h"
#include "WrapOpenGL.h"
#include "GLRenderer.h"
#include "WrapGLRenderer.h"

/* Faster builds with smaller header files */
#define VC_EXTRALEAN 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <cstdio>

static duk_ret_t ReadFile(duk_context *ctx) {
    const char *file = duk_to_string(ctx, 0);
    duk_push_string_file(ctx, file);
    return 1;  /* one return value */
}

DUK_LOCAL void fatal_handler(duk_context *ctx, duk_errcode_t code, const char *msg) {
  fprintf(stderr, "Fatal error: %s [code: %d]", msg, code);
  /* Fatal handler should not return. */
  exit(EXIT_FAILURE);
}



static duk_ret_t adder(duk_context *ctx) {
    int i;
    int n = duk_get_top(ctx);  /* #args */
    double res = 0.0;

    for (i = 0; i < n; i++) {
        res += duk_to_number(ctx, i);
    }

    duk_push_number(ctx, res);
    return 1;  /* one return value */
}

void CreateTempJSObject(duk_context *ctx)
{
    int top = duk_get_top(ctx);

    duk_idx_t obj_idx = duk_push_object(ctx);
    // 设置meaningOfLife属性
    duk_push_int(ctx, 42);
    duk_put_prop_string(ctx, obj_idx, "meaningOfLife");
    // 取得meaningOfLife属性
    duk_get_prop_string(ctx, obj_idx, "meaningOfLife");
    printf("result=: %s\n", duk_safe_to_string(ctx, -1));
    duk_pop(ctx); // 弹出结果

    // 设置adder属性为函数
    duk_push_c_function(ctx, adder, DUK_VARARGS);
    duk_put_prop_string(ctx, obj_idx, "adder");
    // 调用adder函数
    duk_push_string(ctx, "adder");
    duk_push_int(ctx, 1);
    duk_push_int(ctx, 1);
    duk_call_prop(ctx, obj_idx, 2);
    printf("result=: %s\n", duk_safe_to_string(ctx, -1));
    duk_pop(ctx); // 弹出结果
    duk_pop(ctx); // 销毁对象

    // 断言堆栈正常
    assert( duk_get_top(ctx) == top );

    // 创建一个任意构造函数
    duk_eval_string(ctx, "(function (a, b) { this.a = a; this.b = b; print('arg1='+a,'arg2='+b);})"); // 返回函数地址
    duk_push_string(ctx, "string");
    duk_push_int(ctx, 8989);
    duk_int_t rc = duk_pnew(ctx, 2); //2个参数
    if (rc != 0) {
        printf("failed: %s\n", duk_safe_to_string(ctx, -1));
    }else {
        duk_get_prop_string(ctx, 0, "b");
        printf("result=: %s\n", duk_safe_to_string(ctx, -1));
    }
    duk_pop(ctx); // 弹出结果
    duk_pop(ctx); // 销毁对象

    // 断言堆栈正常
    assert( duk_get_top(ctx) == top );

    // 目前了解c没有创建这种 var a = new Object(); 为对象取一个别名,只能得到obj_idx;
}

class JSVM
{
public:
    JSVM():ctx(0)
    {

    }

    ~JSVM()
    {

    }

    bool Start() {
        char *script_filename = "main.js";

        if ((ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal_handler)) == NULL) {
            fprintf(stderr, "FATAL: Failed to create the Duktape context.\n");
            End();
            return false;
        }

        int top = duk_get_top(ctx);

        // 添加dukeape的require()功能所必须得readFile函数.
        duk_push_global_object(ctx);
        duk_push_c_function(ctx, ReadFile, DUK_VARARGS);
        duk_put_prop_string(ctx, -2, "readFile");
        duk_pop(ctx);  /* pop global */

        // 创建game
        const char* createGame = "game = { 'onInit' : function() { print('override me'); }, 'onFrame' : function(deltaTime) { },  'onShut' : function() { print('override me'); } }";
        Eval(createGame);

        // 注册opengl
        duk_open_OpenGL(ctx);
        // 注册GL渲染器
        duk_open_GLRenderer(ctx);

        //Eval("print(GLRenderer.FLAG_FOO)");
        //Eval("GLRenderer.Clear({'a':1.0, 'r':0.0,'g':0.333,'b':0.6666}, 1.0)");

        if (duk_peval_file(ctx, script_filename) != 0) {
            fprintf(stderr, "Error loading file '%s' : %s\n", script_filename, duk_safe_to_string(ctx, -1));
            End();
            return false;
        }
        duk_pop(ctx); // pop peval_file 返回值


        //CreateTempJSObject(ctx); 测试
        // 断言堆栈正常
        assert( duk_get_top(ctx) == top );
        return true;
    }

    void Eval(const char* text) {
        if (ctx && text) {
            if(duk_peval_string(ctx, text) != 0) {
                duk_get_prop_string(ctx, -1, "stack");
                printf("%s\n", duk_safe_to_string(ctx, -1));
                duk_pop(ctx);
                return;
            }
            duk_pop(ctx);
            //printf("duk_top=%d\n", duk_get_top(ctx));
        }
    }

    void End()
    {
        if (ctx) {
            duk_destroy_heap(ctx); /* No-op if ctx is NULL */
            ctx = 0;
        }
    }

    duk_context *ctx;
};

int CALLBACK WinMain(
  HINSTANCE hInstance,     /* handle to the current instance of the application. */
  HINSTANCE hPrevInstance, /* handle to the previous instance of the application */
  LPSTR     lpCmdLine,     /* command line for the application */
  int       nCmdShow) {    /* controls how the window is to be shown */
  /* Get command line count and string from global variable __argc and __argv */

    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    WindowsSystem system;
    system.SetHInstance(hInstance);

    Window* window = system.AllocWindow();
    window->Create("Mind", 0,0, 800, 600);

    GLRenderer renderer;
    __renderer = &renderer;
    system.WindowAccelerateWithGLRender(window, &renderer);

    bool gameInit = false;
    JSVM javm;
    javm.Start();

    MSG msg;
    ZeroMemory (&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            if (gameInit == false) {
                javm.Eval("game.onInit();");
                gameInit = true;
            }else {
                javm.Eval("game.onFrame(0);");
            }
            Sleep(10);
        }
    }

    javm.Eval("game.onShut();");
    javm.End();
    window->Destory();
    return 0;
}
