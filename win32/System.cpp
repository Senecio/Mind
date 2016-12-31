#include "System.h"
#include <gl/glew.h>
#include <gl/wglew.h>
#include "GLRenderer.h"

// 多字节编码转为UTF8编码
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

// UTF8编码转为多字节编码  
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

Window::Window(WindowsSystem* system ):_system(system),_hwnd(0)
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

LRESULT CALLBACK Window::DefWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    //int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // TODO: 在此添加任意绘图代码...
        EndPaint(hwnd, &ps);
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

    // 销毁临时窗口
    wglMakeCurrent(NULL, NULL);

    if (temphRc)
        wglDeleteContext(temphRc);
    if(temphWnd)
        DestroyWindow(temphWnd);
    UnregisterClass(wndclassex.lpszClassName, hInstance);

    HWND hWnd = window->_hwnd;
    HDC  hDC = NULL;     // Private GDI Device Context
    HGLRC hRC = NULL;     // Permanent Rendering Context

    // 设置主窗口
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
                /* 不指定环境版本
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
