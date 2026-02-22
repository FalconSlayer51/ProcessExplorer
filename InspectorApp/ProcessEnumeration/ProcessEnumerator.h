#pragma once

#include <iostream>
#include <windows.h>
#include <Psapi.h>
#include "IProcessEnumerator.h"
#include "../Common/ProcessInfo.h"

class ProcessEnumerator: public IProcessEnumerator {
public:
	virtual BOOL getProcesses(std::vector<ProcessInfo>& processes) override;
	virtual BOOL getProcessMemory(DWORD pid, PROCESS_MEMORY_COUNTERS_EX& memInfo) override;
};