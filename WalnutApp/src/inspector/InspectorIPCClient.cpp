#include <windows.h>
#include <vector>
#include <iostream>
#include "InspectorIPCClient.h"
#include "../Common/ProcessInfo.h"
#include "../Common/Protocol.h"
#include "../Common/Serializer.h"

#define PIPE_NAME L"\\\\.\\pipe\\ProcessExplorerPipe"

InspectorIPCClient::InspectorIPCClient() {
	pipe = CreateFileW(
		PIPE_NAME,
		GENERIC_READ | GENERIC_WRITE,
		0,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);

	if (pipe == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to connect to inspector\n";
		DWORD error = GetLastError();
		std::cout << "Failed to connect to inspector. Error: "
			<< error << std::endl;
	}
}

InspectorIPCClient::~InspectorIPCClient() {
	CloseHandle(pipe);
}

std::vector<ProcessInfo> InspectorIPCClient::DeserializeProcessList(const std::vector<uint8_t>& buffer) {
	std::vector<ProcessInfo> result;

	const uint8_t* ptr = buffer.data();

	uint32_t count;
	memcpy(&count, ptr, sizeof(count));
	ptr += sizeof(count);

	for (uint32_t i = 0; i < count; i++) {
		ProcessInfoWire wire;
		memcpy(&wire, ptr, sizeof(wire));
		ptr += sizeof(wire);

		std::string name(
			(const char*)ptr,
			wire.nameLength
		);
		ptr += wire.nameLength;

		result.push_back({ wire.pid,wire.parentId,name });
	}

	return result;
}

std::vector<ProcessInfo> InspectorIPCClient::getProcessList() {
	MessageHeader request;
	request.size = 0;
	request.type = (uint32_t)MessageType::GetProcessList;

	DWORD written;
	if (!WriteFile(pipe, &request, sizeof(request), &written, nullptr)) {
		std::cerr << "Failed to send request. Error: " << GetLastError() << std::endl;
		return {};
	}

	MessageHeader response;
	DWORD read;
	response.type = (uint32_t)MessageType::ProcessListResponse;
	if (!ReadFile(pipe, &response, sizeof(response), &read, nullptr)) {
		std::cerr << "Failed to get response. Error: " << GetLastError() << std::endl;
		return {};
	}

	std::vector<uint8_t> payload(response.size);
	BOOL result = ReadFile(pipe, payload.data(), response.size, &read, nullptr);

	if (!result) {
		std::cerr << "Failed to read payload. Error: " << GetLastError() << std::endl;
		return {};
	}

	// 4. Deserialize
	return DeserializeProcessList(payload);
}