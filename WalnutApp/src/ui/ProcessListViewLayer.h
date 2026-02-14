#pragma once
#include "Walnut/Application.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"
#include <memory>
#include "../viewmodel/AppState.h"
#include "../worker/InspectorPoller.h"
#include <mutex>

class ProcessListViewLayer : public Walnut::Layer {
public:
	ProcessListViewLayer(
		std::shared_ptr<AppState> state, 
		std::shared_ptr<InspectorPoller> poller
	) :
		state(state), poller(poller) {
	}

	~ProcessListViewLayer();
	virtual void OnUIRender() override;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void UI_DrawAboutModal();
	void ShowAboutModal();

private:
	bool m_AboutModalOpen = false;
	std::shared_ptr<AppState> state;
	std::shared_ptr<InspectorPoller> poller;
};