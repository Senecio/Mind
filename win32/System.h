#pragma once

#include "Common.h"
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

class  WindowsSystem;

class Window
{
public:
    class IKeyboardListener
    {
    public: 
        virtual void OnKeyDown(Key::Scan scan) = 0;
        virtual void OnKeyUp(Key::Scan scan) = 0;
    };

    class IMouseListener
    {
    public:
        virtual void OnMove(int x, int y) = 0;
        virtual void OnButtonDown(int id, int x, int y) = 0;
        virtual void OnButtonUp(int id, int x, int y) = 0;
    };

    Window(WindowsSystem* system);
    ~Window();

    bool Create(const char* title, uint32 x, uint32 y, uint32 width, uint32 height);
    void Destory();

    ATOM RegisterClass(HINSTANCE hInstance, const char* windowClass);
    static LRESULT CALLBACK DefWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

    WindowsSystem*  _system;
    HWND            _hwnd;

    IKeyboardListener*  _keyboardListener;
    IMouseListener*     _mouseListener;
};

class WinFile
{
public:
    WinFile(WindowsSystem* system);
    virtual ~WinFile();

    virtual bool        Open(const char* fileName, const char* mode);
    virtual void        Close();
    virtual uint32      GetLength();
    virtual bool        Seek(uint32 offset);
    virtual uint32      Read(void* buffer, uint32 size, uint32 count = 1);
    int8                ReadByte();
    int32               ReadDWord();
    float               ReadFloat();
    bool                ReadLine(std::string& line);
    virtual bool        IsEnd();

    WindowsSystem*  _system;
    FILE* _fp;
};


class WinThread 
{
    static uint32 __stdcall ThreadFunction(void *pV);
public:

    WinThread(void);
    virtual ~WinThread(void);

    DWORD           Resume();

    DWORD           Suspend();

    bool            Wait();

    bool            Wait(uint32 timeoutMillis);

    void            Terminate(uint32 timeoutMillis);

    HANDLE          GetHandle() { return _handle; }
protected:
    virtual int     Run() = 0;
private:
    HANDLE          _handle;
    HANDLE          _closeEvent;
    uint32          _timeoutMillisTerminate;
};

class WindowsSystem
{
public:
    WindowsSystem(void);
    ~WindowsSystem(void);


    Window* AllocWindow();
    void    DestoryWindow(Window* window);

    WinFile* CreateFile();
    void     CloseFile(WinFile* file);

    void        SetHInstance(HINSTANCE hInstance) { _hInstance = hInstance; }
    HINSTANCE   GetHInstance() { return _hInstance; }


    bool WindowAccelerateWithGLRender(Window* window, class GLRenderer* renderer);

    HINSTANCE _hInstance;
};

