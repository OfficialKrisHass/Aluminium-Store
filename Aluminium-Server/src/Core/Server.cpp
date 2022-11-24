#include "alpch.h"
#include "Server.h"

#include "Core/Constants.h"
#include "Core/Client.h"

#include <thread>
#include <chrono>
#include <ctime>  

namespace Aluminium::Server {

	struct ServerData {

		bool running = true;

		asio::io_context context;
		tcp::acceptor acceptor = tcp::acceptor(context);

		std::vector<Client> clients;
		std::vector<int32_t> gaps;

		ServerData() = default;

	};

	void Start();
	void BeginLoop();
	void Shutdown();

	void ListenForConnections();

	ServerData data;

	void Start() {

		std::thread connectionListener(ListenForConnections);

		BeginLoop();

	}
	void BeginLoop() {

		std::chrono::system_clock::time_point previousLoop = std::chrono::system_clock::now();
		std::chrono::duration<double> timeSinceLastLoop;

		bool pingUsers = false;

		while (data.running) {

			timeSinceLastLoop = std::chrono::system_clock::now() - previousLoop;

			if (timeSinceLastLoop.count() >= 60) {

				previousLoop = std::chrono::system_clock::now();
				pingUsers = true;

			}

			//For some reason we have to loop through the clients vector
			//Like this, if we do it the more modern way it gives
			//you an empty nullptr Client even though the client exists
			//in the vector
			for (int i = 0; i < data.clients.size(); i++) {

				Client& client = data.clients[i];

				if (client.id == -1) continue;
				if (!client.welcomed) continue;

				if (pingUsers) {

					try {

						client.Write("Ping");

					} catch (std::exception e) {

						std::cout << "Client " << client.id << " Has Disconnected" << std::endl;

						data.gaps.push_back(client.id);
						client.Invalidate();

					}

				}
				if (!client.HasRequested()) continue;

				std::string request = client.Read();

				if (request == "Login") {

					std::cout << "Client " << client.id << " Has tried to Log in" << std::endl;

				}

				if (request == "Disconnect") {

					std::cout << "Client " << client.id << " Has Disconnected" << std::endl;

					data.gaps.push_back(client.id);
					client.Invalidate();

				}

			}

			if (pingUsers) pingUsers = false;

			Sleep(MiliSecondsPerTick);

		}

		Shutdown();

	}
	void Shutdown() {

		//

	}

	void ListenForConnections() {

		asio::error_code error;
		data.acceptor = tcp::acceptor(data.context, tcp::endpoint(asio::ip::make_address("127.0.0.1"), 80));

		while (data.running) {

			int32_t clientID;

			//This is a bit bugged, Because we create the Client first and then block the thread
			//until a Client connects, this means that when a client disconnects, at that point
			//the client was already created and only waiting for a connection, so it puts the
			//Client at the back and the next client after that is the one that will be put in the
			//last gap, it works fine it doesn't break anything, it just might be annoying
			if (!data.gaps.empty()) {

				clientID = data.gaps.back();
				data.gaps.pop_back();

				data.clients[clientID] = Client(clientID);

			} else {

				clientID = (int32_t) data.clients.size();
				data.clients.push_back(Client(clientID));

			}

			Client& client = data.clients[clientID];
			std::cout << "Client " << client.id << " Connected" << std::endl;

			std::string welcome = "Welcome Client " + std::to_string(client.id);
			client.Write(welcome);
			client.welcomed = true;

			Sleep(MiliSecondsPerTick);

		}

	}

	asio::io_context& GetContext() { return data.context; }
	tcp::acceptor& GetAcceptor() { return data.acceptor; }

}

int main() {

	Aluminium::Server::Start();

	return 0;

}