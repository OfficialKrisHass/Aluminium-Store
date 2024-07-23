#pragma once

#ifdef AL_DEBUG
#define AL_ASSERT(x, msg) if (!(x)) { std::cout << msg << "\n"; exit(-1); }
#elif AL_RELEASE
#define AL_ASSERT(x, msg)
#endif
