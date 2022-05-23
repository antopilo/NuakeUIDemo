#include <stdio.h>
#include "MyInputManager.h"

#include <NuakeRenderer/Window.h>
#include <NuakeRenderer/NuakeRenderer.h>

#include <NuakeUI/Canvas.h>
#include <NuakeUI/Button.h>
#include <NuakeUI/Checkbox.h>

void DrawUI(std::shared_ptr<NuakeUI::Node> node)
{
	if (ImGui::TreeNode(node->GetID().c_str()))
	{
		for (auto& c : node->GetChildrens())
			DrawUI(c);

		ImGui::TreePop();
	}
}

void main()
{
	auto window = NuakeRenderer::Window("Hello");
	NuakeRenderer::ApplyNuakeImGuiTheme();

	auto canvas = NuakeUI::Canvas();
	canvas.SetInputManager(new MyInputManager(window));

	auto root = NuakeUI::Node::New("Root");
	canvas.SetRootNode(root);

	root->InsertChild(NuakeUI::Node::New("Children 1"));
	root->InsertChild(NuakeUI::Button::New("My Button", "Hello"));
	root->InsertChild(NuakeUI::Checkbox::New("My Checkbox"));

	root->SetWidthPercent(100.f);
	root->SetHeightPercent(100.f);
	root->SetBorder(15.f);
	root->SetBorderColor({ 1.15f, 0.15f, 0.15f, 1.f });
	root->Style.background_color.r = 0.1f;
	root->Style.background_color.g = 0.1f;
	root->Style.background_color.b = 0.1f;
	root->Style.background_color.a = 0.1f;
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

	while (!window.ShouldClose())
	{
		NuakeRenderer::Clear();
		NuakeRenderer::PollEvents();

		canvas.Calculate(window.GetWindowSize());
		canvas.Tick();
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

		canvas.Draw();

		NuakeRenderer::BeginImGuiFrame();

		if (ImGui::Begin("Inspector"))
		{
			DrawUI(root);
		}
		ImGui::End();

		NuakeRenderer::EndImGuiFrame();

		window.SwapBuffers();
	}
}