#include <NuakeRenderer/Window.h>
#include <NuakeUI/NuakeUI.h>

#include "MyInputManager.h"
#include "SliderNode.h"
#include <NuakeUI/Inspector.h>

class myDataModel
{
public:
	myDataModel() = default;
	~myDataModel() = default;

	int tabSelected = 0;
};

class Main
{
public:
	myDataModel& TabDataModel;

	auto LoadCanvas()
	{
		using namespace NuakeUI;

		// Load HTML file.
		auto parser = CanvasParser();
		parser.RegisterNodeType("slider", SliderNode::New);

		auto canvas = parser.Parse("../resources/UI/demo.html");
		
		// Creating data model and binding it to a node.
		auto dataModel = DataModel::New("myDataModel");
		dataModel->Bind("tab", &TabDataModel.tabSelected);
		
		canvas->GetRoot()->SetDataModel(dataModel);

		ButtonPtr btnContainer;
		if (canvas->FindNodeByID<Button>("btn-container", btnContainer))
		{
			auto callback = [this](NuakeUI::Button& btn)
			{
				TabDataModel.tabSelected = btn.GetIndex();
			};
		
			for (uint32_t i = 0; i < btnContainer->GetChildrens().size(); i++)
			{
				ButtonPtr button = btnContainer->GetChild<Button>(i);
				button->SetClickCallback((callback));
			}
		}

		return canvas;
	}

	Main() : TabDataModel(myDataModel())
	{
		using namespace NuakeRenderer;
		using namespace NuakeUI;
		
		auto window = Window("NuakeUI Demo", { 1280, 720 });
		ApplyNuakeImGuiTheme();

		auto inputManager = new MyInputManager(window);

		auto canvas = LoadCanvas();
		canvas->SetInputManager(inputManager);

		while (!window.ShouldClose())
		{
			Begin();

			// Reload html file.
			if (glfwGetKey(window.GetHandle(), GLFW_KEY_0))
			{
				auto parser = CanvasParser();
				canvas = parser.Parse("../resources/UI/demo.html");
				canvas->SetInputManager(inputManager);
			}

			if (glfwGetKey(window.GetHandle(), GLFW_KEY_1))
				Renderer::Get().ReloadShaders();

			if (glfwGetKey(window.GetHandle(), GLFW_KEY_F5))
			{
				canvas = LoadCanvas();
				canvas->SetInputManager(inputManager);
			}

			canvas->ComputeLayout(window.GetWindowSize());
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