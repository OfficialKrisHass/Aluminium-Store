#include "Application.h"
#include "asio.h"

#include "Window.h"
#include "Client.h"

#include "../../Aluminium-Server/src/Core/Constants.h"

#include <iostream>
#include <thread>

namespace Aluminium::MainApplication {

	using namespace asio::ip;

	struct AppData {

		Window window;
		bool running = true;

		asio::io_context context;

	};

	AppData data;

	void StartLoop();

	void HandleNetworking();

	void Initialize() {

		data.window = Window();
		data.window.Initialize(1280, 720, "Aluminium Store");

		std::thread networking(HandleNetworking);

		StartLoop();
		networking.join();

	}

	void StartLoop() {

		while (data.running) {

			data.window.Update();

			Sleep(MiliSecondsPerTick);

		}

	}

	void Shutdown() {

		data.running = false;
		data.window.Shutdown();

		try {

			Client::Write("Disconnect");

		} catch(std::exception e) {

			std::cout << e.what() << std::endl;

		}

	}

	void HandleNetworking() {

		Client::Initialize();
		std::cout << Client::Read() << std::endl;

		Client::Write("Login");

		while (data.running) {

			std::string msg;

			try {

				msg = Client::Read();

			} catch (std::exception e) {

				Shutdown();

			}

			std::cout << msg << std::endl;

			Sleep(MiliSecondsPerTick);

		}

	}

	asio::io_context& GetContext() { return data.context; }

}