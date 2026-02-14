#include "Protocol.h"
#include <vector>
#include <cstring>
#include "ProcessInfo.h"

std::vector<uint8_t> serializeProcessList(const std::vector<ProcessInfo>& list);