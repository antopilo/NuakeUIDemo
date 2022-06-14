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
#include <NuakeUI/DataBindObject.h>

#include "MyInputManager.h"

#include <stdio.h>
int tabSelected = 0;

auto LoadCanvas()
{
	using namespace NuakeUI;
	
	// Load HTML file.
	auto canvas = CanvasParser::Get().Parse("../resources/UI/demo.html");
	
	// Creating data model and binding it to a node.
	auto dataModel = DataModel::New("myDataModel");
	dataModel->Bind("tab", &tabSelected);
	canvas->GetRootNode()->Model = dataModel;
	
	auto tab1Btn = canvas->FindNodeByID<Button>("tab1-btn");
	auto tab2Btn = canvas->FindNodeByID<Button>("tab2-btn");
	auto tab3Btn = canvas->FindNodeByID<Button>("tab3-btn");

	auto tab1 = canvas->FindNodeByID<Node>("tab1");
	tab1->ModelIf = DataModelOperation::New("tab", OperationType::If, ComparaisonType::Equal);
	tab1->ModelIf->RightI = 0;
	
	auto tab2 = canvas->FindNodeByID<Node>("tab2");
	tab2->ModelIf = DataModelOperation::New("tab", OperationType::If, ComparaisonType::Equal);
	tab2->ModelIf->RightI = 1;

	auto tab3 = canvas->FindNodeByID<Node>("tab3");
	tab3->ModelIf = DataModelOperation::New("tab", OperationType::If, ComparaisonType::Equal);
	tab3->ModelIf->RightI = 2;

	tab1Btn->UserData = 0;
	tab2Btn->UserData = 1;
	tab3Btn->UserData = 2;
	auto callback = [](NuakeUI::Button& btn)
	{
		auto model = btn.GetModel();
		tabSelected = std::any_cast<int>(btn.UserData);
	};

	tab1Btn->SetClickCallback(callback);
	tab2Btn->SetClickCallback(callback);
	tab3Btn->SetClickCallback(callback);

	return canvas;
}

void main()
{
	auto window = NuakeRenderer::Window("NuakeUI Demo", {1280, 720});
	NuakeRenderer::ApplyNuakeImGuiTheme();

	auto inputManager = new MyInputManager(window);

	auto canvas = LoadCanvas();
	canvas->SetInputManager(inputManager);
	while (!window.ShouldClose())
	{
		NuakeRenderer::Begin();

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