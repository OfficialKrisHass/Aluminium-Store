#include "Application.h"

#include "Window.h"

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>

#include <iostream>

namespace Aluminium::MainApplication {

	struct AppData {

		Window window;

		asio::io_context context;

	};

	AppData data;

	using namespace asio::ip;

	void Initialize() {

		/*data.window = Window();
		data.window.Initialize(1280, 720, "Aluminium Store");*/

		try {

			tcp::resolver resolver(data.context);
			tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "80");
			tcp::socket socket(data.context);

			std::cout << "Trying to connect to the Server\n" << std::endl;
			asio::connect(socket, endpoints);
			std::cout << "Successfully Connected to the Server\n" << std::endl;

			while (true) {

				std::vector<char> buf(128);
				asio::error_code error;

				std::cout << "Reading the Welcome Message" << std::endl;

				size_t len = socket.read_some(asio::buffer(buf), error);

				if (error == asio::error::eof) {

					std::cout << "Clean disconnection\n" << std::endl;

					break;

				} else if(error) {

					std::cout << "An error has occured while Reading the Message" << std::endl;

					throw asio::system_error(error);

				}

				std::cout.write(buf.data(), len);
				std::cout << std::endl;

				std::cout << "Sending a Response" << std::endl;

				std::string response = "Thanks for welcoming me!";
				socket.write_some(asio::buffer(response), error);

			}

		} catch (std::exception e) {

			std::cout << e.what() << std::endl;

		}

		std::cout << "Exiting the Initialization Function" << std::endl;

	}

	void StartLoop() {

		/*while (data.window.Open()) {

			data.window.Update();

		}*/

	}

	void Shutdown() {

		//data.window.Shutdown();

		std::cin.get();

	}

}