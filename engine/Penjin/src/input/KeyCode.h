#pragma once

#include <cstddef>
#include <string>

#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_keyboard.h>

namespace Penjin {
    // Wraps SDL_Scancode (physical key position, layout-independent) rather
    // than SDL_Keycode (virtual/layout-dependent). Values are deliberately
    // identical to SDL's own scancode values, so any key SDL knows about is
    // representable and usable via fromScancode() - the named constants below
    // just give every one of them a readable name, mirroring SDL_scancode.h.
    enum class KeyCode : int {};

    inline constexpr KeyCode Unknown{SDL_SCANCODE_UNKNOWN};

    // Usage page 0x07 (USB keyboard page)
    inline constexpr KeyCode A{SDL_SCANCODE_A};
    inline constexpr KeyCode B{SDL_SCANCODE_B};
    inline constexpr KeyCode C{SDL_SCANCODE_C};
    inline constexpr KeyCode D{SDL_SCANCODE_D};
    inline constexpr KeyCode E{SDL_SCANCODE_E};
    inline constexpr KeyCode F{SDL_SCANCODE_F};
    inline constexpr KeyCode G{SDL_SCANCODE_G};
    inline constexpr KeyCode H{SDL_SCANCODE_H};
    inline constexpr KeyCode I{SDL_SCANCODE_I};
    inline constexpr KeyCode J{SDL_SCANCODE_J};
    inline constexpr KeyCode K{SDL_SCANCODE_K};
    inline constexpr KeyCode L{SDL_SCANCODE_L};
    inline constexpr KeyCode M{SDL_SCANCODE_M};
    inline constexpr KeyCode N{SDL_SCANCODE_N};
    inline constexpr KeyCode O{SDL_SCANCODE_O};
    inline constexpr KeyCode P{SDL_SCANCODE_P};
    inline constexpr KeyCode Q{SDL_SCANCODE_Q};
    inline constexpr KeyCode R{SDL_SCANCODE_R};
    inline constexpr KeyCode S{SDL_SCANCODE_S};
    inline constexpr KeyCode T{SDL_SCANCODE_T};
    inline constexpr KeyCode U{SDL_SCANCODE_U};
    inline constexpr KeyCode V{SDL_SCANCODE_V};
    inline constexpr KeyCode W{SDL_SCANCODE_W};
    inline constexpr KeyCode X{SDL_SCANCODE_X};
    inline constexpr KeyCode Y{SDL_SCANCODE_Y};
    inline constexpr KeyCode Z{SDL_SCANCODE_Z};

    inline constexpr KeyCode Num1{SDL_SCANCODE_1};
    inline constexpr KeyCode Num2{SDL_SCANCODE_2};
    inline constexpr KeyCode Num3{SDL_SCANCODE_3};
    inline constexpr KeyCode Num4{SDL_SCANCODE_4};
    inline constexpr KeyCode Num5{SDL_SCANCODE_5};
    inline constexpr KeyCode Num6{SDL_SCANCODE_6};
    inline constexpr KeyCode Num7{SDL_SCANCODE_7};
    inline constexpr KeyCode Num8{SDL_SCANCODE_8};
    inline constexpr KeyCode Num9{SDL_SCANCODE_9};
    inline constexpr KeyCode Num0{SDL_SCANCODE_0};

    inline constexpr KeyCode Return{SDL_SCANCODE_RETURN};
    inline constexpr KeyCode Escape{SDL_SCANCODE_ESCAPE};
    inline constexpr KeyCode Backspace{SDL_SCANCODE_BACKSPACE};
    inline constexpr KeyCode Tab{SDL_SCANCODE_TAB};
    inline constexpr KeyCode Space{SDL_SCANCODE_SPACE};

    inline constexpr KeyCode Minus{SDL_SCANCODE_MINUS};
    inline constexpr KeyCode Equals{SDL_SCANCODE_EQUALS};
    inline constexpr KeyCode LeftBracket{SDL_SCANCODE_LEFTBRACKET};
    inline constexpr KeyCode RightBracket{SDL_SCANCODE_RIGHTBRACKET};
    inline constexpr KeyCode Backslash{SDL_SCANCODE_BACKSLASH};
    inline constexpr KeyCode NonUsHash{SDL_SCANCODE_NONUSHASH};
    inline constexpr KeyCode Semicolon{SDL_SCANCODE_SEMICOLON};
    inline constexpr KeyCode Apostrophe{SDL_SCANCODE_APOSTROPHE};
    inline constexpr KeyCode Grave{SDL_SCANCODE_GRAVE};
    inline constexpr KeyCode Comma{SDL_SCANCODE_COMMA};
    inline constexpr KeyCode Period{SDL_SCANCODE_PERIOD};
    inline constexpr KeyCode Slash{SDL_SCANCODE_SLASH};

