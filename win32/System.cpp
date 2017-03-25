#include "System.h"
#include <process.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include "GLRenderer.h"

// ���ֽڱ���תΪUTF8����
inline bool MBToUTF8(std::string& outUTF8, const char* pMB, int lenMB)
{
    // convert an MBCS string to widechar 
    int nLen = MultiByteToWideChar(CP_ACP, 0, pMB, lenMB, NULL, 0);
    WCHAR* lpszW = NULL;
    lpszW = new WCHAR[nLen];
    int nRtn = MultiByteToWideChar(CP_ACP, 0, pMB, lenMB, lpszW, nLen);
    if(nRtn != nLen)
    {
        delete[] lpszW;
        return false;
    }

    // convert an widechar string to utf8
    int lenUtf8 = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);
    if (lenUtf8 <= 0)
        return false;

    outUTF8.resize(lenUtf8);
    nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, &*outUTF8.begin(), lenUtf8, NULL, NULL);
    delete[] lpszW;

    if (nRtn != lenUtf8)
    {
        outUTF8.clear();
        return false;
    }
    return true;
}

// UTF8����תΪ���ֽڱ���  
inline bool UTF8ToMB(std::string& outMB, const char* pUTF8, int lenUtf)  
{  
    // convert an UTF8 string to widechar   
    int nLen = MultiByteToWideChar(CP_UTF8, 0, pUTF8, lenUtf, NULL, 0);  
    WCHAR* lpszW = NULL;  
    lpszW = new WCHAR[nLen]; 
    int nRtn = MultiByteToWideChar(CP_UTF8, 0, pUTF8, lenUtf, lpszW, nLen);  
    if(nRtn != nLen)  
    {  
        delete[] lpszW;  
        return false;  
    }

    // convert an widechar string to Multibyte   
    int lenMB = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
    if (lenMB <=0)  
        return false;

    outMB.resize(lenMB);  
    nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, &*outMB.begin(), lenMB, NULL, NULL);  
    delete[] lpszW;  

    if(nRtn != lenMB)  
    {  
        outMB.clear();  
        return false;  
    }  
    return true;  
}

#define szWindowClass "MindEngine:Window"

Window::Window(WindowsSystem* system ):_system(system),_hwnd(0),_keyboardListener(0),_mouseListener(0)
{

}

bool Window::Create( const char* szTitle, uint32 x, uint32 y, uint32 width, uint32 height )
{
    WNDCLASSEX wcex;
    if(!GetClassInfoEx(_system->GetHInstance(), szWindowClass, &wcex))
    {
        RegisterClass(_system->GetHInstance(), szWindowClass);
    }

    // Set the window's initial width
    UINT windowStyle = WS_VISIBLE | WS_CAPTION | WS_SYSMENU;; //WS_OVERLAPPEDWINDOW;
    RECT rc;
    SetRect( &rc, 0, 0, width, height);
    AdjustWindowRect( &rc, windowStyle, FALSE );

    string title;
    UTF8ToMB(title, szTitle, strlen(szTitle));
    is_valid_utf8(szTitle);
    _hwnd = CreateWindowEx(0, szWindowClass, title.c_str(), windowStyle,
        x, y, (rc.right - rc.left), (rc.bottom - rc.top), NULL, NULL, _system->GetHInstance(), NULL);


    if (!_hwnd)
    {
        return false;
    }

    ShowWindow(_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(_hwnd);

    SetWindowLong(_hwnd, GWL_USERDATA, (LONG)this);

    return true;
}

void Window::Destory()
{
    DestroyWindow(_hwnd);
    _hwnd = 0;
}

ATOM Window::RegisterClass( HINSTANCE hInstance, const char* windowClass )
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= Window::DefWndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= windowClass;
    wcex.hIconSm		= wcex.hIcon;

    return RegisterClassEx(&wcex);
}

