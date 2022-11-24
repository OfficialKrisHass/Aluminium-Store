#pragma once

#include "asio.h"

namespace Aluminium::MainApplication {

	void Initialize();
	void Shutdown();

	asio::io_context& GetContext();

}