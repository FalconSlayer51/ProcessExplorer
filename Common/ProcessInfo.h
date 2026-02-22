#pragma once
#include <string>
#include <cstdint>
#include <windows.h>
#include <Psapi.h>

struct ProcessInfo {
	uint32_t pid;
	uint32_t parentId;
	std::string name;
	PROCESS_MEMORY_COUNTERS_EX memInfo;
};
