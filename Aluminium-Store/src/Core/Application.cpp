#include "Application.h"

#include "Window.h"

namespace Aluminium::MainApplication {

	struct AppData {

		Window window;

	};

	AppData data;

	void Initialize() {

		data.window = Window();
		data.window.Initialize(1280, 720, "Aluminium Store");

	}

	void StartLoop() {

		while (data.window.Open()) {

			data.window.Update();

		}

	}

	void Shutdown() {

		data.window.Shutdown();

	}

}