    inline constexpr KeyCode CapsLock{SDL_SCANCODE_CAPSLOCK};

    inline constexpr KeyCode F1{SDL_SCANCODE_F1};
    inline constexpr KeyCode F2{SDL_SCANCODE_F2};
    inline constexpr KeyCode F3{SDL_SCANCODE_F3};
    inline constexpr KeyCode F4{SDL_SCANCODE_F4};
    inline constexpr KeyCode F5{SDL_SCANCODE_F5};
    inline constexpr KeyCode F6{SDL_SCANCODE_F6};
    inline constexpr KeyCode F7{SDL_SCANCODE_F7};
    inline constexpr KeyCode F8{SDL_SCANCODE_F8};
    inline constexpr KeyCode F9{SDL_SCANCODE_F9};
    inline constexpr KeyCode F10{SDL_SCANCODE_F10};
    inline constexpr KeyCode F11{SDL_SCANCODE_F11};
    inline constexpr KeyCode F12{SDL_SCANCODE_F12};

    inline constexpr KeyCode PrintScreen{SDL_SCANCODE_PRINTSCREEN};
    inline constexpr KeyCode ScrollLock{SDL_SCANCODE_SCROLLLOCK};
    inline constexpr KeyCode Pause{SDL_SCANCODE_PAUSE};
    inline constexpr KeyCode Insert{SDL_SCANCODE_INSERT};
    inline constexpr KeyCode Home{SDL_SCANCODE_HOME};
    inline constexpr KeyCode PageUp{SDL_SCANCODE_PAGEUP};
    inline constexpr KeyCode Delete{SDL_SCANCODE_DELETE};
    inline constexpr KeyCode End{SDL_SCANCODE_END};
    inline constexpr KeyCode PageDown{SDL_SCANCODE_PAGEDOWN};
    inline constexpr KeyCode Right{SDL_SCANCODE_RIGHT};
    inline constexpr KeyCode Left{SDL_SCANCODE_LEFT};
    inline constexpr KeyCode Down{SDL_SCANCODE_DOWN};
    inline constexpr KeyCode Up{SDL_SCANCODE_UP};

    inline constexpr KeyCode NumLockClear{SDL_SCANCODE_NUMLOCKCLEAR};
    inline constexpr KeyCode KeypadDivide{SDL_SCANCODE_KP_DIVIDE};
    inline constexpr KeyCode KeypadMultiply{SDL_SCANCODE_KP_MULTIPLY};
    inline constexpr KeyCode KeypadMinus{SDL_SCANCODE_KP_MINUS};
    inline constexpr KeyCode KeypadPlus{SDL_SCANCODE_KP_PLUS};
    inline constexpr KeyCode KeypadEnter{SDL_SCANCODE_KP_ENTER};
    inline constexpr KeyCode Keypad1{SDL_SCANCODE_KP_1};
    inline constexpr KeyCode Keypad2{SDL_SCANCODE_KP_2};
    inline constexpr KeyCode Keypad3{SDL_SCANCODE_KP_3};
    inline constexpr KeyCode Keypad4{SDL_SCANCODE_KP_4};
    inline constexpr KeyCode Keypad5{SDL_SCANCODE_KP_5};
    inline constexpr KeyCode Keypad6{SDL_SCANCODE_KP_6};
    inline constexpr KeyCode Keypad7{SDL_SCANCODE_KP_7};
    inline constexpr KeyCode Keypad8{SDL_SCANCODE_KP_8};
    inline constexpr KeyCode Keypad9{SDL_SCANCODE_KP_9};
    inline constexpr KeyCode Keypad0{SDL_SCANCODE_KP_0};
    inline constexpr KeyCode KeypadPeriod{SDL_SCANCODE_KP_PERIOD};

