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
	auto window = NuakeRenderer::Window("NuakeUI Demo");
	NuakeRenderer::ApplyNuakeImGuiTheme();

	// Load HTML file.
	auto canvas = NuakeUI::CanvasParser::Get().Parse("../resources/UI/test.xml");
	canvas->SetInputManager(new MyInputManager(window));

	// Set button click callback.
	auto btn = canvas->FindNodeByID<NuakeUI::Button>("main-btn");
	btn->SetClickCallback([]() {
		printf("I have been clicked! \n");
	});

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