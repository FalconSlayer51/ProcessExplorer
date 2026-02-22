#include "../Common/ProcessInfo.h"
#include "../Common/Protocol.h"
#include "../Common/Serializer.h"
#include <vector>
#include <cstring>
#include <string>

std::vector<uint8_t> serializeProcessList(const std::vector<ProcessInfo>& list) {
	size_t payloadSize = sizeof(uint32_t);

	for (const auto& p : list) {
		payloadSize += sizeof(ProcessInfoWire);
		payloadSize += p.name.size();
	}

	std::vector<uint8_t> buffer(payloadSize);
	uint8_t* ptr = buffer.data();
    uint32_t count = (uint32_t)list.size();
    memcpy(ptr, &count, sizeof(count));
    ptr += sizeof(count);

    for (const auto& p : list) {
        ProcessInfoWire wire;
        wire.pid = p.pid;
        wire.parentId = p.parentId;
        wire.nameLength = (uint32_t)p.name.size();
        wire.memInfo = p.memInfo;

        memcpy(ptr, &wire, sizeof(wire));
        ptr += sizeof(wire);

        memcpy(ptr, p.name.data(), p.name.size());
        ptr += p.name.size();
    }

    return buffer;
}