    inline constexpr KeyCode NonUsBackslash{SDL_SCANCODE_NONUSBACKSLASH};
    inline constexpr KeyCode Application{SDL_SCANCODE_APPLICATION};
    inline constexpr KeyCode Power{SDL_SCANCODE_POWER};
    inline constexpr KeyCode KeypadEquals{SDL_SCANCODE_KP_EQUALS};
    inline constexpr KeyCode F13{SDL_SCANCODE_F13};
    inline constexpr KeyCode F14{SDL_SCANCODE_F14};
    inline constexpr KeyCode F15{SDL_SCANCODE_F15};
    inline constexpr KeyCode F16{SDL_SCANCODE_F16};
    inline constexpr KeyCode F17{SDL_SCANCODE_F17};
    inline constexpr KeyCode F18{SDL_SCANCODE_F18};
    inline constexpr KeyCode F19{SDL_SCANCODE_F19};
    inline constexpr KeyCode F20{SDL_SCANCODE_F20};
    inline constexpr KeyCode F21{SDL_SCANCODE_F21};
    inline constexpr KeyCode F22{SDL_SCANCODE_F22};
    inline constexpr KeyCode F23{SDL_SCANCODE_F23};
    inline constexpr KeyCode F24{SDL_SCANCODE_F24};
    inline constexpr KeyCode Execute{SDL_SCANCODE_EXECUTE};
    inline constexpr KeyCode Help{SDL_SCANCODE_HELP};
    inline constexpr KeyCode Menu{SDL_SCANCODE_MENU};
    inline constexpr KeyCode Select{SDL_SCANCODE_SELECT};
    inline constexpr KeyCode Stop{SDL_SCANCODE_STOP};
    inline constexpr KeyCode Again{SDL_SCANCODE_AGAIN};
    inline constexpr KeyCode Undo{SDL_SCANCODE_UNDO};
    inline constexpr KeyCode Cut{SDL_SCANCODE_CUT};
    inline constexpr KeyCode Copy{SDL_SCANCODE_COPY};
    inline constexpr KeyCode Paste{SDL_SCANCODE_PASTE};
    inline constexpr KeyCode Find{SDL_SCANCODE_FIND};
    inline constexpr KeyCode Mute{SDL_SCANCODE_MUTE};
    inline constexpr KeyCode VolumeUp{SDL_SCANCODE_VOLUMEUP};
    inline constexpr KeyCode VolumeDown{SDL_SCANCODE_VOLUMEDOWN};
    inline constexpr KeyCode KeypadComma{SDL_SCANCODE_KP_COMMA};
    inline constexpr KeyCode KeypadEqualsAs400{SDL_SCANCODE_KP_EQUALSAS400};

    inline constexpr KeyCode International1{SDL_SCANCODE_INTERNATIONAL1};
    inline constexpr KeyCode International2{SDL_SCANCODE_INTERNATIONAL2};
    inline constexpr KeyCode International3{SDL_SCANCODE_INTERNATIONAL3};
    inline constexpr KeyCode International4{SDL_SCANCODE_INTERNATIONAL4};
    inline constexpr KeyCode International5{SDL_SCANCODE_INTERNATIONAL5};
    inline constexpr KeyCode International6{SDL_SCANCODE_INTERNATIONAL6};
    inline constexpr KeyCode International7{SDL_SCANCODE_INTERNATIONAL7};
    inline constexpr KeyCode International8{SDL_SCANCODE_INTERNATIONAL8};
    inline constexpr KeyCode International9{SDL_SCANCODE_INTERNATIONAL9};
    inline constexpr KeyCode Lang1{SDL_SCANCODE_LANG1};
    inline constexpr KeyCode Lang2{SDL_SCANCODE_LANG2};
    inline constexpr KeyCode Lang3{SDL_SCANCODE_LANG3};
    inline constexpr KeyCode Lang4{SDL_SCANCODE_LANG4};
    inline constexpr KeyCode Lang5{SDL_SCANCODE_LANG5};
    inline constexpr KeyCode Lang6{SDL_SCANCODE_LANG6};
    inline constexpr KeyCode Lang7{SDL_SCANCODE_LANG7};
    inline constexpr KeyCode Lang8{SDL_SCANCODE_LANG8};
    inline constexpr KeyCode Lang9{SDL_SCANCODE_LANG9};

