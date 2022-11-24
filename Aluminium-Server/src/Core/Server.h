#pragma once

#include "Core/asio.h"

namespace Aluminium::Server {

	asio::io_context& GetContext();
	tcp::acceptor& GetAcceptor();

}