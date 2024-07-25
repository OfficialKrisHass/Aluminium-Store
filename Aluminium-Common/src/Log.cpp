#include <Log.h>

#include <fmt/color.h>

#include <iostream>

#ifdef AL_CLIENT
#define AL_SIDE "[CLIENT]: "
#elif AL_SERVER
#define AL_SIDE "[SERVER]: "
#endif

namespace Aluminium {

    void Logger::OutputMsg(const std::string& msg) {

        std::cout << AL_SIDE << msg << "\n";

    }

}
