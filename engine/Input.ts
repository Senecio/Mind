namespace Mind {

export class Input {
    
    _keysDown : Object = {};
    
    constructor() {
    
    }
    
    OnKeyDown(keyscan : Input.Scan) {
        this._keysDown[keyscan] = true;
    }
    
    OnKeyUp(keyscan : Input.Scan) {
        this._keysDown[keyscan] = false;
    }
    
    IsKeyDown(keyscan : Input.Scan) {
        if (typeof this._keysDown[keyscan] === 'undefined')
            return false;

        return this._keysDown[keyscan];
    }
    
    KeyboardEventCodeToScan(event : KeyboardEvent) : Input.Scan {
        switch (event.keyCode) {
            case 8: return Input.Scan.Backspace;    // "backspace"
            case 9: return Input.Scan.Tab;          // "tab",
            case 13: return Input.Scan.Return;      // "return",
            case 16: if (event.location === 2) { return Input.Scan.RightShift; } else { return Input.Scan.LeftShift; };  // "shift",
            case 17: if (event.location === 2) { return Input.Scan.RightControl; } else { return Input.Scan.LeftControl; };// "ctrl",
            case 18: if (event.location === 2) { return Input.Scan.RightAlt; } else { return Input.Scan.LeftAlt; };   // "alt",
            case 19: return Input.Scan.Pause;       // "pause",
            case 20: return Input.Scan.Capital;     // "capslock",
            case 27: return Input.Scan.Escape;      // "escape",
            case 33: return Input.Scan.PageUp;      // "pageup",
            case 34: return Input.Scan.PageDown;    // "pagedown",
            case 35: return Input.Scan.End;         // "end",
            case 36: return Input.Scan.Home;        // "home",
            case 45: return Input.Scan.Insert;      // "insert",
            case 46: return Input.Scan.Delete;      // "delete",
            case 37: return Input.Scan.ArrowLeft;   // "left",
            case 38: return Input.Scan.ArrowUp;     // "up",
            case 39: return Input.Scan.ArrowRight;  // "right",
            case 40: return Input.Scan.ArrowDown;   // "down",
            case 91: return Input.Scan.LeftWindows; // "lmeta",
            case 92: return Input.Scan.RightWindow; // "rmeta",
            case 93: return Input.Scan.AppMenu;     // "mode",
            case 96: return Input.Scan.Numpad0;     // "kp0",
            case 97: return Input.Scan.Numpad1;     // "kp1",
            case 98: return Input.Scan.Numpad2;     // "kp2",
            case 99: return Input.Scan.Numpad3;     // "kp3",
            case 100: return Input.Scan.Numpad4;    // "kp4",
            case 101: return Input.Scan.Numpad5;    // "kp5",
            case 102: return Input.Scan.Numpad6;    // "kp6",
            case 103: return Input.Scan.Numpad7;    // "kp7",
            case 104: return Input.Scan.Numpad8;    // "kp8",
            case 105: return Input.Scan.Numpad9;    // "kp9",
            case 106: return Input.Scan.Multiply;   // "kp*",
            case 107: return Input.Scan.Add;        // "kp+",
            case 109: return Input.Scan.Subtract;   // "kp-",
            case 110: return Input.Scan.Decimal;    // "kp.",
            case 111: return Input.Scan.Divide;     // "kp/",
            case 112: return Input.Scan.F1;         // "f1",
            case 113: return Input.Scan.F2;         // "f2",
            case 114: return Input.Scan.F3;         // "f3",
            case 115: return Input.Scan.F4;         // "f4",
            case 116: return Input.Scan.F5;         // "f5",
            case 117: return Input.Scan.F6;         // "f6",
            case 118: return Input.Scan.F7;         // "f7",
            case 119: return Input.Scan.F8;         // "f8",
            case 120: return Input.Scan.F9;         // "f9",
            case 121: return Input.Scan.F10;        // "f10",
            case 122: return Input.Scan.F11;        // "f11",
            case 123: return Input.Scan.F12;        // "f12",
            case 144: return Input.Scan.NumLock;    // "numlock",
            case 145: return Input.Scan.ScrollLock; // "scrolllock",
            case 186: return Input.Scan.Semicolon;  // ";",
            case 187: return Input.Scan.Equals;     // "=",
            case 188: return Input.Scan.Comma;      // "<",
            case 189: return Input.Scan.Minus;      // "-",
            case 190: return Input.Scan.Period;     // ">",
            case 191: return Input.Scan.Slash;      // "/",
            case 192: return Input.Scan.Grave;      // "`",
            case 219: return Input.Scan.RightBracket; // "[",
            case 220: return Input.Scan.Backslash;  // "\\",
            case 221: return Input.Scan.LeftBracket;// "]",
            case 222: return Input.Scan.Apostrophe; // "'",
            case 65 : return Input.Scan.A;          // "a",
            case 66 : return Input.Scan.B;          // "b",
            case 67 : return Input.Scan.C;          // "c",
            case 68 : return Input.Scan.D;          // "d",
            case 69 : return Input.Scan.E;          // "e",
            case 70 : return Input.Scan.F;          // "f",
            case 71 : return Input.Scan.G;          // "g",
            case 72 : return Input.Scan.H;          // "h",
            case 73 : return Input.Scan.I;          // "i",
            case 74 : return Input.Scan.J;          // "j",
            case 75 : return Input.Scan.K;          // "k",
            case 76 : return Input.Scan.L;          // "l",
            case 77 : return Input.Scan.M;          // "m",
            case 78 : return Input.Scan.N;          // "n",
            case 79 : return Input.Scan.O;          // "o",
            case 80 : return Input.Scan.P;          // "p",
            case 81 : return Input.Scan.Q;          // "q",
            case 82 : return Input.Scan.R;          // "r",
            case 83 : return Input.Scan.S;          // "s",
            case 84 : return Input.Scan.T;          // "t",
            case 85 : return Input.Scan.U;          // "u",
            case 86 : return Input.Scan.V;          // "v",
            case 87 : return Input.Scan.W;          // "w",
            case 88 : return Input.Scan.X;          // "x",
            case 89 : return Input.Scan.Y;          // "y",
            case 90 : return Input.Scan.Z;          // "z",
        }
        return Input.Scan.None;
    }
    
    _mouseButtons : Object = {};
    _mousePositionX : number = 0;
    _mousePositionY : number = 0;
    
    OnMouseMove(x : number, y : number) {
        this._mousePositionX = x;
        this._mousePositionY = y;
    }
    
    OnMouseDown(button : Input.MouseButton, x : number, y : number) {
        this._mouseButtons[button] = true;
        this._mousePositionX = x;
        this._mousePositionY = y;
    }
    
    OnMouseUp(button : Input.MouseButton, x : number, y : number) {
        this._mouseButtons[button] = false;
        this._mousePositionX = x;
        this._mousePositionY = y;
    }
    
    IsMouseDown(button : Input.MouseButton) {
        return this._mouseButtons[button];
    }
    
    GetMousePosition() {
        return [this._mousePositionX, this._mousePositionY];
    }
    
    MouseEventToMouseButton(event : MouseEvent) {
        if (event.button === 0) return Input.MouseButton.Left;
        else if (event.button === 1) return Input.MouseButton.Middle;
        else if (event.button === 2) return Input.MouseButton.Right;
        else return Input.MouseButton.None;
    }
}

export module Input {
    export enum Scan
    {
        None            =0,
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
    }
    
    export enum MouseButton {
        None,
        Left,
        Middle,
        Right
    }
}

} // end Mind