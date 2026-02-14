#include "IInspectorClient.h"
#include "FakeInspector.h"
#include <vector>
#include <iostream>
#include "../Common/ProcessInfo.h"
#include "../Common/Protocol.h"
#include "../Common/Serializer.h"

#define PIPE_NAME L"\\\\.\\pipe\\ProcessExplorerPipe"

FakeInspectorClient::FakeInspectorClient() {
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

FakeInspectorClient::~FakeInspectorClient() {
    CloseHandle(pipe);
}

std::vector<ProcessInfo>
DeserializeProcessList(const std::vector<uint8_t>& buffer)
{
    std::vector<ProcessInfo> result;

    const uint8_t* ptr = buffer.data();

    uint32_t count;
    memcpy(&count, ptr, sizeof(count));
    ptr += sizeof(count);

    for (uint32_t i = 0; i < count; i++)
    {
        ProcessInfoWire wire;
        memcpy(&wire, ptr, sizeof(wire));
        ptr += sizeof(wire);

        std::string name(
            (const char*)ptr,
            wire.nameLength
        );
        ptr += wire.nameLength;

        result.push_back({
            wire.pid,
            wire.parentId,
            name
            });
    }

    return result;
}


std::vector<ProcessInfo> FakeInspectorClient::getProcessList() {
    MessageHeader request;
    request.type = (uint32_t)MessageType::GetProcessList;
    request.size = 0;

    DWORD written;
    // 1. Send Request
    if (!WriteFile(pipe, &request, sizeof(request), &written, nullptr)) {
        std::cerr << "Failed to send request. Error: " << GetLastError() << std::endl;
        return {};
    }

    MessageHeader response;
    DWORD read;

    // 2. Read Header
    BOOL result = ReadFile(pipe, &response, sizeof(response), &read, nullptr);

    // If ReadFile failed, check if it was just because "More Data" exists
    if (!result && GetLastError() != ERROR_MORE_DATA) {
        std::cerr << "Failed to read header. Error: " << GetLastError() << std::endl;
        return {};
    }

    // 3. Read Payload
    std::vector<uint8_t> payload(response.size);
    result = ReadFile(pipe, payload.data(), response.size, &read, nullptr);

    if (!result) {
        std::cerr << "Failed to read payload. Error: " << GetLastError() << std::endl;
        return {};
    }

    // 4. Deserialize
    return DeserializeProcessList(payload);
}