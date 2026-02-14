#pragma once
#include "../viewmodel/AppState.h"
#include <vector>

class IInspectorClient {
public:
	virtual std::vector<ProcessInfo> getProcessList() = 0;
	virtual ~IInspectorClient() = default;
};