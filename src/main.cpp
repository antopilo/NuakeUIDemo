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

	const auto inputManager = new MyInputManager(window);

	// Load HTML file.
	const std::string filePath = "../resources/UI/test.xml";

	auto canvas = NuakeUI::CanvasParser::Get().Parse(filePath);
	canvas->SetInputManager(inputManager);

	while (!window.ShouldClose())
	{
		NuakeRenderer::Begin();

		// Reload html file.
		if (glfwGetKey(window.GetHandle(), GLFW_KEY_0))
		{
			canvas = NuakeUI::CanvasParser::Get().Parse(filePath);
			canvas->SetInputManager(inputManager);
		}

		if (glfwGetKey(window.GetHandle(), GLFW_KEY_1))
			NuakeUI::Renderer::Get().ReloadShaders();

		canvas->Calculate(window.GetWindowSize());
		canvas->Tick();
		canvas->Draw();

		NuakeUI::DrawInspector(canvas);

		window.SwapBuffers();
	}
}