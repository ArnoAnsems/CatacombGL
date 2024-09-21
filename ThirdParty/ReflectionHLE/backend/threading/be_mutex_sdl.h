#ifndef BE_MUTEX_SDL_H
#define BE_MUTEX_SDL_H

#include <assert.h>
#include "SDL_mutex.h"

typedef SDL_mutex BE_ST_Mutex_T;

static inline BE_ST_Mutex_T *BE_ST_CreateMutex(void) { return SDL_CreateMutex(); }
static inline void BE_ST_FreeMutex(BE_ST_Mutex_T *mutex) { SDL_DestroyMutex(mutex); }
static inline void BE_ST_LockMutexRecursively(BE_ST_Mutex_T *mutex) { assert(!SDL_LockMutex(mutex)); }
static inline void BE_ST_UnlockMutexRecursively(BE_ST_Mutex_T *mutex) { assert(!SDL_UnlockMutex(mutex)); }

#endif
