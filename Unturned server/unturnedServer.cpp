#include "unturnedServer.hpp"
#include <string>
#include <iostream>
#include "fileSystemUtils.hpp"
#include <windows.h>
#include "Commands.hpp"
#include "SteamCMD.hpp"
using namespace std;


void UnturnedServer::createRequiredServerFiles()
{
	SECURITY_ATTRIBUTES sa;

	ZeroMemory(&sa, sizeof(sa));

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;


	std::string serverFolder = M_currentServerPath + "\\Server";
	std::wstring serverFolderCW(serverFolder.begin(), serverFolder.end());
	CreateDirectory(serverFolderCW.c_str(), &sa);

	std::wstring commandsDatPath = serverFolderCW + L"\\commands.dat";

	HANDLE commandsDatH = CreateFile(commandsDatPath.c_str(),
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	CloseHandle(commandsDatH);
	if (commandsDatH == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("couldn't create commands.dat");
	}

	M_commandsDatPath = commandsDatPath;

	writeToFile(M_commandsDatPath, M_commands.commandString());



}


void UnturnedServer::editProperties(std::map<std::string, std::string> props)
{
	for (auto const &pair : props) {
		if (!M_commands.setCommand(pair.first, pair.second)) {
			throw std::runtime_error("error setting command/invalid");
		}
	}
	writeToFile(M_commandsDatPath, M_commands.commandString());
}

UnturnedServer::UnturnedServer(std::string serverPath, std::string unturnedInstallPath, std::string serverName, std::string steamCMDPath)
{
	M_steamCMDPath = steamCMDPath;

	M_commands = CommandsClass();

	M_serverName = serverName;


	M_unturnedPath = unturnedInstallPath;
	M_unturnedServersPath = unturnedInstallPath += "\\servers";
	for (const auto &server : getServerList(M_unturnedServersPath)) {

		if (server.find(serverPath) != std::string::npos) {
			M_currentServerPath = serverPath;
			createRequiredServerFiles();
			return;
		}
	}
	M_currentServerPath = serverPath;
	//make it create the folder if it doesn't exist
	SECURITY_ATTRIBUTES sa;

	ZeroMemory(&sa, sizeof(sa));

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;

	std::wstring serverPathCW(serverPath.begin(), serverPath.end());

	CreateDirectory(serverPathCW.c_str(), &sa);

	createRequiredServerFiles();


}

void UnturnedServer::startUnturnedServer()
{

	std::string unturnedExe = "\"" + M_unturnedPath + "\\unturned.exe\" +LanServer/" + M_serverName;
	std::string unturnedLog = M_unturnedPath + "\\serverLog.txt";

	std::wstring unturnedExeCW(unturnedExe.begin(), unturnedExe.end());
	std::wstring unturnedLogCW(unturnedLog.begin(), unturnedLog.end());

	DWORD processID = runCMD(const_cast<wchar_t*>(unturnedExeCW.c_str()), const_cast<wchar_t*>(unturnedLogCW.c_str()), true);
	M_processId = processID;

}
HWND g_hwnd = NULL;
static BOOL CALLBACK enumWindowFunction(HWND hWnd, LPARAM lparam) {
	DWORD lpdwProcessId;

	GetWindowThreadProcessId(hWnd, &lpdwProcessId);

	if (lpdwProcessId == lparam) {
		g_hwnd = hWnd;
		return false;
	}
	return true;
}
int UnturnedServer::stopUnturnedServer()
{
	
	if (M_processId == NULL) {
		return 2;
	}
	

	HANDLE unturnedExe = OpenProcess(PROCESS_ALL_ACCESS, TRUE, M_processId);
	
	if (unturnedExe == NULL) {
		return 2;
	}

	EnumWindows(enumWindowFunction, M_processId);

	std::string shutdown = "shutdown 0";
	for (char i : shutdown) {
		PostMessage(g_hwnd, WM_KEYDOWN, ascii_to_virtual_key(i), 0);
	}

	PostMessage(g_hwnd, WM_KEYDOWN, VK_RETURN, 0);

	Sleep(1000);
	DWORD exitcode;
	GetExitCodeProcess(unturnedExe, &exitcode);
	if (exitcode == STILL_ACTIVE) {
		return 0;
	}
//if server shutsdown clear out of date variables
	M_processId = NULL;
	g_hwnd = NULL;

	return 1;
	
	
}

void UnturnedServer::update_app()
{

	std::string steamCMDEXE = '\"' + M_steamCMDPath + "\\steamcmd.exe\"" + "+login anonymous +app_update 1110390 +quit";
	std::wstring steamCMDEXECW(steamCMDEXE.begin(), steamCMDEXE.end());


	wchar_t logFilePath[] = L"steamCMDLOG.txt";


	runCMD(const_cast<wchar_t*>(steamCMDEXECW.c_str()), logFilePath);

	SteamCMD::checkOutputForErrors(logFilePath);


}

void UnturnedServer::workshopDownload(std::string workshopID)
{
//steamCMD doesn't understand how to install mods for an unturned server so first we download it to a temp folder
	std::string downloadPath = '\"' + M_currentServerPath + "\\tempWorkshop" + '\"';
	std::string workshopCommandStr = '\"' + M_steamCMDPath + "\\steamcmd.exe\"" + " +login anonymous +force_install_dir " + downloadPath + " +workshop_download_item 304930 " + workshopID + " +quit";
	std::wstring workshopCommandWStr(workshopCommandStr.begin(), workshopCommandStr.end());
	wchar_t logFilePath[] = L"steamCMDLOG.txt";

	DWORD workshopCommand = runCMD(const_cast<wchar_t*>(workshopCommandWStr.c_str()), logFilePath);

	SteamCMD::checkOutputForErrors(logFilePath);

//after we download it to the tempFolder we now need to move the appropriate folder into server_name/workshop/content or server_name/workshop/maps
//maps is just for maps content is for everything else
	
//fix repeating folder structure stuff here
//add error checking to make sure we have the permission to move files and make sure that the directories exist before doing anything

 	std::string workshop_folder =  M_currentServerPath + "\\tempworkshop\\steamapps\\workshop\\content\\304930\\" + workshopID;
	std::wstring workshop_folderW(workshop_folder.begin(), workshop_folder.end());

	std::string new_workshop_folder =  M_currentServerPath + "\\Workshop\\Content\\" + workshopID ;
	std::wstring new_workshop_folderW(new_workshop_folder.begin(), new_workshop_folder.end());
	bool moveWorkshopFolder_status = MoveFile(
		const_cast<wchar_t*>(workshop_folderW.c_str()),
		const_cast<wchar_t*>(new_workshop_folderW.c_str())
	);

	if (moveWorkshopFolder_status == false) {
		DWORD error = GetLastError();
		throw std::runtime_error("move folder failed");
	}

}