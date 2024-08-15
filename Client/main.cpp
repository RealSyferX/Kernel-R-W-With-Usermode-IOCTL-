#include "driver.hpp"
#include <TlHelp32.h>

constexpr uintptr_t dwLocalPlayer = 0xD2FB94;
constexpr uintptr_t dwForceJump = 0x51ED760;
constexpr uintptr_t m_fFlags = 0x104;
constexpr uintptr_t FL_ONGROUND = 1 << 0;

auto get_module(const char* module_name, DWORD process_id) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!strcmp(modEntry.szModule, module_name)) {
					CloseHandle(hSnap);
					return modEntry;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
}

int main() {
	auto hwnd = FindWindowA(NULL, "Untitled - Notepad");
	DWORD process_id; GetWindowThreadProcessId(hwnd, &process_id);
	auto client = get_module("notepad.exe", process_id);
	auto client_base = (uintptr_t)client.modBaseAddr;

	auto driver = new driver_manager("\\\\.\\animepatrol", process_id);

	while (true) {
		//auto local_player = driver->RPM<uintptr_t>(client_base + 0x2B370);
		//if (!local_player) continue;4
		//auto flags = driver->RPM<DWORD>(local_player + m_fFlags);

		//if (flags & FL_ONGROUND)
		if (GetAsyncKeyState(VK_F2) & 1)
			driver->WPM<uintptr_t>(client_base + 0x2B370, 22222);
		if (GetAsyncKeyState(VK_F3) & 1)
			driver->WPM<uintptr_t>(client_base + 0x2B370, 33333);
		Sleep(5);
	}
}
