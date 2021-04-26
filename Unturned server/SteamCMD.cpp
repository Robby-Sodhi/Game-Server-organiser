#include "SteamCMD.hpp"
#include <string>
#include <fstream>
#include <exception>
#include "fileSystemUtils.hpp"

std::string ERRORS[] = { "FAILED", "not found", "ERROR!", "ERROR" };

void SteamCMD::checkOutputForErrors(wchar_t logFilePath[])
{
	std::ifstream logFile{ logFilePath };
	if (logFile.fail() || logFile.peek() == std::ifstream::traits_type::eof()) { //if we fail to open the file or the file is empty
		throw std::runtime_error("error opening log (likely steamcmd.exe didn't run)");
	}

	if (logFile.is_open())
	{
		while (!logFile.eof())
		{
			std::string data;
			std::getline(logFile, data);
			for (auto const &error : ERRORS) {
				if (data.find(error) != std::string::npos) {
					throw std::runtime_error("steamcmd.exe likely failed");
				}
			}
		}
	}


}




