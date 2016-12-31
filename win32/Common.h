#pragma once

#if defined(_MSC_VER)
    typedef __int8     int8;
    typedef __int16    int16;
    typedef __int32    int32;
    typedef __int64    int64;

    typedef unsigned __int8     uint8;
    typedef unsigned __int16    uint16;
    typedef unsigned __int32    uint32;
    typedef unsigned __int64    uint64;
#else
    typedef char       int8;
    typedef short      int16;
    typedef int        int32;
    typedef long long  int64;

    typedef unsigned char       uint8;
    typedef unsigned short      uint16;
    typedef unsigned int        uint32;
    typedef unsigned long long  uint64; 
#endif

    //! Defines an override macro, to protect virtual functions from typos and other mismatches
    /** Usage in a derived class:
    virtual void somefunc() _OVERRIDE_; copy from irrlicth.
    */
#if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 7 && (defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L) )
#define _OVERRIDE_ override
#elif (_MSC_VER >= 1600 ) /* supported since MSVC 2010 */
#define _OVERRIDE_ override
#elif (__clang_major__ >= 3)
#define _OVERRIDE_ override
#else
#define _OVERRIDE_
#endif

#if defined(_MSC_VER)
#pragma warning (disable : 4996)    ///Turn off warnings about deprecated C routines
#endif

// c
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstdarg>
#include <cstring>
#include <cmath>

// c++
#include <string>
#include <vector>
#include <map>

using namespace std;


#undef SAFE_DELETE
#define SAFE_DELETE(x) { if(x) { delete x; x = NULL; } }

#undef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if(x) { delete[] x; x = NULL; } }


// ¿í×Ö·û´®×ª¶à×Ö·û´®
inline std::string WS2S(const std::wstring& ws)
{
    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* source = ws.c_str();
    size_t destSize = 2 * ws.size() + 1;
    char *dest = new char[destSize];
    memset(dest, 0, destSize);
    wcstombs(dest, source, destSize);
    std::string result(dest);
    delete [] dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

// ¶à×Ö·û´®×ª¿í×Ö·û´®
inline std::wstring S2WS(const std::string& s)
{
    setlocale(LC_ALL, "chs"); 
    const char* source = s.c_str();
    size_t destSize = s.size() + 1;
    wchar_t *dest = new wchar_t[destSize];
    wmemset(dest, 0, destSize);
    mbstowcs(dest, source, destSize);
    std::wstring result(dest);
    delete [] dest;
    setlocale(LC_ALL, "C");
    return result;
}


inline bool is_valid_utf8(const char * string)
{
    if (!string)
        return false;

    const unsigned char * bytes = (const unsigned char *)string;
    unsigned int cp;
    int num;

    while (*bytes != 0x00)
    {
        if ((*bytes & 0x80) == 0x00)
        {
            // U+0000 to U+007F 
            cp = (*bytes & 0x7F);
            num = 1;
        }
        else if ((*bytes & 0xE0) == 0xC0)
        {
            // U+0080 to U+07FF 
            cp = (*bytes & 0x1F);
            num = 2;
        }
        else if ((*bytes & 0xF0) == 0xE0)
        {
            // U+0800 to U+FFFF 
            cp = (*bytes & 0x0F);
            num = 3;
        }
        else if ((*bytes & 0xF8) == 0xF0)
        {
            // U+10000 to U+10FFFF 
            cp = (*bytes & 0x07);
            num = 4;
        }
        else
            return false;

        bytes += 1;
        for (int i = 1; i < num; ++i)
        {
            if ((*bytes & 0xC0) != 0x80)
                return false;
            cp = (cp << 6) | (*bytes & 0x3F);
            bytes += 1;
        }

        if ((cp > 0x10FFFF) ||
            ((cp >= 0xD800) && (cp <= 0xDFFF)) ||
            ((cp <= 0x007F) && (num != 1)) ||
            ((cp >= 0x0080) && (cp <= 0x07FF) && (num != 2)) ||
            ((cp >= 0x0800) && (cp <= 0xFFFF) && (num != 3)) ||
            ((cp >= 0x10000) && (cp <= 0x1FFFFF) && (num != 4)))
            return false;
    }

    return true;
}


// All interfaces derive from this.
class IBaseInterface
{
public:
    virtual         ~IBaseInterface() {}
};

extern const int IFACE_OK;      // 0
extern const int IFACE_FAILED;  // -1

#define CREATEINTERFACE_PROCNAME    "CreateInterface"
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

typedef void* (*InstantiateInterfaceFn)();

//////////////////////////////////////////////////////////////////////////
// Used internally to register classes.
class InterfaceReg
{
public:
    InterfaceReg(InstantiateInterfaceFn fn, const char *pName);

public:
    InstantiateInterfaceFn  m_CreateFn;
    const char              *m_pName;

    InterfaceReg            *m_pNext; // For the global list.
    static InterfaceReg     *s_pInterfaceRegs;
};

#define EXPOSE_INTERFACE_FN(functionName, interfaceName, versionName) \
    static InterfaceReg __g_Create##interfaceName##_reg(functionName, versionName);

#define EXPOSE_INTERFACE(className, interfaceName, versionName) \
    static void* __Create##className##_interface() {return (interfaceName *)new className;}\
    static InterfaceReg __g_Create##className##_reg(__Create##className##_interface, versionName );

// Use this to expose a singleton interface with a global variable you've created.
#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, globalVarName) \
    static void* __Create##className##interfaceName##_interface() {return (interfaceName *)&globalVarName;}\
    static InterfaceReg __g_Create##className##interfaceName##_reg(__Create##className##interfaceName##_interface, versionName);

// Use this to expose a singleton interface. This creates the global variable for you automatically.
#define EXPOSE_SINGLE_INTERFACE(className, interfaceName, versionName) \
    static className __g_##className##_singleton;\
    EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, __g_##className##_singleton)

//DLL_EXPORT 
void* CreateInterface(const char *pName, int *pReturnCode);
//////////////////////////////////////////////////////////////////////////