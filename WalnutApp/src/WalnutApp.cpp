#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"
#include "ui/ProcessListViewLayer.h"
#include "inspector/InspectorIPCClient.h"
#include "worker/InspectorPoller.h"


Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";
	spec.CustomTitlebar = true;

	Walnut::Application* app = new Walnut::Application(spec);
	auto state = std::make_shared<AppState>();
	auto inspector = std::make_unique<InspectorIPCClient>();
	auto poller = std::make_unique<InspectorPoller>(std::move(inspector), state);
	auto exampleLayer = std::make_shared<ProcessListViewLayer>(state, std::move(poller));
	app->PushLayer(exampleLayer);
	return app;
}