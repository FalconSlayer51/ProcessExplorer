#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <optional>
#include <mutex>
#include "../Common/ProcessInfo.h"

struct AppState {
	std::vector<ProcessInfo> processes;
	std::optional<DWORD> selectedProcess;
	std::mutex mutex;
};