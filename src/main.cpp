#include <stdio.h>
#include <NuakeRenderer/Window.h>
#include <NuakeRenderer/NuakeRenderer.h>

#include <NuakeUI/Canvas.h>
#include <NuakeUI/Button.h>

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
	auto root = NuakeUI::Node::New("Root");
	root->InsertChild(NuakeUI::Node::New("Children 1"));
	root->InsertChild(NuakeUI::Button::New("Children 2", "Hello"));
	
	root->SetWidthPercent(50.f);
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
	firstChild->InsertChild(NuakeUI::Node::New("Children 3"));

	firstChild->SetWidthPixel(100);
	firstChild->SetHeightPixel(100);
	firstChild->SetBorder(8.f);
	firstChild->SetBorderColor({ 0.15, 0.15, 0.15, 1.0f });
	firstChild->Style.background_color = { 0.8, 0.2, 0.2, 1 };

	auto button = root->GetChild<NuakeUI::Button>(1);
	button->SetWidthPixel(100);
	button->SetHeightPixel(100);
	button->SetBorder(16.f);
	button->SetBorderColor({ 0.15, 0.15, 0.15, 1.0f });
	button->Style.background_color = { 0.2, 0.0, 0.2, 1 };

	canvas.SetRootNode(root);

	root->FindChildByID<NuakeUI::Node>("Children 3");
	
	float t = 0.f;
	while (!window.ShouldClose())
	{
		t += 0.001;

		NuakeRenderer::Clear();
		NuakeRenderer::PollEvents();

		canvas.Calculate(window.GetWindowSize());
		button->Style.background_color.r = (std::sin(t) + 1.0f) / 2.0f;;
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