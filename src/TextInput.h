#pragma once
#include <NuakeUI/Node.h>
#include <NuakeUI/Text.h>

#include <memory>

using namespace NuakeUI;

class TextInput : public Node
{
private:
	std::string Buffer = "TextInput";
	TextPtr TextLabel;

public:
	static std::shared_ptr<TextInput> New(const std::string& id, const std::string& name)
	{
		return std::make_shared<TextInput>(id, name);
	}

	TextInput(const std::string& id, const std::string& name)
	{
		InitializeNode();
		CanGrabFocus = true;

		std::map<StyleProperties, PropValue> styles;
		styles[StyleProperties::BackgroundColor] = Color(1.f, 0.f, 0.f, 255.f);
		styles[StyleProperties::BorderColor] = Color(0.f, 255.f, 0.f, 255.f);
		styles[StyleProperties::Height] = PropValue(PropValueType::Auto, 20.f);
		styles[StyleProperties::BorderSize] = 2.0f;

		ComputedStyle.BorderRadius = 4.0f;
		ComputedStyle.BorderSize = 2.0f;
		styles[StyleProperties::PaddingLeft] = PropValue(PropValueType::Pixel, 8.f);;
		styles[StyleProperties::PaddingTop] = PropValue(PropValueType::Pixel, 4.f);;
		styles[StyleProperties::PaddingBottom] = PropValue(PropValueType::Pixel, 4.f);;
		styles[StyleProperties::PaddingRight] = PropValue(PropValueType::Pixel, 8.f);;
		ApplyStyleProperties(styles);

		TextLabel = Text::New("textInput", "Hello");

		InsertChild(TextLabel);
	}

	~TextInput() = default;

	virtual void OnTick(InputManager* inputManager) override
	{
		TextLabel->SetText(Buffer);

		if (HasFocus())
		{
			if (std::size(inputManager->InputStack) > 0)
			{
				std::string result = inputManager->ConsumeStack();
				if (result == "_backspace")
				{
					if(std::size(Buffer) > 0)
						Buffer.erase(Buffer.end() - 1);
				}
				else
				{
					Buffer += result;
				}
			}

			ComputedStyle.BorderSize = 2.0f;
		}
		else
			ComputedStyle.BorderSize = 0.f;
	}

	virtual void OnClick(Vector2 mousePosition) override
	{
		
	}
};