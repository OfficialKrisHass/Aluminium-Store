#include "Core/Server.h"

#include <iostream>

int main() {

	using namespace Aluminium;

	Server::Initialize();
	Server::Start();
	Server::Shutdown();

	std::cin.get();

	return 0;

}