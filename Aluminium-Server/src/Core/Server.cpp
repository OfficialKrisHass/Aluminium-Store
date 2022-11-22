#include "Server.h"

#include <iostream>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>

namespace Aluminium::Server {

	struct ServerData {

		asio::io_context context;

	};

	ServerData data;

	using namespace asio::ip;

	void Initialize() {

		try {

			asio::error_code error;
			tcp::acceptor acceptor(data.context, tcp::endpoint(asio::ip::make_address("127.0.0.1"), 80));

			while (true) {

				std::cout << "Waiting for Connection on port 80" << std::endl;

				std::string welcome = "Welcome to the Server!\n";
				tcp::socket socket(data.context);
				acceptor.accept(socket);

				std::cout << "Client Connected!\n" << std::endl;

				asio::write(socket, asio::buffer(welcome), error);

				std::cout << "Listening for a Client Response" << std::endl;

				std::vector<char> buf(128);
				size_t len = socket.read_some(asio::buffer(buf), error);

				if (error == asio::error::eof) {

					std::cout << "Clean disconnection\n" << std::endl;

					break;

				} else if (error) {

					std::cout << "An error has occured while Reading the Message" << std::endl;

					throw asio::system_error(error);

				}

				std::cout.write(buf.data(), len);
				std::cout << "\n" << std::endl;

			}

		} catch (std::exception e) {

			std::cout << e.what() << std::endl;

		}

	}

	void Start() {

		//

	}

	void Shutdown() {

		//

	}

}