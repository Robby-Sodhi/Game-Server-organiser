#ifndef UNTURNEDSERVERHPP
#define UNTURNEDSERVERHPP
#include <vector>
#include <map>
#include <windows.h>
#include "commands.hpp"

class UnturnedServer {
private:
	std::string M_steamCMDPath;
	std::string M_currentServerPath;
	std::string M_unturnedPath;
	std::string M_unturnedServersPath;
	std::wstring M_commandsDatPath;
	CommandsClass M_commands;
	std::string M_serverName;
	DWORD M_processId = NULL;
	void createRequiredServerFiles();
public:
	void editProperties(std::map<std::string, std::string> props);
	UnturnedServer(std::string serverPath, std::string unturnedServersPath, std::string serverName, std::string steamCMDPath);
	void startUnturnedServer();
	int stopUnturnedServer();
	void update_app();
	void workshopDownload(std::string workshopLink);

};


#endif // !UNTURNEDSERVERHPP
