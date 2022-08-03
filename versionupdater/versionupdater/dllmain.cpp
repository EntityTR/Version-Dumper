#include "Windows.h"
#include <thread>
#include <string>
#include "extra.hpp"
#include <iostream>





void main()
{
	ConsoleBypass("Version Updater - Entity");

	std::string RealVersion = DownloadURL("http://setup.roblox.com/version");
	std::string DllVersion = "version-9045f70ea52248c"; //if updated write new version or DownloadURL("pastebin");
	if (RealVersion == DllVersion)
	{
		std::cout << "Version True well dll working lol!" << std::endl;
	}
	else 
    {
		auto_update();
	    MessageBoxA(NULL, "Roblox Updated. Please wait for Update", "Your Exector Name", NULL);
	    exit(0);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		std::thread(main).detach();

		break;
	}


	return TRUE;
}
