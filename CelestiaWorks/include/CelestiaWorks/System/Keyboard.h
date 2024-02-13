#pragma once
//#include <Windows.h>

#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif


namespace celestia
{
	namespace Keyboard
	{
        enum class Key {
            BACKSPACE = 0x08,
            TAB = 0x09,
            ENTER = 0x0D,
            SHIFT = 0x10,
            CAPS_LOCK = 0x14,
            ESCAPE = 0x1B,
            SPACE = 0x20,
            ZERO = '0',
            ONE = '1',
            TWO = '2',
            THREE = '3',
            FOUR = '4',
            FIVE = '5',
            SIX = '6',
            SEVEN = '7',
            EIGHT = '8',
            NINE = '9',
            A = 'A',
            B = 'B',
            C = 'C',
            D = 'D',
            E = 'E',
            F = 'F',
            G = 'G',
            H = 'H',
            I = 'I',
            J = 'J',
            K = 'K',
            L = 'L',
            M = 'M',
            N = 'N',
            O = 'O',
            P = 'P',
            Q = 'Q',
            R = 'R',
            S = 'S',
            T = 'T',
            U = 'U',
            V = 'V',
            W = 'W',
            X = 'X',
            Y = 'Y',
            Z = 'Z',
            LEFT_ARROW = 0x25,
            UP_ARROW = 0x26,
            RIGHT_ARROW = 0x27,
            DOWN_ARROW = 0x28,
            INSERT = 0x2D,
            DELETE_KEY = 0x2E,
            PAGE_UP = 0x21,
            PAGE_DOWN = 0x22,
            END = 0x23,
            HOME = 0x24,
            LEFT_CTRL = 0xA2,
            RIGHT_CTRL = 0xA3,
            LEFT_ALT = 0xA4,
            RIGHT_ALT = 0xA5
        };

        CELESTIA_WORKS bool isKeyPressed(Key key);

	}
}