#pragma once

#include <Log.h>

#ifdef AL_DEBUG
#define AL_ASSERT(x, ...) if (!(x)) { LogError(__VA_ARGS__); exit(-1); }
#elif AL_RELEASE
#define AL_ASSERT(x, ...)
#endif
