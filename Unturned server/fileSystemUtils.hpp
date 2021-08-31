#ifndef FILESYSTEMUTILSHPP
#define FILESYSTEMUTILSHPP
#include <vector>
#include <string>
#include <filesystem>
#include <windows.h>

std::vector<std::string> getServerList(std::string path);
DWORD runCMD(wchar_t cmd[], wchar_t logFilePath[], bool createConsole = false);
void writeToFile(std::wstring path, std::string data);
int ascii_to_virtual_key(char ascii);

#endif // !FILESYSTEMUTILSHPP
