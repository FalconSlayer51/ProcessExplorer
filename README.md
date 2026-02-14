# ProcessExplorer

**ProcessExplorer** is a high-performance, real-time Windows process manager built from scratch using C++. It separates the backend logic from the frontend GUI using a robust Client-Server architecture powered by Windows Named Pipes (IPC).

*(Replace this link with an actual screenshot of your app later)*

## Key Features

* **Real-Time Monitoring:** View active processes, PIDs, and hierarchy.
* **Client-Server Architecture:** * **Server (Backend):** A standalone, multithreaded Win32 service that queries low-level system APIs.
* **Client (Frontend):** A lightweight, responsive GUI that connects to the server via IPC.


* **Inter-Process Communication (IPC):** Uses Windows Named Pipes with a custom binary protocol for fast, low-latency data transfer.
* **Modern GUI:** Built with **Dear ImGui** and the **Walnut** application framework (by The Cherno).
* **Multithreaded:** The server uses the Dispatcher Pattern to handle multiple client connections simultaneously without blocking.

## Tech Stack

* **Language:** C++ (C++17/20)
* **GUI Framework:** [Walnut](https://github.com/TheCherno/Walnut) (wrapping [Dear ImGui](https://github.com/ocornut/imgui))
* **Graphics API:** Vulkan / DirectX (via Walnut)
* **System API:** Win32 API (Process enumeration, Handles, Named Pipes)
* **Concurrency:** `std::thread`, `CreateNamedPipe` (Overlapped I/O model)
* **Build System:** Visual Studio Solution (`.sln`) / Premake (if applicable)

## Architecture

The project is split into two distinct processes to ensure stability and privilege separation:

1. **The Server (`ProcessExplorerServer`):**
* Runs as a background process (can be elevated to Administrator).
* Uses `CreateToolhelp32Snapshot` and `OpenProcess` to gather system data.
* Listens on `\\.\pipe\ProcessExplorerPipe`.
* Spawns a new worker thread for every connecting client.


2. **The Client (`ProcessExplorerClient`):**
* The user interface application.
* Connects to the server pipe.
* Sends requests (e.g., `GetProcessList`) and deserializes the binary response.
* Renders data using ImGui tables and widgets.



## Getting Started

### Prerequisites

* **OS:** Windows 10 or 11
* **Compiler:** Visual Studio 2022 (with "Desktop development with C++" workload)
* **Dependencies:**
* Vulkan SDK (Required by Walnut)
* Git



### Installation & Build

1. **Clone the Repository:**
```bash
git clone https://github.com/FalconSlayer51/ProcessExplorer.git
cd ProcessExplorer

```


2. **Initialize Submodules (Walnut/ImGui):**
```bash
git submodule update --init --recursive

```


3. **Generate Project Files (if using Premake):**
*(Skip this step if you are using a raw .sln file)*
```bash
call vendor\bin\premake\premake5.exe vs2022

```


4. **Build the Solution:**
* Open `ProcessExplorer.sln` in Visual Studio.
* Select **Release** or **Debug** configuration.
* Build the entire solution (F7).



## How to Run

Because this is a Client-Server application, you must run the components in a specific order:

1. **Start the Server:**
* Navigate to `bin/Release-windows-x86_64/ProcessExplorerServer`.
* Run `ProcessExplorerServer.exe`.
* *You should see: "Server Dispatcher Started. Listening on pipe..."*


2. **Start the Client:**
* Navigate to `bin/Release-windows-x86_64/ProcessExplorerClient`.
* Run `ProcessExplorerClient.exe`.
* The GUI should launch and populate with process data immediately.



## Contributing

Contributions are welcome! If you have ideas for new features (e.g., killing processes, viewing memory usage, or graphs), feel free to fork the repo and submit a Pull Request.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request


* **The Cherno:** for the excellent [Walnut](https://github.com/TheCherno/Walnut) framework and C++ tutorials.
* **ocornut:** for the incredible [Dear ImGui](https://github.com/ocornut/imgui) library.
* **Microsoft:** for the Win32 API documentation.
