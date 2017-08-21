/*
Simple DirectMedia Layer
Copyright (C) 2009-2014 Sergii Pylypenko

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required. 
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
/*
This source code is distibuted under ZLIB license, however when compiling with SDL 1.2,
which is licensed under LGPL, the resulting library, and all it's source code,
falls under "stronger" LGPL terms, so is this file.
If you compile this code with SDL 1.3 or newer, or use in some other way, the license stays ZLIB.
*/

#ifndef _SDL_ANDROIDINPUT_H_
#define _SDL_ANDROIDINPUT_H_

#include "SDL_config.h"

#include "SDL_version.h"
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "SDL_mutex.h"
#include "SDL_thread.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "SDL_events.h"
#if (SDL_VERSION_ATLEAST(1,3,0))
#include "../../events/SDL_events_c.h"
#include "../../events/SDL_keyboard_c.h"
#include "../../events/SDL_mouse_c.h"
#include "SDL_keycode.h"
#include "SDL_scancode.h"
//#include "SDL_compat.h"
#else
#include "SDL_keysym.h"
#include "../../events/SDL_events_c.h"
#endif
#include "SDL_joystick.h"
#include "../../joystick/SDL_sysjoystick.h"
#include "../../joystick/SDL_joystick_c.h"

#include "../SDL_sysvideo.h"
#include "SDL_androidvideo.h"
#include "javakeycodes.h"

/* JNI-C++ wrapper stuff */

enum
{
	// Mouse buttons can be mapped to on-screen keys
	SDLK_MOUSE_LEFT = 500,
	SDLK_MOUSE_MIDDLE = 501,
	SDLK_MOUSE_RIGHT = 502,
	SDLK_MOUSE_WHEEL_UP = 503,
	SDLK_MOUSE_WHEEL_DOWN = 504,
	SDLK_MOUSE_X1 = 505,
	SDLK_MOUSE_X2 = 506,

	// Special key to signal that key should be handled by Java internally, such as Volume Up/Down keys
	SDLK_NO_REMAP = 512,
};

#define SDL_SCANCODE_NO_REMAP SDLK_NO_REMAP

#if SDL_VERSION_ATLEAST(1,3,0)

#define SDL_KEY2(X) SDL_SCANCODE_ ## X
#define SDL_KEY(X) SDL_KEY2(X)
typedef SDL_Scancode SDL_scancode;
typedef SDL_Keycode SDLKey;
typedef SDL_Keysym SDL_keysym;

#else

#define SDL_KEY2(X) SDLK_ ## X
#define SDL_KEY(X) SDL_KEY2(X)

// Randomly redefining SDL 1.3 scancodes to SDL 1.2 keycodes
#define KP_0 KP0
#define KP_1 KP1
#define KP_2 KP2
#define KP_3 KP3
#define KP_4 KP4
#define KP_5 KP5
#define KP_6 KP6
#define KP_7 KP7
#define KP_8 KP8
#define KP_9 KP9
#define NUMLOCKCLEAR NUMLOCK
#define GRAVE BACKQUOTE
#define APOSTROPHE QUOTE
#define LGUI LSUPER
#define RGUI RSUPER
#define SCROLLLOCK SCROLLOCK
// Overkill haha
#define A a
#define B b
#define C c
#define D d
#define E e
#define F f
#define G g
#define H h
#define I i
#define J j
#define K k
#define L l
#define M m
#define N n
#define O o
#define P p
#define Q q
#define R r
#define S s
#define T t
#define U u
#define V v
#define W w
#define X x
#define Y y
#define Z z

typedef SDLKey SDL_scancode;
#define SDL_GetKeyboardState SDL_GetKeyState

#endif

#define SDL_KEY_VAL(X) X

enum MOUSE_ACTION { MOUSE_DOWN = 0, MOUSE_UP = 1, MOUSE_MOVE = 2, MOUSE_HOVER = 3 };
enum { TOUCHSCREEN_KEYBOARD_PASS_EVENT_DOWN_TO_SDL = 0x40000000 };
extern unsigned SDL_ANDROID_processTouchscreenKeyboard(int x, int y, int action, int pointerId);
extern int SDL_ANDROID_isTouchscreenKeyboardUsed;

