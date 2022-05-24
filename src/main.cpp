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
	
	auto window = NuakeRenderer::Window("Hello");
	NuakeRenderer::ApplyNuakeImGuiTheme();

	auto test = NuakeUI::CanvasParser::Get().Parse("../resources/UI/test.xml");

	auto canvas = NuakeUI::Canvas::New();
	canvas->SetInputManager(new MyInputManager(window));

	auto root = NuakeUI::Node::New("Root");
	canvas->SetRootNode(test);
	root->InsertChild(NuakeUI::Node::New("Children 1"));
	root->InsertChild(NuakeUI::Button::New("My Button", "Hello"));
	root->InsertChild(NuakeUI::Checkbox::New("My Checkbox"));
	root->InsertChild(NuakeUI::Text::New("myText", "Hello World :)"));

	root->SetWidthPercent(100.f);
	root->SetHeightPercent(100.f);
	root->SetBorder(15.f);
	root->SetBorderColor({ 1.15f, 0.15f, 0.15f, 1.f });
	root->SetDisplay(YGDisplayFlex);
	root->SetJustify(YGJustifyCenter);
	root->SetAlignItem(YGAlignCenter);

	auto firstChild = root->FindChildByID<NuakeUI::Node>("Children 1");
	firstChild->SetWidthPixel(100);
	firstChild->SetHeightPixel(100);
	firstChild->SetBorder(8.f);
	firstChild->SetBorderColor({ 0.15, 0.15, 0.15, 1.0f });
	firstChild->Style.background_color = { 0.8, 0.2, 0.2, 1 };

	auto button = root->FindChildByID<NuakeUI::Button>("My Button");
	button->SetWidthPixel(100); button->SetHeightPixel(100);
	button->SetBorder(8.f);
	button->SetBorderColor({ 0.15, 0.15, 0.15, 1.0f });
	button->Style.background_color = { 0.2, 0.0, 0.2, 1 };

	button->SetClickCallback([]() {
		printf("Hello, I have been clicked!! \n");
	});

	auto checkbox = root->FindChildByID<NuakeUI::Checkbox>("My Checkbox");
	checkbox->SetWidthPixel(100); checkbox->SetHeightPixel(100);
	checkbox->SetBorder(8.f);
	checkbox->SetBorderColor({ 0.15, 0.15, 0.15, 1.0f });
	checkbox->Style.background_color = { 0.2, 0.2, 0.0, 1 };

	auto myText = root->FindChildByID<NuakeUI::Text>("myText");
	myText->SetBorder(4.f);
	myText->SetBorderColor({ 1, 0, 0, 1 });
	myText->FontStyle.Alignment = NuakeUI::TextAlign::Center;
	while (!window.ShouldClose())
	{
		NuakeRenderer::Clear();
		NuakeRenderer::PollEvents();

		canvas->Calculate(window.GetWindowSize());
		canvas->Tick();
		if (button->GetState() == NuakeUI::State::Hover)
		{
			button->Style.background_color.r = 0.5f;
			button->Style.border = 8.f;
		}
		else if (button->GetState() == NuakeUI::State::Pressed)
		{
			button->Style.border = 16.f;
			button->Style.background_color.r = 1.0f;
		}
		else
		{
			button->Style.background_color.r = 0.f;
			button->Style.border = 8.f;
		}

		if (checkbox->Checked())
		{
			checkbox->Style.background_color = { 0.2, 0.8, 0.0, 1 };
		}
		else
		{
			checkbox->Style.background_color = { 0.8, 0.2, 0.0, 1 };
		}
		canvas->Draw();

		NuakeUI::DrawInspector(canvas);


		window.SwapBuffers();
	}
}