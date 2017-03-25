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
#include <list>
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

struct Key
{
    // enumeration of scan codes. Customised from table taken from Ogre, which took them from DirectInput...
    enum Scan
    {
        Escape          =0x01,
        One             =0x02,
        Two             =0x03,
        Three           =0x04,
        Four            =0x05,
        Five            =0x06,
        Six             =0x07,
        Seven           =0x08,
        Eight           =0x09,
        Nine            =0x0A,
        Zero            =0x0B,
        Minus           =0x0C,    /* - on main keyboard */
        Equals          =0x0D,
        Backspace       =0x0E,    /* backspace */
        Tab             =0x0F,
        Q               =0x10,
        W               =0x11,
        E               =0x12,
        R               =0x13,
        T               =0x14,
        Y               =0x15,
        U               =0x16,
        I               =0x17,
        O               =0x18,
        P               =0x19,
        LeftBracket     =0x1A,
        RightBracket    =0x1B,
        Return          =0x1C,    /* Enter on main keyboard */
        LeftControl     =0x1D,
        A               =0x1E,
        S               =0x1F,
        D               =0x20,
        F               =0x21,
        G               =0x22,
        H               =0x23,
        J               =0x24,
        K               =0x25,
        L               =0x26,
        Semicolon       =0x27,
        Apostrophe      =0x28,
        Grave           =0x29,    /* accent grave */
        LeftShift       =0x2A,
        Backslash       =0x2B,
        Z               =0x2C,
        X               =0x2D,
        C               =0x2E,
        V               =0x2F,
        B               =0x30,
        N               =0x31,
        M               =0x32,
        Comma           =0x33,
        Period          =0x34,    /* . on main keyboard */
        Slash           =0x35,    /* '/' on main keyboard */
        RightShift      =0x36,
        Multiply        =0x37,    /* * on numeric keypad */
        LeftAlt        =0x38,    /* left Alt */
        Space           =0x39,
        Capital         =0x3A,
        F1              =0x3B,
        F2              =0x3C,
        F3              =0x3D,
        F4              =0x3E,
        F5              =0x3F,
        F6              =0x40,
        F7              =0x41,
        F8              =0x42,
        F9              =0x43,
        F10             =0x44,
        NumLock         =0x45,
        ScrollLock      =0x46,    /* Scroll Lock */
        Numpad7         =0x47,
        Numpad8         =0x48,
        Numpad9         =0x49,
        Subtract        =0x4A,    /* - on numeric keypad */
        Numpad4         =0x4B,
        Numpad5         =0x4C,
        Numpad6         =0x4D,
        Add             =0x4E,    /* + on numeric keypad */
        Numpad1         =0x4F,
        Numpad2         =0x50,
        Numpad3         =0x51,
        Numpad0         =0x52,
        Decimal         =0x53,    /* . on numeric keypad */
        OEM_102         =0x56,    /* < > | on UK/Germany keyboards */
        F11             =0x57,
        F12             =0x58,
        F13             =0x64,    /*                     (NEC PC98) */
        F14             =0x65,    /*                     (NEC PC98) */
        F15             =0x66,    /*                     (NEC PC98) */
        Kana            =0x70,    /* (Japanese keyboard)            */
        ABNT_C1         =0x73,    /* / ? on Portugese (Brazilian) keyboards */
        Convert         =0x79,    /* (Japanese keyboard)            */
        NoConvert       =0x7B,    /* (Japanese keyboard)            */
        Yen             =0x7D,    /* (Japanese keyboard)            */
        ABNT_C2         =0x7E,    /* Numpad . on Portugese (Brazilian) keyboards */
        NumpadEquals    =0x8D,    /* = on numeric keypad (NEC PC98) */
        PrevTrack       =0x90,    /* Previous Track (KC_CIRCUMFLEX on Japanese keyboard) */
        At              =0x91,    /*                     (NEC PC98) */
        Colon           =0x92,    /*                     (NEC PC98) */
        Underline       =0x93,    /*                     (NEC PC98) */
        Kanji           =0x94,    /* (Japanese keyboard)            */
        Stop            =0x95,    /*                     (NEC PC98) */
        AX              =0x96,    /*                     (Japan AX) */
        Unlabeled       =0x97,    /*                        (J3100) */
        NextTrack       =0x99,    /* Next Track */
        NumpadEnter     =0x9C,    /* Enter on numeric keypad */
        RightControl    =0x9D,
        Mute            =0xA0,    /* Mute */
        Calculator      =0xA1,    /* Calculator */
        PlayPause       =0xA2,    /* Play / Pause */
        MediaStop       =0xA4,    /* Media Stop */
        VolumeDown      =0xAE,    /* Volume - */
        VolumeUp        =0xB0,    /* Volume + */
        WebHome         =0xB2,    /* Web home */
        NumpadComma     =0xB3,    /* , on numeric keypad (NEC PC98) */
        Divide          =0xB5,    /* / on numeric keypad */
        SysRq           =0xB7,
        RightAlt        =0xB8,    /* right Alt */
        Pause           =0xC5,    /* Pause */
        Home            =0xC7,    /* Home on arrow keypad */
        ArrowUp         =0xC8,    /* UpArrow on arrow keypad */
        PageUp          =0xC9,    /* PgUp on arrow keypad */
        ArrowLeft       =0xCB,    /* LeftArrow on arrow keypad */
        ArrowRight      =0xCD,    /* RightArrow on arrow keypad */
        End             =0xCF,    /* End on arrow keypad */
        ArrowDown       =0xD0,    /* DownArrow on arrow keypad */
        PageDown        =0xD1,    /* PgDn on arrow keypad */
        Insert          =0xD2,    /* Insert on arrow keypad */
        Delete          =0xD3,    /* Delete on arrow keypad */
        LeftWindows     =0xDB,    /* Left Windows key */
        RightWindow     =0xDC,    /* Right Windows key */
        RightWindows    =0xDC,    /* Right Windows key - Correct spelling :) */
        AppMenu         =0xDD,    /* AppMenu key */
        Power           =0xDE,    /* System Power */
        Sleep           =0xDF,    /* System Sleep */
        Wake            =0xE3,    /* System Wake */
        WebSearch       =0xE5,    /* Web Search */
        WebFavorites    =0xE6,    /* Web Favorites */
        WebRefresh      =0xE7,    /* Web Refresh */
        WebStop         =0xE8,    /* Web Stop */
        WebForward      =0xE9,    /* Web Forward */
        WebBack         =0xEA,    /* Web Back */
        MyComputer      =0xEB,    /* My Computer */
        Mail            =0xEC,    /* Mail */
        MediaSelect     =0xED     /* Media Select */
    };
};

enum MouseButtonId
{
    MBID_UNKNOWN = 0,
    MBID_LEFT,
    MBID_MIDDLE,
    MBID_RIGHT
};

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