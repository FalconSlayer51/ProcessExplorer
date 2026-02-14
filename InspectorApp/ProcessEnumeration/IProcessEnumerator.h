#pragma once

#include <vector>
#include <windows.h>
#include "../Common/ProcessInfo.h"

class IProcessEnumerator {
public:
	virtual BOOL getProcesses(std::vector<ProcessInfo>& processes) = 0;
};