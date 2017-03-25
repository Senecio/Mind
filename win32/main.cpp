
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

static duk_ret_t SyncReadFile(duk_context *ctx) {
    const char *file = duk_to_string(ctx, 0);
    duk_push_string_file(ctx, file);
    return 1;  /* one return value */
}

static duk_ret_t AsyncReadFile(duk_context *ctx);

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
    // ����meaningOfLife����
    duk_push_int(ctx, 42);
    duk_put_prop_string(ctx, obj_idx, "meaningOfLife");
    // ȡ��meaningOfLife����
    duk_get_prop_string(ctx, obj_idx, "meaningOfLife");
    printf("result=: %s\n", duk_safe_to_string(ctx, -1));
    duk_pop(ctx); // �������

    // ����adder����Ϊ����
    duk_push_c_function(ctx, adder, DUK_VARARGS);
    duk_put_prop_string(ctx, obj_idx, "adder");
    // ����adder����
    duk_push_string(ctx, "adder");
    duk_push_int(ctx, 1);
    duk_push_int(ctx, 1);
    duk_call_prop(ctx, obj_idx, 2);
    printf("result=: %s\n", duk_safe_to_string(ctx, -1));
    duk_pop(ctx); // �������
    duk_pop(ctx); // ���ٶ���

    // ���Զ�ջ����
    assert( duk_get_top(ctx) == top );

    // ����һ�����⹹�캯��
    duk_eval_string(ctx, "(function (a, b) { this.a = a; this.b = b; print('arg1='+a,'arg2='+b);})"); // ���غ�����ַ
    duk_push_string(ctx, "string");
    duk_push_int(ctx, 8989);
    duk_int_t rc = duk_pnew(ctx, 2); //2������
    if (rc != 0) {
        printf("failed: %s\n", duk_safe_to_string(ctx, -1));
    }else {
        duk_get_prop_string(ctx, 0, "b");
        printf("result=: %s\n", duk_safe_to_string(ctx, -1));
    }
    duk_pop(ctx); // �������
    duk_pop(ctx); // ���ٶ���

    // ���Զ�ջ����
    assert( duk_get_top(ctx) == top );

    // Ŀǰ�˽�cû�д������� var a = new Object(); Ϊ����ȡһ������,ֻ�ܵõ�obj_idx;
}

class JSVM
{
public:
    JSVM():ctx(0){ }

    ~JSVM(){ }

    bool Start() {
        char *script_filename = "main.js";

        if ((ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal_handler)) == NULL) {
            fprintf(stderr, "FATAL: Failed to create the Duktape context.\n");
            End();
            return false;
        }

        int top = duk_get_top(ctx);


        duk_push_global_object(ctx);
        // Ϊʵ��dukeape��require()����,require����Duktape.modSearch,��Duktape.modSearch����ʹ����SyncReadFile.
        // ���Ա���ʵ��SyncReadFile
        duk_push_c_function(ctx, SyncReadFile, DUK_VARARGS);
        duk_put_prop_string(ctx, -2, "SyncReadFile");
        // �첽�ļ���ȡ
        duk_push_c_function(ctx, AsyncReadFile, DUK_VARARGS);
        duk_put_prop_string(ctx, -2, "AsyncReadFile");

        duk_pop(ctx);  /* pop global */

        // ����game
        const char* createGame = "game = { 'onInit' : function() { print('override me'); }, 'onFrame' : function(deltaTime) { },  'onShut' : function() { print('override me'); } }";
        Eval(createGame);

        // ע��opengl
        duk_open_OpenGL(ctx);
        // ע��GL��Ⱦ��
        duk_open_GLRenderer(ctx);

        if (duk_peval_file(ctx, script_filename) != 0) {
            fprintf(stderr, "Error loading file '%s' : %s\n", script_filename, duk_safe_to_string(ctx, -1));
            End();
            return false;
        }
        duk_pop(ctx); // pop peval_file ����ֵ


