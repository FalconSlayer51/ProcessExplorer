#include <iostream>
#include <windows.h>
#include "ThreadPool/ThreadPool.h"

class Server {
public:
	Server();
	~Server();

	void startServer();
private:
	static void ClientHandler(HANDLE pipe);
	std::unique_ptr<ThreadPool> threadPool;
};