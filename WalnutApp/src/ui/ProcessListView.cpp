#include "ProcessListViewLayer.h"
#include "Walnut/Application.h"
#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"
#include <iostream>
#include <cstdio> // For snprintf

// Helper function to format raw bytes into readable Megabytes (MB)
static std::string FormatBytes(SIZE_T bytes) {
	char buf[64];
	double mb = static_cast<double>(bytes) / (1024.0 * 1024.0);
	snprintf(buf, sizeof(buf), "%.2f MB", mb);
	return std::string(buf);
}

void ProcessListViewLayer::OnUIRender()
{
	// Set a nice default starting size for the window
	ImGui::SetNextWindowSize(ImVec2(1000, 600), ImGuiCond_FirstUseEver);

	ImGui::Begin("Task Manager");
	{
		std::lock_guard<std::mutex> lock(state->mutex);

		//// ==========================================
		//// LEFT SIDEBAR: Process List
		//// ==========================================
		//// Create a child window fixed to 250px wide. 'true' gives it a border.
		//ImGui::BeginChild("Sidebar", ImVec2(250, 0), true);
		//ImGui::Text("Processes (%zu)", state->processes.size());
		//ImGui::Separator();

		//for (auto& p : state->processes) {
		//	// std::optional safely compares against the raw DWORD
		//	bool selected = (state->selectedProcess == p.pid);
		//	std::string label = p.name + " (" + std::to_string(p.pid) + ")";

		//	if (ImGui::Selectable(label.c_str(), selected)) {
		//		state->selectedProcess = p.pid;
		//	}
		//}
		//ImGui::EndChild();

		//// Put the next UI element on the same line (creates the split pane effect)
		//ImGui::SameLine();

		// ==========================================
		// RIGHT MAIN AREA: Detailed Data Table
		// ==========================================
		ImGui::BeginChild("MainArea", ImVec2(0, 0), false);
		ImGui::Text("Process Details");
		ImGui::Separator();

		// Professional Table Flags (Resizable, alternating row backgrounds, scrollable)
		static ImGuiTableFlags flags =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
			ImGuiTableFlags_Sortable | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
			ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY;

		if (ImGui::BeginTable("ProcessTable", 6, flags)) {
			// Freeze the top row so headers stay visible when scrolling down
			ImGui::TableSetupScrollFreeze(0, 1);

			// Define columns
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort);
			ImGui::TableSetupColumn("PID");
			ImGui::TableSetupColumn("Parent PID");
			ImGui::TableSetupColumn("Working Set (RAM)");
			ImGui::TableSetupColumn("Private Bytes");
			ImGui::TableSetupColumn("Page Faults");
			ImGui::TableHeadersRow();

			for (auto& p : state->processes) {
				ImGui::TableNextRow();
				bool selected = (state->selectedProcess == p.pid);

				// Column 0: Name (Span selectable across the whole row)
				ImGui::TableSetColumnIndex(0);
				if (ImGui::Selectable(p.name.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap)) {
					state->selectedProcess = p.pid;
				}

				// Column 1: PID
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%u", p.pid);

				// Column 2: Parent PID
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%u", p.parentId);

				// Column 3: Working Set (Physical RAM actively used)
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%s", FormatBytes(p.memInfo.WorkingSetSize).c_str());

				// Column 4: Private Bytes (Memory allocated exclusively by this process)
				ImGui::TableSetColumnIndex(4);
				ImGui::Text("%s", FormatBytes(p.memInfo.PrivateUsage).c_str());

				// Column 5: Page Faults
				ImGui::TableSetColumnIndex(5);
				ImGui::Text("%lu", p.memInfo.PageFaultCount);
			}
			ImGui::EndTable();
		}
		ImGui::EndChild();
	}
	ImGui::End();

	// Render the About Modal if it's open
	UI_DrawAboutModal();
}

void ProcessListViewLayer::OnAttach() {
	poller->start();
}

void ProcessListViewLayer::OnDetach() {
	poller->stop();
}

ProcessListViewLayer::~ProcessListViewLayer()
{
	std::cout << "ProcessListViewLayer Destroyed" << std::endl;
}

void ProcessListViewLayer::UI_DrawAboutModal()
{
	if (!m_AboutModalOpen)
		return;

	ImGui::OpenPopup("About");
	m_AboutModalOpen = ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (m_AboutModalOpen)
	{
		auto image = Walnut::Application::Get().GetApplicationIcon();
		ImGui::Image(image->GetDescriptorSet(), { 48, 48 });

		ImGui::SameLine();
		Walnut::UI::ShiftCursorX(20.0f);

		ImGui::BeginGroup();
		ImGui::Text("Walnut application framework");
		ImGui::Text("by Studio Cherno.");
		ImGui::EndGroup();

		if (Walnut::UI::ButtonCentered("Close"))
		{
			m_AboutModalOpen = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void ProcessListViewLayer::ShowAboutModal()
{
	m_AboutModalOpen = true;
}