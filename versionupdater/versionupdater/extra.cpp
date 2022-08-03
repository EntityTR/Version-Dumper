#include "framework.h"
#include <thread>
#include <string>
#include "Extra.hpp"
#include <intrin.h>
#include "EyeStep/eyestep_utility.h"
#include <iostream>

#pragma once
#include <Wininet.h>
#pragma comment(lib, "wininet.lib")

using EyeStep::scanner::scan;
using namespace EyeStep::util;

const std::uintptr_t m_odule = reinterpret_cast<std::uintptr_t>(GetModuleHandle(NULL));
#define lol(x)( x - 0x000000 - (DWORD)GetModuleHandle(NULL)) //shh LOL

const uintptr_t r_lua_output = m_odule + 0;//auto updates
uintptr_t r_lua_spawn = m_odule + 0;//auto updates
uintptr_t r_lua_load = m_odule + 0;//auto updates

void auto_update()
{
	EyeStep::open(GetCurrentProcess());
	Sleep(1000);
	auto spaw_offset = EyeStep::util::raslr(EyeStep::util::getPrologue(EyeStep::scanner::scan_xrefs("Spawn function requires 1 argument")[0]));
	r_lua_spawn = m_odule + spaw_offset;
	Sleep(1000);
	std::cout << "Spawn: " << std::hex << std::setw(2) << std::setfill('0') << "0x" << lol(r_lua_spawn) << std::endl;

	auto luau_load = raslr(getPrologue(scan("0F ?? ?? 83 ?? 7F D3 ?? 83")[10]));
	r_lua_load = m_odule + luau_load;

	std::cout << "Deserialize: " << std::hex << std::setw(2) << std::setfill('0') << "0x" << lol(r_lua_load) << std::endl;

}

void ConsoleBypass(const char* Name)
{
	DWORD ree;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &ree);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Name);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}
inline std::string Replace(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string DownloadURL(const std::string& url) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	std::string rtn;

	if (interwebs) {
		HINTERNET url_file = InternetOpenUrlA(interwebs, url.c_str(), NULL, NULL, NULL, NULL);

		if (url_file) {
			char buffer[2000];
			DWORD bytes_read;

			do {
				InternetReadFile(url_file, buffer, 2000, &bytes_read);
				rtn.append(buffer, bytes_read);
				memset(buffer, 0, 2000);
			} while (bytes_read);

			InternetCloseHandle(interwebs);
			InternetCloseHandle(url_file);
			std::string p = Replace(rtn, "|n", "\r\n");
			return p;
		}
	}

	InternetCloseHandle(interwebs);
	std::string p = Replace(rtn, "|n", "\r\n");
	return p;
}