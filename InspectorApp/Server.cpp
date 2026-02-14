#include "Server.h"
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "../Common/ProcessInfo.h"
#include "../Common/Protocol.h"
#include "../Common/Serializer.h"
#include "ProcessEnumeration/ProcessEnumerator.h"

#define PIPE_NAME L"\\\\.\\pipe\\ProcessExplorerPipe"
#define BUFSIZE 65536

Server::Server() {
    // init
}

Server::~Server() {
    // clean up
}

void Server::ClientHandler(HANDLE pipe) {
    std::cout << "[Thread " << std::this_thread::get_id() << "] Client connected.\n";
    while (true) {
        MessageHeader header;
        DWORD bytesRead;

        BOOL result = ReadFile(pipe, &header, sizeof(header), &bytesRead, nullptr);
        if (!result || bytesRead == 0) {
            DWORD err = GetLastError();
            if (err == ERROR_BROKEN_PIPE) {
                std::cout << "[Thread " << std::this_thread::get_id() << "] Client disconnected normally.\n";
            }
            else {
                std::cout << "[Thread " << std::this_thread::get_id() << "] Read error: " << err << "\n";
            }
            break;
        }

        if (header.type == (uint32_t)MessageType::GetProcessList) {
            std::cout << "[Thread " << std::this_thread::get_id() << "] Processing GetProcessList...\n";

            ProcessEnumerator pe;
            std::vector<ProcessInfo> processes;
            BOOL result = pe.getProcesses(processes);

            if (!result) {
                std::cout << "Failed to get processes" << std::endl;
                break;
            }

            auto payload = serializeProcessList(processes);

            // Prepare Response
            MessageHeader response;
            response.type = (uint32_t)MessageType::ProcessListResponse;
            response.size = (uint32_t)payload.size();

            std::vector<uint8_t> message(sizeof(MessageHeader) + payload.size());
            memcpy(message.data(), &response, sizeof(response));
            memcpy(message.data() + sizeof(response), payload.data(), payload.size());

            // Send Response
            DWORD written;
            if (!WriteFile(pipe, message.data(), (DWORD)message.size(), &written, nullptr)) {
                std::cout << "[Thread " << std::this_thread::get_id() << "] Write failed!\n";
                break;
            }
        }
    }

    FlushFileBuffers(pipe);
    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);
    std::cout << "[Thread " << std::this_thread::get_id() << "] Handler exiting.\n";
}

void Server::startServer() {
    std::wcout << "Server Dispatcher Started. Listening on " << PIPE_NAME << "...\n";
    while (true) {
        HANDLE pipe = CreateNamedPipeW(
            PIPE_NAME,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES, // <--- Key change: Allow unlimited instances
            BUFSIZE,
            BUFSIZE,
            0,
            nullptr
        );

        if (pipe == INVALID_HANDLE_VALUE) {
            std::cerr << "CreateNamedPipe failed. Error: " << GetLastError() << "\n";
            Sleep(1000);
            continue;
        }

        BOOL connected = ConnectNamedPipe(pipe, nullptr) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (connected) {
            std::thread t(ClientHandler, pipe);
            t.detach();
        }
        else {
            CloseHandle(pipe);
        }
    }
}
