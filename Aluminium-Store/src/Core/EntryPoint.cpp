#include "Core/Application.h"

int main() {

	using namespace Aluminium;

	MainApplication::Initialize();
	MainApplication::StartLoop();
	MainApplication::Shutdown();

	return 0;

}