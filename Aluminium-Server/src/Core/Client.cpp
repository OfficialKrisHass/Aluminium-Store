#include "Client.h"
#include "alpch.h"
#include "Client.h"

#include "Core/Server.h"

namespace Aluminium {

	Client::Client(int32_t id) : id(id) {

		socket = tcp::socket(Server::GetContext());
		Server::GetAcceptor().accept(socket);

	}

	void Client::Invalidate() {

		id = -1;
		welcomed = false;

	}

	void Client::Write(std::string message) {

		isWriting = true;
		asio::write(socket, asio::buffer(message));
		isWriting = false;

	}

	std::string Client::Read() {

		isReading = true;
		std::vector<char> buf(128);
		socket.read_some(asio::buffer(buf), error);
		isReading = false;

		return buf.data();

	}

	bool Client::HasRequested() {

		if (isWriting) return false;

		uint32_t bytes = (uint32_t) socket.available();
		return bytes > 0 ? true : false;

	}

}