// Default button mapping is rather random, make sure to redefine it inside AndroidAppSettings.cfg
#ifndef SDL_ANDROID_KEYCODE_0
#define SDL_ANDROID_KEYCODE_0 RETURN
#endif
#ifndef SDL_ANDROID_KEYCODE_1
#define SDL_ANDROID_KEYCODE_1 END
#endif
#ifndef SDL_ANDROID_KEYCODE_2
#define SDL_ANDROID_KEYCODE_2 NO_REMAP
#endif
#ifndef SDL_ANDROID_KEYCODE_3
#define SDL_ANDROID_KEYCODE_3 NO_REMAP
#endif
#ifndef SDL_ANDROID_KEYCODE_4
#define SDL_ANDROID_KEYCODE_4 LCTRL
#endif
#ifndef SDL_ANDROID_KEYCODE_5
#define SDL_ANDROID_KEYCODE_5 ESCAPE
#endif
#ifndef SDL_ANDROID_KEYCODE_6
#define SDL_ANDROID_KEYCODE_6 RSHIFT
#endif
#ifndef SDL_ANDROID_KEYCODE_7
#define SDL_ANDROID_KEYCODE_7 SDL_ANDROID_KEYCODE_1
#endif
#ifndef SDL_ANDROID_KEYCODE_8
#define SDL_ANDROID_KEYCODE_8 LALT
#endif
#ifndef SDL_ANDROID_KEYCODE_9
#define SDL_ANDROID_KEYCODE_9 RALT
#endif

// Touchscreen keyboard keys + zoom and rotate keycodes
#ifndef SDL_ANDROID_SCREENKB_KEYCODE_0
#define SDL_ANDROID_SCREENKB_KEYCODE_0 SDL_ANDROID_KEYCODE_0
#endif
#ifndef SDL_ANDROID_SCREENKB_KEYCODE_1
#define SDL_ANDROID_SCREENKB_KEYCODE_1 SDL_ANDROID_KEYCODE_1
#endif
#ifndef SDL_ANDROID_SCREENKB_KEYCODE_2
#define SDL_ANDROID_SCREENKB_KEYCODE_2 PAGEUP
#endif
#ifndef SDL_ANDROID_SCREENKB_KEYCODE_3
#define SDL_ANDROID_SCREENKB_KEYCODE_3 PAGEDOWN
#endif
#ifndef SDL_ANDROID_SCREENKB_KEYCODE_4
#define SDL_ANDROID_SCREENKB_KEYCODE_4 SDL_ANDROID_KEYCODE_6
#endif
#ifndef SDL_ANDROID_SCREENKB_KEYCODE_5
#define SDL_ANDROID_SCREENKB_KEYCODE_5 SDL_ANDROID_KEYCODE_7
#endif
#ifndef SDL_ANDROID_SCREENKB_KEYCODE_6
#define SDL_ANDROID_SCREENKB_KEYCODE_6 UNKNOWN
#endif
#ifndef SDL_ANDROID_SCREENKB_KEYCODE_7
#define SDL_ANDROID_SCREENKB_KEYCODE_7 UNKNOWN
#endif
#ifndef SDL_ANDROID_SCREENKB_KEYCODE_8
#define SDL_ANDROID_SCREENKB_KEYCODE_8 UNKNOWN
#endif
#ifndef SDL_ANDROID_SCREENKB_KEYCODE_9
#define SDL_ANDROID_SCREENKB_KEYCODE_9 UNKNOWN
#endif

