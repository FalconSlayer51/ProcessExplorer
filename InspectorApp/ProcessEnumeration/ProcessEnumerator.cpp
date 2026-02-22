#include <iostream>
#include <windows.h>
#include <vector>
#include <tlhelp32.h>
#include <psapi.h>
#include "../Common/ProcessInfo.h"
#include "ProcessEnumerator.h"

BOOL ProcessEnumerator::getProcessMemory(DWORD pid, PROCESS_MEMORY_COUNTERS_EX& memInfo) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (hProcess == INVALID_HANDLE_VALUE || hProcess == nullptr) {
		std::cout << "Unable to open process with pid: " << pid << "error: " << GetLastError();
		return FALSE;
	}

	if (!GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&memInfo, sizeof(memInfo))) {
		//std::cout << "Unable to get process memory with pid: " << pid << "error: " << GetLastError();
		CloseHandle(hProcess);
		return FALSE;
	}

	CloseHandle(hProcess);
	return TRUE;
}

BOOL ProcessEnumerator::getProcesses(std::vector<ProcessInfo>& processes) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS,
		0
	);

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		std::cout << "Invalid handle value: " << GetLastError() << std::endl;
		return FALSE;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnapshot, &pe32)) {
		std::cout << "Unable to open snapshot: " << GetLastError() << std::endl;
		CloseHandle(hSnapshot);
		return FALSE;
	}

	do {
		ProcessInfo pInfo;

		// WCHAR to std::string conversion
		std::wstring wname(pe32.szExeFile);
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string name(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, &name[0], size_needed, nullptr, nullptr);
		if (!name.empty() && name.back() == '\0') name.pop_back();

		// find memInfo
		PROCESS_MEMORY_COUNTERS_EX memInfo;
		ZeroMemory(&memInfo, sizeof(PROCESS_MEMORY_COUNTERS_EX));
		BOOL res = getProcessMemory(pe32.th32ProcessID, memInfo);
		if (res) {
			pInfo.name = name;
			pInfo.pid = pe32.th32ProcessID;
			pInfo.parentId = pe32.th32ParentProcessID;
			pInfo.memInfo = memInfo;
			std::cout << "Memory" << memInfo.PrivateUsage << std::endl;
			processes.push_back(pInfo);
		}
		
	} while (Process32Next(hSnapshot, &pe32));

	CloseHandle(hSnapshot);
	return TRUE;
}