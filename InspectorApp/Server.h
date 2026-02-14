#include <iostream>
#include <windows.h>

class Server {
public:
	Server();
	~Server();

	void startServer();
private:
	static void ClientHandler(HANDLE pipe);
};