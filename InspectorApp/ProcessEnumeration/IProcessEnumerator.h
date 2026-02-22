#pragma once

#include <vector>
#include <windows.h>
#include "../Common/ProcessInfo.h"
#include <Psapi.h>

class IProcessEnumerator {
public:
	virtual BOOL getProcesses(std::vector<ProcessInfo>& processes) = 0;
	virtual BOOL getProcessMemory(DWORD pid, PROCESS_MEMORY_COUNTERS_EX& memInfo) = 0;
};