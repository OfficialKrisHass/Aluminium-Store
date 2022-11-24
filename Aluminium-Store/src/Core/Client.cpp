#include "Client.h"
#include "asio.h"

#include "Core/Application.h"

#include <iostream>

namespace Aluminium::Client {
	
	struct ClientData {

		tcp::socket& socket = tcp::socket(MainApplication::GetContext());

	};

	ClientData data;

	void Initialize() {
		
		tcp::resolver resolver(MainApplication::GetContext());
		tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "80");
		asio::connect(data.socket, endpoints);

	}

	void Write(std::string message) {

		asio::write(data.socket, asio::buffer(message));

	}

	std::string Read() {

		std::vector<char> buf(128);
		data.socket.read_some(asio::buffer(buf));

		return buf.data();

	}

}