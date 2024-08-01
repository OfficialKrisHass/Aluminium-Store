#include "Input.h"
#include "main.h"

#include <mutex>
#include <queue>
#include <thread>

namespace Aluminium::Input {

    std::mutex mutex;
    std::queue<std::string> queue;
    InputHandler inputHandler;

    void InputThread() {

        while (IsRunning()) {

            char line[512];
            if (!fgets(line, sizeof(line), stdin)) {

                if (!IsRunning()) return;

                LogError("Could not read from stdin, quitting");
                Quit();

                break;

            }

            mutex.lock();
            std::string input = line;
            input.pop_back();
            queue.push(input);
            mutex.unlock();

        }

    }
    void Update() {

        mutex.lock();
        while (!queue.empty()) {

            inputHandler(queue.front());
            queue.pop();

        }
        mutex.unlock();

    }

    void SetInputHandler(InputHandler handler) {

        inputHandler = handler;

    }

}
