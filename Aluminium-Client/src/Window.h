#pragma once

#include <Core.h>

namespace Aluminium::Window {

    typedef void (*WindowCloseCallback)();

    void Initialize(WindowCloseCallback winCloseCallback);
    void Update();
    void Shutdown();

    bool IsWindowOpen();

    void* GetPointer();

}