Key::Scan Virtualkey2KeyScan(WPARAM wParam, LPARAM lParam)
{
    UINT scancode = 0;
    if (HIWORD(lParam) & 0x0F00)
    { 
        scancode = MapVirtualKey(wParam, 0) | 0x80;
    }
    else
    {
        scancode = HIWORD(lParam) & 0x00FF;
    }
    return Key::Scan(scancode);
}

LRESULT CALLBACK Window::DefWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    //int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    Window *window = (Window*)GetWindowLong(hwnd, GWL_USERDATA);

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // TODO: �ڴ���������ͼ����...
        EndPaint(hwnd, &ps);
        break;
    case WM_KEYDOWN:
        {
            if (window->_keyboardListener) {
                Key::Scan scan = Virtualkey2KeyScan(wParam, lParam);
                window->_keyboardListener->OnKeyDown(scan);
            }
        }
        break;
    case WM_KEYUP:
        {
            if (window->_keyboardListener) {
                Key::Scan scan = Virtualkey2KeyScan(wParam, lParam);
                window->_keyboardListener->OnKeyUp(scan);
            }
        }
        break;
    case WM_MOUSEMOVE:
        {
            int x_pos = LOWORD(lParam);
            int y_pos = HIWORD(lParam);
            if (window->_mouseListener)
            {
                window->_mouseListener->OnMove(x_pos, y_pos);
            }
            return 0;
        }
        break;
    case WM_LBUTTONDOWN:
        {
            int x_pos = LOWORD(lParam);
            int y_pos = HIWORD(lParam);
            if (window->_mouseListener)
            {
                window->_mouseListener->OnButtonDown(MBID_LEFT, x_pos, y_pos);
            }
        }
        break;
    case WM_LBUTTONUP:
        {
            int x_pos = LOWORD(lParam);
            int y_pos = HIWORD(lParam);
            if (window->_mouseListener)
            {
                window->_mouseListener->OnButtonUp(MBID_LEFT, x_pos, y_pos);
            }
        }
        break;
    case WM_RBUTTONDOWN:
        {
            int x_pos = LOWORD(lParam);
            int y_pos = HIWORD(lParam);
            if (window->_mouseListener)
            {
                window->_mouseListener->OnButtonDown(MBID_RIGHT, x_pos, y_pos);
            }
        }
        break;
    case WM_RBUTTONUP:
        {
            int x_pos = LOWORD(lParam);
            int y_pos = HIWORD(lParam);
            if (window->_mouseListener)
            {
                window->_mouseListener->OnButtonUp(MBID_RIGHT, x_pos, y_pos);
            }
        }
        break;
    case WM_MBUTTONDOWN:
        {
            int x_pos = LOWORD(lParam);
            int y_pos = HIWORD(lParam);
            if (window->_mouseListener)
            {
                window->_mouseListener->OnButtonDown(MBID_MIDDLE, x_pos, y_pos);
            }
        }
        break;
    case WM_MBUTTONUP:
        {
            int x_pos = LOWORD(lParam);
            int y_pos = HIWORD(lParam);
            if (window->_mouseListener)
            {
                window->_mouseListener->OnButtonUp(MBID_MIDDLE, x_pos, y_pos);
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}


WinFile::WinFile(WindowsSystem* system):_system(system)
{

}

WinFile::~WinFile()
{

}

bool WinFile::Open(const char* fileName, const char* mode)
{
    _fp = fopen(fileName, mode);
    if (_fp == 0) { printf("Open file(%s) failure.\n", fileName); }
    return _fp != 0; 
}

void WinFile::Close()
{
    if(_fp)
    { 
        fclose(_fp); 
        _fp = NULL; 
    } 
}

uint32 WinFile::GetLength()
{
    int positon = ftell(_fp);
    fseek(_fp, 0, SEEK_END);
    int length = ftell(_fp);
    fseek(_fp, positon, SEEK_SET);
    return length;
}

bool  WinFile::Seek(uint32 offset)
{ 
    return fseek(_fp, offset, SEEK_CUR) == 0;
}

uint32 WinFile::Read(void* buffer, uint32 size, uint32 count) 
{ 
    return fread(buffer, size, count, _fp); 
}


int8 WinFile::ReadByte()  
{ 
    static int const byteSize = sizeof(int8);
    int8 byte; 
    Read((char*)&byte, byteSize, 1); 
    return byte; 
}

int32 WinFile::ReadDWord() 
{ 
    static int const dwordSize = sizeof(int32);
    int32 dword; 
    Read((char*)&dword, dwordSize, 1); 
    return dword; 
}

float WinFile::ReadFloat() 
{ 
    static int const floatSize = sizeof(float);
    float real; 
    Read((char*)&real, floatSize, 1);
    return real; 
}

bool WinFile::IsEnd()
{
    return 0 != feof(_fp);
}

bool WinFile::ReadLine( std::string& line )
{
    int n = 0;
    do
    {
        char c = ReadByte();
        if (IsEnd())
            break;

        if (c == '\r')
        {
            if (n != 0)
            {
                return true;
            }
            else
            {
                continue;
            }
        }
        if (c != '\n')
        {
            if (c == '\t')
            {
                c = ' ';
            }
            line.push_back(c);
        }
    }while (1);

    return false;
}


inline uint32 __stdcall WinThread::ThreadFunction(void *pV)
{
    static int result = 0;
    WinThread* pThis = (WinThread*)pV;

    if (pThis)
    {
        try
        {
            while (::WaitForSingleObject(pThis->_closeEvent, pThis->_timeoutMillisTerminate) == WAIT_TIMEOUT)
            {
                result = pThis->Run();
            }
        }
        catch(...)
        {
        }
    }
    return result;
}

WinThread::WinThread()
{
    unsigned int threadID = 0;

    _handle = (HANDLE)::_beginthreadex(0, 0, ThreadFunction, (void*)this, CREATE_SUSPENDED, &threadID);

    if (_handle == INVALID_HANDLE_VALUE)
    {
        ::MessageBox(0, "�̴߳���ʧ��", "����", MB_OK);
    }

    // �����ر��¼����
    _closeEvent = ::CreateEvent(NULL, false, false, NULL);		// "Close Thread"
    if(_closeEvent == 0)
    {
        ::MessageBox(0, "�̹߳ر��¼�����ʧ��", "����", MB_OK);
    }

    _timeoutMillisTerminate = 0;
}

WinThread::~WinThread()
{
    if (_handle != INVALID_HANDLE_VALUE)
    {
        if (::WaitForSingleObject (_handle, 0) == WAIT_TIMEOUT)
        {
            ::TerminateThread (_handle, 5);
        }
        ::CloseHandle(_handle);
        _handle = INVALID_HANDLE_VALUE;
    }

    if (_closeEvent != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(_closeEvent);
        _closeEvent = INVALID_HANDLE_VALUE;
    }
}

DWORD WinThread::Resume()
{
    return ::ResumeThread(_handle);
}

DWORD WinThread::Suspend()
{
    return ::SuspendThread(_handle);
}

bool WinThread::Wait()
{
    return Wait((uint32)INFINITE);
}

bool WinThread::Wait(uint32 timeoutMillis)
{
    bool ok = false;
    DWORD result = ::WaitForSingleObject(_handle, timeoutMillis);
    if (result == WAIT_TIMEOUT)
        ok = false;
    else if (result == WAIT_OBJECT_0)
        ok = true;
    else
    {

    }
    return ok;
}

void WinThread::Terminate(uint32 timeoutMillis)
{
    _timeoutMillisTerminate = timeoutMillis;
    ::ResumeThread(_handle);
    ::SetEvent(_closeEvent);
}

WindowsSystem::WindowsSystem(void)
{

}

WindowsSystem::~WindowsSystem(void)
{
}

Window* WindowsSystem::AllocWindow()
{
    return new Window(this);
}

void WindowsSystem::DestoryWindow( Window* window )
{
    window->Destory();
}

WinFile* WindowsSystem::CreateFile()
{
    return new WinFile(this);
}
void WindowsSystem::CloseFile(WinFile* file)
{
    file->Close();
    delete file;
}

bool WindowsSystem::WindowAccelerateWithGLRender( Window* window, class GLRenderer* renderer )
{
    static PIXELFORMATDESCRIPTOR pfd =                  // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
        1,                                          // Version Number
        PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
        PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
        PFD_TYPE_RGBA,                              // Request An RGBA Format
        32,                                         // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
        0,                                          // No Alpha Buffer
        0,                                          // Shift Bit Ignored
        0,                                          // No Accumulation Buffer
        0, 0, 0, 0,                                 // Accumulation Bits Ignored
        24,                                         // 24Bit Z-Buffer (Depth Buffer)
        8,                                          // No Stencil Buffer
        0,                                          // No Auxiliary Buffer
        PFD_MAIN_PLANE,                             // Main Drawing Layer
        0,                                          // Reserved
        0, 0, 0                                     // Layer Masks Ignored
    };

        
    HINSTANCE hInstance = window->_system->GetHInstance();;
    UINT windowStyle = 0;
    WNDCLASSEX   wndclassex = {0};
    wndclassex.cbSize        = sizeof(WNDCLASSEX);
    wndclassex.style         = windowStyle;
    wndclassex.lpfnWndProc   = DefWindowProc;
    wndclassex.hInstance     = hInstance;
    wndclassex.hIcon         = LoadIcon (NULL, IDI_WINLOGO);
    wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wndclassex.lpszClassName = "TempWindow";
    wndclassex.hIconSm       = wndclassex.hIcon;
    HWND temphWnd = NULL;
    HDC temphDC = NULL;
    HGLRC temphRc = NULL;
    if (RegisterClassEx (&wndclassex))
    {
        temphWnd = CreateWindowEx (0, wndclassex.lpszClassName, wndclassex.lpszClassName, windowStyle, 0, 0, 
            100, 100, NULL, NULL, hInstance, NULL);

        if (temphWnd)
        {
            if (temphDC = GetDC(temphWnd))
            {
                int tempPixelFormat;
                if ((tempPixelFormat=ChoosePixelFormat(temphDC, &pfd)))
                {
                    if(SetPixelFormat(temphDC,tempPixelFormat,&pfd))
                    {
                        if ((temphRc=wglCreateContext(temphDC)))
                        {
                            wglMakeCurrent(temphDC,temphRc);
                        }
                    }
                }
            }
        }
    }

    // Initialize GLEW
    if (GLEW_OK != glewInit())
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(temphRc);
        MessageBox(NULL, "Failed to initialize GLEW.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        DestroyWindow(temphWnd);
        return false;
    }

    // ������ʱ����
    wglMakeCurrent(NULL, NULL);

    if (temphRc)
        wglDeleteContext(temphRc);
    if(temphWnd)
        DestroyWindow(temphWnd);
    UnregisterClass(wndclassex.lpszClassName, hInstance);

    HWND hWnd = window->_hwnd;
    HDC  hDC = NULL;     // Private GDI Device Context
    HGLRC hRC = NULL;     // Permanent Rendering Context

    // ����������
    if (!(hDC=GetDC(hWnd)))                           // Did We Get A Device Context?
    {
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        DestroyWindow(hWnd);
        return false;                               // Return FALSE
    }

    bool multisampling = true;
    int PixelFormat;
    if (multisampling && wglChoosePixelFormatARB /*&& wglCreateContextAttribsARB*/)
    {
        // MSAA PixelFormat
        int attributes[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB,     32,
            WGL_ALPHA_BITS_ARB,     8,
            WGL_DEPTH_BITS_ARB,     24,
            WGL_STENCIL_BITS_ARB,   8,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB,        0,
            0, 0
        };

        UINT numFormats = 0;
        BOOL bStatus = FALSE;

        for (int samples = 16; samples > 0; samples /= 2)
        {
            attributes[17] = samples;

            bStatus = wglChoosePixelFormatARB(hDC, attributes, 0, 1,
                &PixelFormat, &numFormats);

            if (bStatus == TRUE && numFormats)
            {
                break;
                /* ��ָ�������汾
                int attribs[] =
                {
                    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                    WGL_CONTEXT_MINOR_VERSION_ARB, 1,
                    WGL_CONTEXT_FLAGS_ARB, 0,
                    0
                };

                if (!(hRC=wglCreateContextAttribsARB(hDC, 0, attribs)))               // Are We Able To Get A Rendering Context?
                {
                    MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
                    DestroyWindow(g_hWnd);
                    return false;                               // Return FALSE
                }

                glEnable(GL_MULTISAMPLE_ARB);
                */
            }
        }
        if (!(bStatus == TRUE && numFormats))
        {
            MessageBox(NULL, "Can't Find A Suitable wglChoosePixelFormatARB.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
            DestroyWindow(hWnd);
            return false;
        }
        /*
        if (wglChoosePixelFormatARB)
        {
            struct CSAAPixelFormat
            {
                int numColorSamples;
                int numCoverageSamples;
                const char *pszDescription;
            };

            CSAAPixelFormat csaaPixelFormats[] =
            {
                { 4, 16, "16x CSAA" },
                { 4, 8,  "8x CSAA" }
            };

            CSAAPixelFormat csaaQualityPixelFormats[] =
            {
                { 8, 16, "16xQ (Quality) CSAA" },
                { 8, 8,  "8xQ (Quality) CSAA" }
            };

            CSAAPixelFormat *pCSAAFormats = 0;

            int attributes[] =
            {
                WGL_SAMPLE_BUFFERS_ARB,  1,
                WGL_COLOR_SAMPLES_NV,    0,
                WGL_COVERAGE_SAMPLES_NV, 0,
                WGL_DOUBLE_BUFFER_ARB,   1,
                0, 0
            };

            int returnedPixelFormat = 0;
            UINT numFormats = 0;
            BOOL bStatus = FALSE;

            int samples = 8;
            if (samples >= 8)
                pCSAAFormats = csaaQualityPixelFormats;
            else
                pCSAAFormats = csaaPixelFormats;

            for (int i = 0; i < 2; ++i)
            {
                attributes[3] = pCSAAFormats[i].numColorSamples;
                attributes[5] = pCSAAFormats[i].numCoverageSamples;

                bStatus = wglChoosePixelFormatARB(hDC, attributes, 0, 1,
                    &returnedPixelFormat, &numFormats);

                if (bStatus == TRUE && numFormats)
                {
                    break;
                }
            }
        }
        */
    }
    else
    {
            
        if (!(PixelFormat=ChoosePixelFormat(hDC, &pfd)))    // Did Windows Find A Matching Pixel Format?
        {
            MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
            DestroyWindow(hWnd);
            return false;
        }
    }
      
    if(!SetPixelFormat(hDC,PixelFormat,&pfd))       // Are We Able To Set The Pixel Format?
    {
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        DestroyWindow(hWnd);
        return false;
    }

    if (!(hRC=wglCreateContext(hDC)))               // Are We Able To Get A Rendering Context?
    {
        MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        DestroyWindow(hWnd);
        return false;                               // Return FALSE
    }

    if(!wglMakeCurrent(hDC,hRC))                    // Try To Activate The Rendering Context
    {
        wglDeleteContext(hRC);
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        DestroyWindow(hWnd);
        return false;                               // Return FALSE
    }
    
    renderer->SetHDC(hDC);
    return true;
}


