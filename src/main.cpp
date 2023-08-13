#include <NuakeRenderer/Window.h>
#include <NuakeUI/NuakeUI.h>

#include "MyInputManager.h"
#include "SliderNode.h"
#include <NuakeUI/Inspector.h>

#include <NuakeUI/Font/FontManager.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace NuakeUI;

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

	CanvasPtr _canvas;
	InputManager* _inputmanager;

	bool needsReload = false;

	void SetCallbacks(CanvasPtr canvas)
	{
		// system menu
		ButtonPtr fileMenuBtn;
		if (canvas->FindNodeByID<Button>("file-menu-btn", fileMenuBtn))
		{
			auto callback = [this](NuakeUI::Button& btn)
			{
				TabDataModel.fileMenuContext->ComputedStyle.Visibility = (VisibilityType)!(bool)TabDataModel.fileMenuContext->ComputedStyle.Visibility;
			};

			fileMenuBtn->SetClickCallback(callback);
		}

		ButtonPtr reloadBtn;
		if (canvas->FindNodeByID<Button>("reload-btn", reloadBtn))
		{
			reloadBtn->SetClickCallback([this](NuakeUI::Button& btn)
				{
					needsReload = true;
				});
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

			for (uint32_t i = 0; i < btnContainer->GetChildrens().size(); i++)
			{
				ButtonPtr button = btnContainer->GetChild<Button>(i);
				button->SetClickCallback((callback));
			}
		}
	}

	CanvasPtr LoadCanvas()
	{
		using namespace NuakeUI;

		// Load HTML file.
		auto parser = CanvasParser();
		parser.RegisterNodeType("slider", SliderNode::New);

		auto canvas = parser.Parse("../resources/UI/demo.html");
		
		return canvas;
	}

	Main() : TabDataModel(myDataModel())
	{
		using namespace NuakeUI;

		// Initialize renderer
		auto window = NuakeRenderer::Window("NuakeUI Demo", { 1280, 720 });
		NuakeRenderer::SetMSAA(false);
		NuakeRenderer::SetMSAASamples(2);
		NuakeRenderer::ApplyNuakeImGuiTheme();

		_inputmanager = new MyInputManager(window);
		_canvas = LoadCanvas();
		_canvas->SetInputManager(_inputmanager);
		SetCallbacks(_canvas);

		while (!window.ShouldClose())
		{
			NuakeRenderer::Begin();

			if (_inputmanager->IsKeyPressed(GLFW_KEY_F5) || needsReload)
			{
				Renderer::Get().ReloadShaders();
				_canvas = LoadCanvas();
				_canvas->SetInputManager(_inputmanager);
				SetCallbacks(_canvas);

				needsReload = false;
			}

			_canvas->ComputeLayout(window.GetWindowSize());
			_canvas->Tick();
			_canvas->Draw();

			NuakeUI::DrawInspector(_canvas);

			window.SwapBuffers();
		}
	}
};

void main()
{
	Main m = Main();
}