#pragma once
#include "IInspectorClient.h"
#include <vector>

class FakeInspectorClient : public IInspectorClient {
public:
	FakeInspectorClient();
	~FakeInspectorClient();
	std::vector<ProcessInfo> getProcessList() override;
private:
	HANDLE pipe;
};