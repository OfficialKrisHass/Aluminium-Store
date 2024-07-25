#pragma once

#include <Core.h>

namespace Aluminium::Window {

    void Initialize();
    void Update();
    void Shutdown();

    bool IsWindowOpen();

    void* GetPointer();

}
