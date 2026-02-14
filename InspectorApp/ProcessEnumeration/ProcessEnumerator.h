#pragma once

#include <iostream>
#include <windows.h>
#include "IProcessEnumerator.h"
#include "../Common/ProcessInfo.h"

class ProcessEnumerator: public IProcessEnumerator {
public:
	virtual BOOL getProcesses(std::vector<ProcessInfo>& processes) override;
};