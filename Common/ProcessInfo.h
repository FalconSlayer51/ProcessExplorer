#pragma once
#include <string>
#include <cstdint>

struct ProcessInfo {
	uint32_t pid;
	uint32_t parentId;
	std::string name;
};
