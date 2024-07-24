#include "Network.h"

#include <iostream>

#include <thread>

namespace Aluminium {

    bool running = true;

    void ConnectionStatusChangedCallback(Network::ConnectionState state);

    int StartServer() {

        std::cout << "Starting Aluminium Server\n";

        Network::Initialize();
        Network::SetConnectionStatucChangedCallback(ConnectionStatusChangedCallback);

        int i = 0;
        while (running) {

            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (i == 10)
                running = false;

            i++;

        }

        std::cout << "Shutting down Aluminium Server\n";

        Network::Shutdown();

        std::cout << "Aluminium Server successfully shut down\n";

        return 0;

    }

    void ConnectionStatusChangedCallback(Network::ConnectionState state) {

        using namespace Network;

    }

}

int main() {

    std::cout << "Hello, World, but from the server!\n";
    return Aluminium::StartServer();

}