// Gamepad keys
#ifndef SDL_ANDROID_GAMEPAD_KEYCODE_0
#define SDL_ANDROID_GAMEPAD_KEYCODE_0 SDL_ANDROID_SCREENKB_KEYCODE_0
#endif
#ifndef SDL_ANDROID_GAMEPAD_KEYCODE_1
#define SDL_ANDROID_GAMEPAD_KEYCODE_1 SDL_ANDROID_SCREENKB_KEYCODE_1
#endif
#ifndef SDL_ANDROID_GAMEPAD_KEYCODE_2
#define SDL_ANDROID_GAMEPAD_KEYCODE_2 SDL_ANDROID_SCREENKB_KEYCODE_2
#endif
#ifndef SDL_ANDROID_GAMEPAD_KEYCODE_3
#define SDL_ANDROID_GAMEPAD_KEYCODE_3 SDL_ANDROID_SCREENKB_KEYCODE_3
#endif
#ifndef SDL_ANDROID_GAMEPAD_KEYCODE_4
#define SDL_ANDROID_GAMEPAD_KEYCODE_4 SDL_ANDROID_SCREENKB_KEYCODE_5
#endif
#ifndef SDL_ANDROID_GAMEPAD_KEYCODE_5
#define SDL_ANDROID_GAMEPAD_KEYCODE_5 SDL_ANDROID_SCREENKB_KEYCODE_4
#endif
#ifndef SDL_ANDROID_GAMEPAD_KEYCODE_6
#define SDL_ANDROID_GAMEPAD_KEYCODE_6 SDL_ANDROID_KEYCODE_0
#endif
#ifndef SDL_ANDROID_GAMEPAD_KEYCODE_7
#define SDL_ANDROID_GAMEPAD_KEYCODE_7 SDL_ANDROID_KEYCODE_6
#endif
#ifndef SDL_ANDROID_GAMEPAD_KEYCODE_8
#define SDL_ANDROID_GAMEPAD_KEYCODE_8 SPACE
#endif
#ifndef SDL_ANDROID_GAMEPAD_KEYCODE_9
#define SDL_ANDROID_GAMEPAD_KEYCODE_9 RETURN
#endif

// Queue events to main thread
extern void SDL_ANDROID_MainThreadPushMouseMotion(int x, int y);
extern void SDL_ANDROID_MainThreadPushMouseButton(int pressed, int button);
extern void SDL_ANDROID_MainThreadPushKeyboardKey(int pressed, SDL_scancode key, int unicode);
extern void SDL_ANDROID_MainThreadPushMultitouchButton(int id, int pressed, int x, int y, int force); // SDL 1.3 only
extern void SDL_ANDROID_MainThreadPushMultitouchMotion(int id, int x, int y, int force); // SDL 1.3 only
extern void SDL_ANDROID_MainThreadPushJoystickAxis(int joy, int axis, int value);
extern void SDL_ANDROID_MainThreadPushJoystickButton(int joy, int button, int pressed);
extern void SDL_ANDROID_MainThreadPushJoystickBall(int joy, int ball, int x, int y);
extern void SDL_ANDROID_MainThreadPushText( int ascii, int unicode );
extern void SDL_android_init_keymap(SDLKey *SDL_android_keymap);
extern void SDL_ANDROID_MainThreadPushMouseWheel( int x, int y ); // SDL 1.3 only
extern void SDL_ANDROID_MainThreadPushAppActive(int active);

// Internal input queue stuff
extern SDLKey SDL_android_keymap[];
extern int SDL_android_KeysymFromUnicodeShiftNeeded(int unicode);
extern unsigned char SDL_android_keysym_to_scancode[SDLK_LAST];
extern SDL_Joystick *SDL_ANDROID_CurrentJoysticks[];
extern int SDL_ANDROID_isMouseUsed;
extern int SDL_ANDROID_currentMouseX, SDL_ANDROID_currentMouseY, SDL_ANDROID_currentMouseButtons;
extern int SDL_ANDROID_moveMouseWithArrowKeys;
extern int SDL_ANDROID_moveMouseWithKbSpeed;
extern int SDL_ANDROID_moveMouseWithKbAccel;
extern int SDL_ANDROID_moveMouseWithKbActive;
extern int SDL_ANDROID_moveMouseWithKbX, SDL_ANDROID_moveMouseWithKbY;
extern int SDL_ANDROID_moveMouseWithKbSpeedX, SDL_ANDROID_moveMouseWithKbSpeedY;
extern int SDL_ANDROID_moveMouseWithKbAccelX, SDL_ANDROID_moveMouseWithKbAccelY;
extern int SDL_ANDROID_moveMouseWithKbAccelUpdateNeeded;
extern int SDL_ANDROID_SystemBarAndKeyboardShown;

#endif
