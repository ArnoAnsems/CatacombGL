// This header MUST be included early, so macros can be properly used

#ifndef REFKEEN_CONFIG_H
#define REFKEEN_CONFIG_H

// BIG ***FIXME*** - Should probably be externally generated instead
#include "SDL_endian.h"
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define REFKEEN_ARCH_LITTLE_ENDIAN
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
#define REFKEEN_ARCH_BIG_ENDIAN
#endif

#define REFKEEN_RESAMPLER_NONE

// Note: There may be multiple platform definitions at once,
// say REFKEEN_PLATFORM_ANDROID/EMSCRIPTEN and REFKEEN_PLATFORM_UNIX.

#ifdef _WIN32 // Should also cover _WIN64
#define REFKEEN_PLATFORM_WINDOWS
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#endif

#ifdef __APPLE__
#include "TargetConditionals.h"
// Depending on the SDK version, TARGET_OS_IPHONE may either be
// undefined, or defined to 0. So check the macros this way.
#if (!TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR)
#define REFKEEN_PLATFORM_MACOS
#endif
#endif

#if (defined ANDROID) || (defined __ANDROID__)
#define REFKEEN_PLATFORM_ANDROID
#endif

#ifdef EMSCRIPTEN
#define REFKEEN_PLATFORM_EMSCRIPTEN
#endif

#if (defined __unix__) || (defined __unix) || (defined unix) || (defined __APPLE__)
#define REFKEEN_PLATFORM_UNIX
#endif

#ifndef REFKEEN_PLATFORM_EMSCRIPTEN
#define REFKEEN_CONFIG_THREADS
#endif

#if (!defined REFKEEN_PLATFORM_ANDROID) && (!defined REFKEEN_PLATFORM_EMSCRIPTEN)
#define REFKEEN_CONFIG_USER_FULLSCREEN_TOGGLE
#define REFKEEN_CONFIG_USER_FULLSCREEN_RES_SETTING
#define REFKEEN_CONFIG_LAUNCHER_WINDOWTYPE_MENUITEM
#define REFKEEN_CONFIG_CHECK_FOR_STEAM_INSTALLATION
#define REFKEEN_CONFIG_ENABLE_CMDLINE
#endif

// On the Mac, usage of multitouch trackpad may lead to SDL2
// finger events, and these should not be mistakenly
// processed as touchscreen input
#ifndef REFKEEN_PLATFORM_MACOS
#define REFKEEN_CONFIG_ENABLE_TOUCHINPUT
#endif

#ifdef REFKEEN_PLATFORM_ANDROID
// On some platforms e.g., OS X, usage of multitouch trackpad may lead
// to SDL2 finger events, and these should not be mistakenly detected
// as touchscreen input (at least by default)
#define REFKEEN_CONFIG_AUTODETECT_TOUCHINPUT_BY_DEFAULT

#define REFKEEN_CONFIG_RESET_SDL_HINT_ACCELEROMETER_AS_JOYSTICK
#define REFKEEN_CONFIG_REQUIRE_LANDSCAPE_ORIENTATION
#define REFKEEN_CONFIG_EVENTS_CALLBACK
#endif

// Sanity check
#if (!defined REFKEEN_CONFIG_THREADS) && (defined REFKEEN_CONFIG_EVENTS_CALLBACK)
#error "REFKEEN sanity check failed - REFKEEN_CONFIG_EVENTS_CALLBACK is defined, but REFKEEN_CONFIG_THREADS is not!"
#endif

#endif // REFKEEN_CONFIG_H
