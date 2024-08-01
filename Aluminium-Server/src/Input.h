#pragma once

#include <Core.h>

namespace Aluminium::Input {

    void InputThread();
    void Update();

    typedef void (*InputHandler)(const std::string& input);
    void SetInputHandler(InputHandler handler);

}
