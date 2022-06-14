#include <NuakeRenderer/Window.h>
#include <NuakeRenderer/NuakeRenderer.h>

// UI components
#include <NuakeUI/NuakeUI.h>

#include "MyInputManager.h"

#include <stdio.h>

class myDataModel
{
public:
	myDataModel()
	{

	}

	~myDataModel()
	{

	}

	int tabSelected = 0;
};


using namespace NuakeUI;

class Main
{
public:
	myDataModel& TabDataModel;

	auto LoadCanvas()
	{
		// Load HTML file.
		auto canvas = CanvasParser::Get().Parse("../resources/UI/demo.html");
		
		// Creating data model and binding it to a node.
		auto dataModel = DataModel::New("myDataModel");
		dataModel->Bind("tab", &TabDataModel.tabSelected);

		canvas->SetDataModel(dataModel);

		//ButtonRef btnContainer;
		//if (canvas->FindNodeByID<Button>("btn-container", btnContainer))
		//{
		//	auto callback = [this](NuakeUI::Button& btn)
		//	{
		//		TabDataModel.tabSelected = btn.GetIndex();
		//	};
		//
		//	for (uint32_t i = 0; i < btnContainer->GetChildrens().size(); i++)
		//	{
		//		ButtonRef button = btnContainer->GetChild<Button>(i);
		//		button->SetClickCallback((callback));
		//	}
		//}

		return canvas;
	}

	Main() : TabDataModel(myDataModel())
	{
		auto window = NuakeRenderer::Window("NuakeUI Demo", { 1280, 720 });
		NuakeRenderer::ApplyNuakeImGuiTheme();

		auto inputManager = new MyInputManager(window);

		auto canvas = LoadCanvas();
		canvas->SetInputManager(inputManager);

		while (!window.ShouldClose())
		{
			NuakeRenderer::Begin();

			// Reload html file.
			if (glfwGetKey(window.GetHandle(), GLFW_KEY_0))
			{
				canvas = NuakeUI::CanvasParser::Get().Parse("../resources/UI/demo.html");
				canvas->SetInputManager(inputManager);
			}

			if (glfwGetKey(window.GetHandle(), GLFW_KEY_1))
				NuakeUI::Renderer::Get().ReloadShaders();

			if (glfwGetKey(window.GetHandle(), GLFW_KEY_F5))
			{
				canvas = LoadCanvas();
				canvas->SetInputManager(inputManager);
			}

			canvas->Calculate(window.GetWindowSize());
			canvas->Tick();
			canvas->Draw();

			NuakeUI::DrawInspector(canvas);

			window.SwapBuffers();
		}
	}

};


void main()
{
	auto m = Main();
}