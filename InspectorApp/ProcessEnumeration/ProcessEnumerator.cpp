#include <iostream>
#include <windows.h>
#include <vector>
#include <tlhelp32.h>
#include "../Common/ProcessInfo.h"
#include "ProcessEnumerator.h"

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

		pInfo.name = name;
		pInfo.pid = pe32.th32ProcessID;
		pInfo.parentId = pe32.th32ParentProcessID;

		processes.push_back(pInfo);
	} while (Process32Next(hSnapshot, &pe32));
	return TRUE;
}