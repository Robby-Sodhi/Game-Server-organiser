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
	

//write a function to convert letters into their virtual key counterparts using some sort of map/hash-table

#define VK_KEY_S 0x53
#define VK_KEY_H 0x48
#define VK_KEY_U 0x55
#define VK_KEY_T 0x54
#define VK_KEY_D 0x44
#define VK_KEY_O 0x4F
#define VK_KEY_W 0x57
#define VK_KEY_N 0x4E
#define VK_SPACE 0x20 
#define VK_KEY_0 0x30
#define VK_RETURN 0x0D


	PostMessage(g_hwnd, WM_KEYDOWN, VK_KEY_S, 0);
	PostMessage(g_hwnd, WM_KEYDOWN, VK_KEY_H, 0);
	PostMessage(g_hwnd, WM_KEYDOWN, VK_KEY_U, 0);
	PostMessage(g_hwnd, WM_KEYDOWN, VK_KEY_T, 0);
	PostMessage(g_hwnd, WM_KEYDOWN, VK_KEY_D, 0);
	PostMessage(g_hwnd, WM_KEYDOWN, VK_KEY_O, 0);
	PostMessage(g_hwnd, WM_KEYDOWN, VK_KEY_W, 0);
	PostMessage(g_hwnd, WM_KEYDOWN, VK_KEY_N, 0);
	PostMessage(g_hwnd, WM_KEYDOWN, VK_SPACE, 0);
	PostMessage(g_hwnd, WM_KEYDOWN, VK_KEY_0, 0);
	PostMessage(g_hwnd, WM_KEYDOWN, VK_RETURN, 0);


	Sleep(5000);
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
	std::string downloadPath = '\"' + M_currentServerPath + "\\Workshop" + '\"';
	std::string workshopCommandStr = '\"' + M_steamCMDPath + "\\steamcmd.exe\"" + " +login anonymous +force_install_dir " + downloadPath + " +workshop_download_item 304930 " + workshopID + " +quit";
	std::wstring workshopCommandWStr(workshopCommandStr.begin(), workshopCommandStr.end());
	wchar_t logFilePath[] = L"steamCMDLOG.txt";

	DWORD workshopCommand = runCMD(const_cast<wchar_t*>(workshopCommandWStr.c_str()), logFilePath);

	SteamCMD::checkOutputForErrors(logFilePath);
}