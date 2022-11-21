#pragma once

#include <stdint.h>
#include <string>

namespace Aluminium {

	class Window {

	public:
		void Initialize(uint32_t width, uint32_t height, std::string title);
		void Update();
		void Shutdown();

		bool Open();

	private:
		void* wPointer;

	};

}