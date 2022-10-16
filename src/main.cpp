#include <NuakeRenderer/Window.h>
#include <NuakeUI/NuakeUI.h>

#include "MyInputManager.h"
#include "SliderNode.h"
#include <NuakeUI/Inspector.h>
#include "TextInput.h"

#include <NuakeUI/Font/FontManager.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class myDataModel
{
public:
	myDataModel() = default;
	~myDataModel() = default;

	int tabSelected = 0;
	NodePtr fileMenuContext;
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
		parser.RegisterNodeType("textInput", TextInput::New);

		auto canvas = parser.Parse("../resources/UI/demo.html");
		
		// system menu
		ButtonPtr fileMenuBtn;
		if (canvas->FindNodeByID<Button>("file-menu-btn", fileMenuBtn))
		{
			auto callback = [this](NuakeUI::Button& btn)
			{
				TabDataModel.fileMenuContext->ComputedStyle.Visibility =  (VisibilityType)!(bool)TabDataModel.fileMenuContext->ComputedStyle.Visibility;
			};

			fileMenuBtn->SetClickCallback(callback);
		}

		canvas->FindNodeByID<Node>("file-context-menu", TabDataModel.fileMenuContext);

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
		
			//for (uint32_t i = 0; i < btnContainer->GetChildrens().size(); i++)
			//{
			//	ButtonPtr button = btnContainer->GetChild<Button>(i);
			//	button->SetClickCallback((callback));
			//}
		}

		return canvas;
	}

	Main() : TabDataModel(myDataModel())
	{
		using namespace NuakeRenderer;
		using namespace NuakeUI;
		


		auto window = Window("NuakeUI Demo", { 1280, 720 });
		glEnable(GL_MULTISAMPLE);
		glfwWindowHint(GLFW_SAMPLES, 8);

		ApplyNuakeImGuiTheme();

		auto inputManager = new MyInputManager(window);

		auto canvas = LoadCanvas();
		canvas->SetInputManager(inputManager);

		while (!window.ShouldClose())
		{
			Begin();

			if (glfwGetKey(window.GetHandle(), GLFW_KEY_1))
			{
				Renderer::Get().ReloadShaders();
			}

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
	Main m = Main();
}