    inline constexpr KeyCode AltErase{SDL_SCANCODE_ALTERASE};
    inline constexpr KeyCode SysReq{SDL_SCANCODE_SYSREQ};
    inline constexpr KeyCode Cancel{SDL_SCANCODE_CANCEL};
    inline constexpr KeyCode Clear{SDL_SCANCODE_CLEAR};
    inline constexpr KeyCode Prior{SDL_SCANCODE_PRIOR};
    inline constexpr KeyCode Return2{SDL_SCANCODE_RETURN2};
    inline constexpr KeyCode Separator{SDL_SCANCODE_SEPARATOR};
    inline constexpr KeyCode Out{SDL_SCANCODE_OUT};
    inline constexpr KeyCode Oper{SDL_SCANCODE_OPER};
    inline constexpr KeyCode ClearAgain{SDL_SCANCODE_CLEARAGAIN};
    inline constexpr KeyCode CrSel{SDL_SCANCODE_CRSEL};
    inline constexpr KeyCode ExSel{SDL_SCANCODE_EXSEL};

    inline constexpr KeyCode Keypad00{SDL_SCANCODE_KP_00};
    inline constexpr KeyCode Keypad000{SDL_SCANCODE_KP_000};
    inline constexpr KeyCode ThousandsSeparator{SDL_SCANCODE_THOUSANDSSEPARATOR};
    inline constexpr KeyCode DecimalSeparator{SDL_SCANCODE_DECIMALSEPARATOR};
    inline constexpr KeyCode CurrencyUnit{SDL_SCANCODE_CURRENCYUNIT};
    inline constexpr KeyCode CurrencySubunit{SDL_SCANCODE_CURRENCYSUBUNIT};
    inline constexpr KeyCode KeypadLeftParen{SDL_SCANCODE_KP_LEFTPAREN};
    inline constexpr KeyCode KeypadRightParen{SDL_SCANCODE_KP_RIGHTPAREN};
    inline constexpr KeyCode KeypadLeftBrace{SDL_SCANCODE_KP_LEFTBRACE};
    inline constexpr KeyCode KeypadRightBrace{SDL_SCANCODE_KP_RIGHTBRACE};
    inline constexpr KeyCode KeypadTab{SDL_SCANCODE_KP_TAB};
    inline constexpr KeyCode KeypadBackspace{SDL_SCANCODE_KP_BACKSPACE};
    inline constexpr KeyCode KeypadA{SDL_SCANCODE_KP_A};
    inline constexpr KeyCode KeypadB{SDL_SCANCODE_KP_B};
    inline constexpr KeyCode KeypadC{SDL_SCANCODE_KP_C};
    inline constexpr KeyCode KeypadD{SDL_SCANCODE_KP_D};
    inline constexpr KeyCode KeypadE{SDL_SCANCODE_KP_E};
    inline constexpr KeyCode KeypadF{SDL_SCANCODE_KP_F};
    inline constexpr KeyCode KeypadXor{SDL_SCANCODE_KP_XOR};
    inline constexpr KeyCode KeypadPower{SDL_SCANCODE_KP_POWER};
    inline constexpr KeyCode KeypadPercent{SDL_SCANCODE_KP_PERCENT};
    inline constexpr KeyCode KeypadLess{SDL_SCANCODE_KP_LESS};
    inline constexpr KeyCode KeypadGreater{SDL_SCANCODE_KP_GREATER};
    inline constexpr KeyCode KeypadAmpersand{SDL_SCANCODE_KP_AMPERSAND};
    inline constexpr KeyCode KeypadDblAmpersand{SDL_SCANCODE_KP_DBLAMPERSAND};
    inline constexpr KeyCode KeypadVerticalBar{SDL_SCANCODE_KP_VERTICALBAR};
    inline constexpr KeyCode KeypadDblVerticalBar{SDL_SCANCODE_KP_DBLVERTICALBAR};
    inline constexpr KeyCode KeypadColon{SDL_SCANCODE_KP_COLON};
    inline constexpr KeyCode KeypadHash{SDL_SCANCODE_KP_HASH};
    inline constexpr KeyCode KeypadSpace{SDL_SCANCODE_KP_SPACE};
    inline constexpr KeyCode KeypadAt{SDL_SCANCODE_KP_AT};
    inline constexpr KeyCode KeypadExclam{SDL_SCANCODE_KP_EXCLAM};
    inline constexpr KeyCode KeypadMemStore{SDL_SCANCODE_KP_MEMSTORE};
    inline constexpr KeyCode KeypadMemRecall{SDL_SCANCODE_KP_MEMRECALL};
    inline constexpr KeyCode KeypadMemClear{SDL_SCANCODE_KP_MEMCLEAR};
    inline constexpr KeyCode KeypadMemAdd{SDL_SCANCODE_KP_MEMADD};
    inline constexpr KeyCode KeypadMemSubtract{SDL_SCANCODE_KP_MEMSUBTRACT};
    inline constexpr KeyCode KeypadMemMultiply{SDL_SCANCODE_KP_MEMMULTIPLY};
    inline constexpr KeyCode KeypadMemDivide{SDL_SCANCODE_KP_MEMDIVIDE};
    inline constexpr KeyCode KeypadPlusMinus{SDL_SCANCODE_KP_PLUSMINUS};
    inline constexpr KeyCode KeypadClear{SDL_SCANCODE_KP_CLEAR};
    inline constexpr KeyCode KeypadClearEntry{SDL_SCANCODE_KP_CLEARENTRY};
    inline constexpr KeyCode KeypadBinary{SDL_SCANCODE_KP_BINARY};
    inline constexpr KeyCode KeypadOctal{SDL_SCANCODE_KP_OCTAL};
    inline constexpr KeyCode KeypadDecimal{SDL_SCANCODE_KP_DECIMAL};
    inline constexpr KeyCode KeypadHexadecimal{SDL_SCANCODE_KP_HEXADECIMAL};

