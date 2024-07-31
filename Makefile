OS = linux
CONFIGURATION = Release
BUILD_DIR = $(OS)-x86_64-$(CONFIGURATION)

CLIENT_LOGGING = OFF

.PHONY: all
.PHONY: clean
.PHONY: build
.PHONY: cmake

all: cmake build

cmake: CMakeLists.txt Aluminium-Client/CMakeLists.txt Aluminium-Server/CMakeLists.txt
	@cmake . -B cmake/$(CONFIGURATION) -DCMAKE_BUILD_TYPE=$(CONFIGURATION) -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCLIENT_LOGGING=$(CLIENT_LOGGING)
	@rm -f compile_commands.json
	@ln -s cmake/$(CONFIGURATION)/compile_commands.json compile_commands.json

build:
	@${MAKE} --no-print-directory -C cmake/$(CONFIGURATION)/Aluminium-Client -f Makefile
	@${MAKE} --no-print-directory -C cmake/$(CONFIGURATION)/Aluminium-Server -f Makefile

launch:
	@./build/$(BUILD_DIR)/Aluminium-Client/Aluminium-Client

start-server:
	@./build/$(BUILD_DIR)/Aluminium-Server/Aluminium-Server $(PASSWORD)

database:
	@mariadb -u dev -p aluminium_store

clean:
	@rm -rf build/
	@rm -rf cmake/
	@rm -f compile_commands.json
