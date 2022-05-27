#include <NuakeRenderer/Window.h>
#include <NuakeRenderer/NuakeRenderer.h>

// UI components
#include <NuakeUI/Canvas.h>
#include <NuakeUI/Button.h>
#include <NuakeUI/Checkbox.h>
#include <NuakeUI/Text.h>

#include <NuakeUI/Renderer.h>
#include <NuakeUI/Inspector.h>
#include <NuakeUI/CanvasParser.h>

#include "MyInputManager.h"

#include <stdio.h>

void main()
{
	auto window = NuakeRenderer::Window("NuakeUI Demo", {800, 600});
	NuakeRenderer::ApplyNuakeImGuiTheme();

	// Load HTML file.
	auto canvas = NuakeUI::CanvasParser::Get().Parse("../resources/UI/demo.html");
	canvas->SetInputManager(new MyInputManager(window));

	auto tab1Btn = canvas->FindNodeByID<NuakeUI::Button>("tab1-btn");
	auto tab2Btn = canvas->FindNodeByID<NuakeUI::Button>("tab2-btn");

	auto tab1 = canvas->FindNodeByID<NuakeUI::Node>("tab1");
	auto tab2 = canvas->FindNodeByID<NuakeUI::Node>("tab2");

	struct tabsData
	{
		std::vector<std::shared_ptr<NuakeUI::Node>> tabs;
		int tabId;
	};

	tabsData data = {
		{ tab1, tab2 },
		0
	};

	data.tabId = 0;
	tab1Btn->UserData = data;

	data.tabId = 1;
	tab2Btn->UserData = data;

	auto& callback = [](NuakeUI::Button& btn) 
	{
		auto tab = std::any_cast<tabsData>(btn.UserData);
		if (tab.tabs[tab.tabId]->HasClass("visible"))
			return;

		tab.tabs[tab.tabId]->RemoveClass("hidden");
		tab.tabs[tab.tabId]->AddClass("visible");

		int i = 0;
		for (auto& t : tab.tabs)
		{
			if (i != tab.tabId)
			{
				t->RemoveClass("visible");
				t->AddClass("hidden");
			}
			i++;
		}
	};

	// TODO: Has class check.

	tab1Btn->SetClickCallback(callback);
	tab2Btn->SetClickCallback(callback);

	while (!window.ShouldClose())
	{
		NuakeRenderer::Begin();

		canvas->Calculate(window.GetWindowSize());
		canvas->Tick();
		canvas->Draw();

		NuakeUI::DrawInspector(canvas);

		window.SwapBuffers();
	}
}