    inline constexpr KeyCode LeftCtrl{SDL_SCANCODE_LCTRL};
    inline constexpr KeyCode LeftShift{SDL_SCANCODE_LSHIFT};
    inline constexpr KeyCode LeftAlt{SDL_SCANCODE_LALT};
    inline constexpr KeyCode LeftGui{SDL_SCANCODE_LGUI};
    inline constexpr KeyCode RightCtrl{SDL_SCANCODE_RCTRL};
    inline constexpr KeyCode RightShift{SDL_SCANCODE_RSHIFT};
    inline constexpr KeyCode RightAlt{SDL_SCANCODE_RALT};
    inline constexpr KeyCode RightGui{SDL_SCANCODE_RGUI};

    inline constexpr KeyCode Mode{SDL_SCANCODE_MODE};

    // Usage page 0x0C (USB consumer page)
    inline constexpr KeyCode AudioNext{SDL_SCANCODE_MEDIA_NEXT_TRACK};
    inline constexpr KeyCode AudioPrev{SDL_SCANCODE_MEDIA_PREVIOUS_TRACK};
    inline constexpr KeyCode AudioStop{SDL_SCANCODE_MEDIA_STOP};
    inline constexpr KeyCode AudioPlay{SDL_SCANCODE_MEDIA_PLAY};
    inline constexpr KeyCode AudioMute{SDL_SCANCODE_MUTE};
    inline constexpr KeyCode AcSearch{SDL_SCANCODE_AC_SEARCH};
    inline constexpr KeyCode AcHome{SDL_SCANCODE_AC_HOME};
    inline constexpr KeyCode AcBack{SDL_SCANCODE_AC_BACK};
    inline constexpr KeyCode AcForward{SDL_SCANCODE_AC_FORWARD};
    inline constexpr KeyCode AcStop{SDL_SCANCODE_AC_STOP};
    inline constexpr KeyCode AcRefresh{SDL_SCANCODE_AC_REFRESH};
    inline constexpr KeyCode AcBookmarks{SDL_SCANCODE_AC_BOOKMARKS};

    // Usage page 0x0C (additional media keys)
    inline constexpr KeyCode AudioRewind{SDL_SCANCODE_MEDIA_REWIND};
    inline constexpr KeyCode AudioFastForward{SDL_SCANCODE_MEDIA_FAST_FORWARD};

    // Mobile keys
    inline constexpr KeyCode SoftLeft{SDL_SCANCODE_SOFTLEFT};
    inline constexpr KeyCode SoftRight{SDL_SCANCODE_SOFTRIGHT};
    inline constexpr KeyCode Call{SDL_SCANCODE_CALL};
    inline constexpr KeyCode EndCall{SDL_SCANCODE_ENDCALL};

    constexpr std::size_t KeyCodeCount = SDL_SCANCODE_COUNT;

    constexpr KeyCode fromScancode(SDL_Scancode scancode) {
        return static_cast<KeyCode>(scancode);
    }

    // Layout-aware label for displaying the key in UI (e.g. a keybinding
    // menu) - unlike the scancode itself, this reflects what's actually
    // printed on the user's keyboard under their current OS layout.
    inline std::string displayName(KeyCode key) {
        const auto scancode = static_cast<SDL_Scancode>(key);
        const SDL_Keycode virtualKey = SDL_GetKeyFromScancode(scancode, SDL_KMOD_NONE, false);
        return SDL_GetKeyName(virtualKey);
    }
}
