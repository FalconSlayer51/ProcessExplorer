#include "Walnut/Application.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"
#include "ProcessListViewLayer.h"
#include <iostream>


void ProcessListViewLayer::OnUIRender()
{
	ImGui::Begin("Processes");
	{
		std::lock_guard<std::mutex> lock(state->mutex);
		std::cout << state->processes.size() << std::endl;
		for (auto& p : state->processes) {
			bool selected =
				state->selectedProcess == p.pid;

			if (ImGui::Selectable(
				p.name.c_str(),
				selected))
			{
				state->selectedProcess = p.pid;
			}
		}
	}

	ImGui::End();
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
