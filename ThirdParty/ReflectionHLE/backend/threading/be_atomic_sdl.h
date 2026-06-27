#ifndef BE_ATOMIC_SDL_H
#define BE_ATOMIC_SDL_H

#include <SDL3/SDL_atomic.h>

typedef SDL_AtomicInt BE_ST_AtomicInt_T;

static int BE_ST_AtomicInt_Set(BE_ST_AtomicInt_T *a, int n) { return SDL_SetAtomicInt(a, n); }
static int BE_ST_AtomicInt_Get(BE_ST_AtomicInt_T *a) { return SDL_GetAtomicInt(a); }
static int BE_ST_AtomicInt_PostAdd(BE_ST_AtomicInt_T *a, int n) { return SDL_AddAtomicInt(a, n); }
static int BE_ST_AtomicInt_PostInc(BE_ST_AtomicInt_T *a) { return SDL_AddAtomicInt(a, 1); }

#endif
