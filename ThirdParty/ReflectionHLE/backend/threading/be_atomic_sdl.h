#ifndef BE_ATOMIC_SDL_H
#define BE_ATOMIC_SDL_H

#include "SDL_atomic.h"

typedef SDL_atomic_t BE_ST_AtomicInt_T;

static int BE_ST_AtomicInt_Set(BE_ST_AtomicInt_T *a, int n) { return SDL_AtomicSet(a, n); }
static int BE_ST_AtomicInt_Get(BE_ST_AtomicInt_T *a) { return SDL_AtomicGet(a); }
static int BE_ST_AtomicInt_PostAdd(BE_ST_AtomicInt_T *a, int n) { return SDL_AtomicAdd(a, n); }
static int BE_ST_AtomicInt_PostInc(BE_ST_AtomicInt_T *a) { return SDL_AtomicAdd(a, 1); }

#endif
