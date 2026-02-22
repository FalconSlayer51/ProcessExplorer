#pragma once
#include "../inspector/IInspectorClient.h"
#include <memory>
#include <iostream>
#include <thread>
#include <mutex>

class InspectorPoller {
private:
	std::shared_ptr<IInspectorClient> client;
	std::shared_ptr<AppState> state;
	std::thread worker;
	std::atomic<bool> running{ false };

public:
	InspectorPoller(std::shared_ptr<IInspectorClient> inspectorClient, std::shared_ptr<AppState> state)
		:client(inspectorClient), state(state) {
		std::cout << "Inspector initialized" << std::endl;
	}

	void start() {
		if (running == true)
			return;
		running = true;
		worker = std::thread(&InspectorPoller::run, this);
	}

	void stop() {
		running = false;
		if (worker.joinable()) {
			worker.join();
		}
	}

	~InspectorPoller() {
		stop();
	}

private:

	void run() {
		static int tick = 0;
		while (running) {
			auto list = client->getProcessList();

			{
				std::lock_guard<std::mutex> lock(state->mutex);
				state->processes = std::move(list);
				std::cout << state->processes[67].memInfo.PrivateUsage << std::endl;
			}


			std::this_thread::sleep_for(
				std::chrono::seconds(1)
			);
		}
	}
};