#pragma once

#include "alpch.h"

#include "Core/asio.h"

namespace Aluminium {

	class Client {

	public:
		Client(int32_t id);

		void Invalidate();

		void Write(std::string message);
		std::string Read();

		bool HasRequested();

		inline tcp::socket& GetSocket() { return socket; }

		bool welcomed;

	public:
		int32_t id;
		tcp::socket socket = tcp::socket(asio::io_context());

		asio::error_code error;

		bool isWriting;
		bool isReading;

	};

}