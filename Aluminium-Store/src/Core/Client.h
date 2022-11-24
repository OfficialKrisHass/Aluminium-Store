#pragma once

#include <string>

#include "asio.h"

namespace Aluminium::Client {

	void Initialize();

	void Write(std::string message);
	std::string Read();

}