        //CreateTempJSObject(ctx); ����
        // ���Զ�ջ����
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
            if (duk_get_top(ctx) != 0) {
                printf("duk_top=%d\n", duk_get_top(ctx));
            }
        }
    }

    void AsyncReadFileCallback(void* callback, unsigned buffLength, void* buffer)
    {
        if (callback)
        {
            int top = duk_get_top(ctx);

            duk_push_heapptr(ctx, callback);
            if (duk_is_callable(ctx, -1))
            {
                void *duk_buff = duk_push_fixed_buffer(ctx, buffLength);
                memcpy(duk_buff, buffer, buffLength);
                duk_push_buffer_object(ctx, 
                                        -1 /*index of plain buffer*/,
                                        0 /*byte offset*/,
                                        buffLength /*byte (!) length */,
                                        DUK_BUFOBJ_ARRAYBUFFER /*flags and type*/);

                /* The plain buffer is now referenced by the buffer object
	             * and doesn't need to be kept explicitly reachable.
	             */
                duk_remove(ctx, -2);

                duk_pcall(ctx, 1);
            }

            duk_pop(ctx);
            assert( duk_get_top(ctx) == top );
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

class ReadFileThread : public WinThread
{
public:
    ReadFileThread(WindowsSystem& system):_system(system) { }

    virtual int     Run() {
        if (_work.vaild)
        {
            WinFile* file = _system.CreateFile();
            file->Open(_work.fileName.c_str(), "r+b");
            _work.fileMemorySize = file->GetLength();
            if (_work.fileMemory != 0){
                SAFE_DELETE_ARRAY( _work.fileMemory );
            }
            _work.fileMemory = new char[_work.fileMemorySize];
            file->Read(_work.fileMemory, _work.fileMemorySize);
            _work.vaild = false;
        }
        
        Suspend();
        return 0;
    }

    struct Work
    {
        bool vaild;
        uint32 id;
        void* fileMemory;
        uint32 fileMemorySize;
        string fileName;
        Work():vaild(0),id(0),fileMemory(0),fileMemorySize(0){}
    };

public:
    Work _work;
    WindowsSystem& _system;
};

class AsyncReadFileMgr
{
public:
    AsyncReadFileMgr(ReadFileThread* thr, JSVM* jsvm):_id(0),_working(false),_thr(thr),_jsvm(jsvm) {}

    struct ReadFileInfo
    {
        uint32 id;
        void* callback;
        bool acquire;
        string fileName;

        ReadFileInfo():id(0),callback(0),acquire(0) {}
    };

    void Add(const char* fileName, void* callback) {
        int size = _infos.size();
        _infos.resize(size + 1);
        ReadFileInfo& info = _infos.back();
        info.id = ++_id;
        info.callback = callback;
        info.fileName.assign(fileName);
    }

    void CheckWork() {
        
        if (_infos.empty())
            return;
        
        ReadFileThread::Work& work = _thr->_work;
        if(!work.vaild)
        {
            if (!work.fileName.empty())
            {
                for (auto it = _infos.begin(); it != _infos.end(); ++it)
                {
                    if(it->id == work.id)
                    {
                        // �첽�ļ��ص�.
                        _jsvm->AsyncReadFileCallback(it->callback, work.fileMemorySize, work.fileMemory);
                        // ����
                        _infos.erase(it);
                        _working = false;
                        break;
                    }
                }
            }
        }

        if (!_infos.empty() && !_working)
        {
            auto it = _infos.front();
            work.vaild = true;
            work.fileName = it.fileName;
            work.id =it.id;

            _thr->Resume();
            _working = true;
        }
    }

    uint32 _id;
    bool _working;
    ReadFileThread* _thr;
    JSVM* _jsvm;
    std::list<ReadFileInfo> _infos;
    
};

AsyncReadFileMgr* _asyncReadFileMgr = 0;

BOOL FindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter)  
{  
    WIN32_FIND_DATA fd;  
    HANDLE hFind = FindFirstFile(lpPath, &fd);  
    BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;  
    BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;  
    FindClose(hFind);  
    return RetValue;  
}  

static duk_ret_t AsyncReadFile(duk_context *ctx) {

    const char *fileName = duk_to_string(ctx, 0);

    if (FindFirstFileExists(fileName, FALSE))
    {
        duk_bool_t b = duk_is_callable(ctx, 1);
        if (b) {
            void* callback = duk_get_heapptr(ctx, 1);
            if (_asyncReadFileMgr)
            {
                _asyncReadFileMgr->Add(fileName, callback);
            }
        }
    }
    else
    {
        printf("%s async read fail because the file not exists.\n", fileName);
    }


    return 0;  /* one return value */
}

class KeyboardListener : public Window::IKeyboardListener, public Window::IMouseListener
{
public:
    KeyboardListener(JSVM* jsvm = 0):_jsvm(jsvm) {}

    virtual void OnKeyDown(Key::Scan scan) {
        if (_jsvm) {
            char cmd[64];
            sprintf(cmd, "input.OnKeyDown(%d);", scan);
            _jsvm->Eval(cmd);
        }
    };
    virtual void OnKeyUp(Key::Scan scan) {
        if (_jsvm) {
            char cmd[64];
            sprintf(cmd, "input.OnKeyUp(%d);", scan);
            _jsvm->Eval(cmd);
        }
    };

    virtual void OnMove(int x, int y) {
        if (_jsvm) {
            char cmd[64];
            sprintf(cmd, "input.OnMouseMove(%d,%d);", x, y);
            _jsvm->Eval(cmd);
        }
    }
    virtual void OnButtonDown(int id, int x, int y) {
        if (_jsvm) {
            char cmd[64];
            sprintf(cmd, "input.OnMouseDown(%d,%d,%d);", id, x, y);
            _jsvm->Eval(cmd);
        }
    }
    virtual void OnButtonUp(int id, int x, int y) {
        if (_jsvm) {
            char cmd[64];
            sprintf(cmd, "input.OnMouseUp(%d,%d,%d);", id, x, y);
            _jsvm->Eval(cmd);
        }
    }
private:
    JSVM* _jsvm;
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
    JSVM jsvm;
    jsvm.Start();

    KeyboardListener listener(&jsvm);
    window->_keyboardListener = &listener;
    window->_mouseListener = &listener;

    ReadFileThread thr(system);

    AsyncReadFileMgr asyncReadFileMgr(&thr, &jsvm);
    _asyncReadFileMgr = &asyncReadFileMgr;

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
                jsvm.Eval("game.onInit();");
                _asyncReadFileMgr->CheckWork();
                gameInit = true;
            }else {
                jsvm.Eval("game.onFrame(0);");
                _asyncReadFileMgr->CheckWork();
            }
            Sleep(10);
        }
    }

    jsvm.Eval("game.onShut();");
    jsvm.End();
    window->Destory();
    return 0;
}
