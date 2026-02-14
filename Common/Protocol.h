#pragma once
#include <cstdint>

enum class MessageType : uint32_t {
	GetProcessList = 1,
	ProcessListResponse = 2
};

#pragma pack(push, 1)
struct MessageHeader
{
	uint32_t type;
	uint32_t size;   // payload size in bytes
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ProcessInfoWire
{
	uint32_t pid;
	uint32_t parentId;
	uint32_t nameLength;
};
#pragma pack(pop)
