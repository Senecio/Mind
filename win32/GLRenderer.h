#pragma once

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

class GLRenderer
{
public:
    GLRenderer(void);
    ~GLRenderer(void);

    void Present();

    void        SetHDC(HDC hDC) { _hDC = hDC; }
    HDC         GetHDC() { return _hDC; }
private:
    HDC         _hDC;
};

extern class GLRenderer* __renderer;
