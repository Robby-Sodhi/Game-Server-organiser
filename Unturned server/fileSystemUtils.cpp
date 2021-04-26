#include "fileSystemUtils.hpp"
#include <experimental/filesystem>
#include <windows.h>
#include <fstream>


std::vector<std::string> getServerList(std::string path)
{
	std::vector<std::string> servers;

	for (const auto &fileInPath : std::experimental::filesystem::directory_iterator(path)) {
		servers.push_back(fileInPath.path().string());
	}


	return servers;

}


DWORD runCMD(wchar_t cmd[], wchar_t logFilePath[], bool createConsole) {


	SECURITY_ATTRIBUTES securityA;
	securityA.nLength = sizeof(securityA);
	securityA.lpSecurityDescriptor = NULL;
	securityA.bInheritHandle = TRUE;

	HANDLE LogFileHandle = CreateFile(logFilePath,
		GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&securityA,
		CREATE_ALWAYS, //overwrites the file if it exists and is accessible/has the permissions to fuck it
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (LogFileHandle == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("couldn't create CMDLOG");
	}
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startInfo;


	ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	startInfo.cb = sizeof(STARTUPINFO);
	if (!createConsole) {
		startInfo.dwFlags |= STARTF_USESTDHANDLES;
	}
	startInfo.hStdInput = NULL;
	startInfo.hStdError = LogFileHandle;
	startInfo.hStdOutput = LogFileHandle;


	BOOL bPResult = FALSE;
	if (createConsole) {
		bPResult = CreateProcessW(NULL, cmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &startInfo, &processInfo);
	}
	else {
		bPResult = CreateProcessW(NULL, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &startInfo, &processInfo);
	}

	if (!bPResult)
	{
		CloseHandle(LogFileHandle);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
		throw std::runtime_error("create process failed");

	}

	bool bisProcessClosed = false;
	if (!createConsole) {
		while (!bisProcessClosed)
		{
			bisProcessClosed = WaitForSingleObject(processInfo.hProcess, 50) == WAIT_OBJECT_0;
		}
	}
	DWORD processID = processInfo.dwProcessId;
	CloseHandle(LogFileHandle);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	return processID;

}

void writeToFile(std::wstring path, std::string data) {

	std::fstream file{ path };
	if (file.is_open()) {
		file << data;
		return;
	}
	throw std::runtime_error("error opening file");

}