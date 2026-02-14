#include <iostream>
#include <vector>
#include <windows.h>
#include <memory.h>
#include "Server.h"
#include "../Common/ProcessInfo.h"
#include "../Common/Protocol.h"
#include "../Common/Serializer.h"

int main() {
	std::unique_ptr<Server> server = std::make_unique<Server>();
	server->startServer();
	return 0;
}