#ifndef STEAMCMD
#include <string>
#include <windows.h>
#include <atlstr.h>
#define STEAMCMD

namespace SteamCMD {
	void checkOutputForErrors(wchar_t logFilePath[]);
}
#endif // STEAMCMD
