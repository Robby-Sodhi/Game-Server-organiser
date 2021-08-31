#include "fileSystemUtils.hpp"
#include <experimental/filesystem>
#include <windows.h>
#include <fstream>
#include <map>


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

int ascii_to_virtual_key(char ascii) {
	//https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	std::map<char, int> vKey_lookup;
	vKey_lookup['0'] = 0x30;
	vKey_lookup['1'] = 0x31;
	vKey_lookup['2'] = 0x32;
	vKey_lookup['3'] = 0x33;
	vKey_lookup['4'] = 0x34;
	vKey_lookup['5'] = 0x35;
	vKey_lookup['6'] = 0x36;
	vKey_lookup['7'] = 0x37;
	vKey_lookup['8'] = 0x38;
	vKey_lookup['9'] = 0x39;
	vKey_lookup['a'] = 0x41;
	vKey_lookup['b'] = 0x42;
	vKey_lookup['c'] = 0x43;
	vKey_lookup['d'] = 0x44;
	vKey_lookup['e'] = 0x45;
	vKey_lookup['f'] = 0x46;
	vKey_lookup['g'] = 0x47;
	vKey_lookup['h'] = 0x48;
	vKey_lookup['i'] = 0x49;
	vKey_lookup['j'] = 0x4A;
	vKey_lookup['k'] = 0x4B;
	vKey_lookup['l'] = 0x4C;
	vKey_lookup['m'] = 0x4D;
	vKey_lookup['n'] = 0x4E;
	vKey_lookup['o'] = 0x4F;
	vKey_lookup['p'] = 0x50;
	vKey_lookup['q'] = 0x51;
	vKey_lookup['r'] = 0x52;
	vKey_lookup['s'] = 0x53;
	vKey_lookup['t'] = 0x54;
	vKey_lookup['u'] = 0x55;
	vKey_lookup['v'] = 0x56;
	vKey_lookup['w'] = 0x57;
	vKey_lookup['x'] = 0x58;
	vKey_lookup['y'] = 0x59;
	vKey_lookup['z'] = 0x5A;
	vKey_lookup[' '] = 0x20;

	if (vKey_lookup.find(ascii) == vKey_lookup.end()){
		throw std::runtime_error("vKey lookup failed");
	} 

	return vKey_lookup[ascii];

}