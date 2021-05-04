#include <iostream>
#include "SteamCMD.hpp"
#include <thread>
#include <future>
#include <map>
#include "unturnedServer.hpp"
#include "fileSystemUtils.hpp"
#include <string>



using namespace std;


//std::string unturnedServersPath = "C:\\Users\\robby\\Desktop\\steam cmd aka unturned server test\\steamapps\\common\\U3DS\\Servers";

int main() {

	/*	std::future<void> update_result;
		string input{};
		cout << "server_update, edit_server: ";
		cin >> input;
		if (input == "server_update") {
			cout << "Downloading...... Please Wait" << endl;
			update_result = std::async(std::launch::async, [&] { SteamCMD::update_app(); });
			try {
				update_result.get();
			}
			catch (const std::exception& e) {
				cout << e.what() << endl;
				return 1;
			}
			cout << "Succesfull" << endl;

		}
		else if (input == "edit_server") {
			std::string serverPath = getServerList(unturnedServersPath)[0]; //asume path 0 (eg folder 1 a-z)
			UnturnedServer unturnedServer = UnturnedServer(serverPath, unturnedServersPath);
			cout << serverPath << endl;
			std::map<string, string> propertyMap;
			propertyMap["networkType"] = "InternetServer";
			propertyMap["serverName"] = "My Test Server";
			unturnedServer.editProperties(propertyMap);
		}*/





	bool threadFinished{ false };
	bool threadRunning{ false };
	std::thread update_app;
	std::vector<UnturnedServer> servers;

	while (true) {

		if (threadFinished) {
			std::cout << "Thread finished" << std::endl;
			update_app.join();
			threadRunning = false;
			threadFinished = false;
		}

		std::string input;
		cout << "serverUpdate or editServer or exit or addServer or startServer or stopServer or workshopDownload: ";
		cin >> input;

		if (input == "exit") {
			if (update_app.joinable()) {
				update_app.join();
			}
			break;
		}
		if (input == "serverUpdate" && !threadRunning) {
			threadRunning = true;
			cout << "spinning thread" << endl;
			update_app = std::thread{ [&] {servers[0].update_app(); threadFinished = true; } };
		}

		else if (input == "editServer") {
			servers[0].editProperties(std::map<std::string, std::string>{std::pair<std::string, std::string>{"map", "russia"}, std::pair<std::string, std::string>{"gold", "place"}});
		}
		else if (input == "addServer") {
			std::string serverName{};
			std::string unturnedPath{};
			std::string steamCMDPath{};

			cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			cout << "unturnedPath: ";
			std::getline(std::cin, unturnedPath);

			cout << "unturnedServerName: ";
			std::getline(std::cin, serverName);

			cout << "steamCMDPath: ";
			std::getline(std::cin, steamCMDPath);

			std::string serverPath = unturnedPath + "\\Servers\\" + serverName;


			UnturnedServer server = UnturnedServer(serverPath, unturnedPath, serverName, steamCMDPath);
			servers.push_back(server);
		}
		else if (input == "startServer") {
			servers[0].startUnturnedServer();

		}
		else if (input == "stopServer") {
			int result = servers[0].stopUnturnedServer();
			if (result == 0)
				cout << "failed" << endl;
			else if (result == 1)
				cout << "succesfull" << endl;
			else if (result == 2)
				cout << "fatal error can't recover" << endl;


		}
		else if (input == "workshopDownload")
		{
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			std::cout << "workshop ID: ";
			std::string workshopLink;
			std::cin >> workshopLink;

			cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			servers[0].workshopDownload(workshopLink);
		}
	}

	return 0;
}