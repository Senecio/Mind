#include "GLRenderer.h"

GLRenderer* __renderer = 0;

GLRenderer::GLRenderer( void )
{

}

GLRenderer::~GLRenderer( void )
{

}

void GLRenderer::Present()
{
    SwapBuffers(_hDC);
}

