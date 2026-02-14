#pragma once
#include "../viewmodel/AppState.h"
#include "IInspectorClient.h"
#include <vector>

class InspectorIPCClient : public IInspectorClient {
public:
	InspectorIPCClient();
	~InspectorIPCClient();
	std::vector<ProcessInfo> getProcessList() override;
	std::vector<ProcessInfo> DeserializeProcessList(const std::vector<uint8_t>& buffer);
private:
	HANDLE pipe;
};