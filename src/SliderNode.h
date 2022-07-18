#pragma once
#include <NuakeUI/Node.h>
#include <iostream>
using namespace NuakeUI;

class SliderHandle;
typedef std::shared_ptr<SliderHandle> SliderHandlePtr;
class SliderHandle : public NuakeUI::Node
{
private:
	bool isHandleGrabbed = false;
	const uint32_t SIZE = 20;
public:
	float Delta = 0.0f;

	static SliderHandlePtr New(const std::string& id, const std::string& value = "")
	{
		return std::make_shared<SliderHandle>(id);
	}

	SliderHandle(const std::string& id)
	{
		InitializeNode();

		std::map<StyleProperties, PropValue> styles;
		ComputedStyle.BackgroundColor = Color(0.5, 0.5, 0.5, 1);
		ComputedStyle.BorderRadius = 8.f;
		styles[StyleProperties::Height] = PropValue{ PropValueType::Pixel, 20.0f };
		styles[StyleProperties::Width] = PropValue{ PropValueType::Pixel, 20.0f };
		ApplyStyleProperties(styles);

	};
	~SliderHandle() = default;

	void OnTick(InputManager* inputManager) override
	{
		if (isHandleGrabbed)
		{
			float mouseX = inputManager->GetMouseX();
			float positionX = mouseX - Parent->ComputedPosition.x - (SIZE / 2.0);
			positionX = std::min(Parent->ComputedSize.x - SIZE, positionX);
			positionX = std::max(0.0f, positionX);

			float minX = 0.0f;
			float maxX = Parent->ComputedSize.x - SIZE;
			Delta = positionX / maxX;

			YGNodeStyleSetPosition(mNode, YGEdge::YGEdgeLeft, positionX);
		}
	}

	void OnClick(Vector2 position) override
	{
		isHandleGrabbed = true;
	}

	void OnClickReleased(Vector2 mousePosition) override
	{
		isHandleGrabbed = false;
	}
};

class SliderNode;
typedef std::shared_ptr<SliderNode> SliderPtr;

class SliderNode : public NuakeUI::Node
{
public:
	enum class SliderType
	{
		Normal, 
		Drag
	};

	SliderType Type = SliderType::Normal;

	float MaxValue;
	float MinValue;
	float Value;
	float StepSize;

	SliderHandlePtr Handle;
	TextPtr Label;

	Vector2 PosStart;
	bool Dragging = false;

	static SliderPtr New(const std::string& id, const std::string& value)
	{
		return std::make_shared<SliderNode>(id, value);
	}

	SliderNode(const std::string& id, const std::string& value) : MaxValue(100.0f), MinValue(0.0f), StepSize(1.0f), Value(0.0f)
	{
		InitializeNode();
		this->ComputedStyle.BackgroundColor = Color(0.2, 0.2, 0.2, 1);
		this->ComputedStyle.BorderRadius = 4.f;

		std::map<StyleProperties, PropValue> styles;
		styles[StyleProperties::FlexDirection] = (int)FlexDirectionType::Row;
		styles[StyleProperties::AlignItems] = (int)AlignItemsType::Center;
		ApplyStyleProperties(styles);

		if (Type != SliderType::Drag)
		{
			Handle = SliderHandle::New("SliderHandle");
			this->InsertChild(Handle);
		}
		
		Label = Text::New("text", "100");
		Label->ComputedStyle.FontSize = 32.f;
		this->InsertChild(Label);
	}

	~SliderNode()
	{

	}

	void OnTick(InputManager* inputManager) override
	{
		Label->Lines[0] = std::to_string((int)Value);

		float center = ComputedSize.x / 2.0 - (Label->ComputedSize.x / 2.0);
		YGNodeStyleSetPosition(Label->GetYogaNode(), YGEdge::YGEdgeLeft, center);

		if (Type == SliderType::Drag )
		{
			float mouseX = inputManager->GetMouseX();
			if (Dragging)
			{
				float delta = (PosStart.x - mouseX) * StepSize;
				Value -= delta;
				PosStart.x = mouseX;
				//std::cout << "State: " << (int)State << std::endl;
			}
		}
		else
		{
			Value = MinValue + (Handle->Delta * (MaxValue - MinValue));
		}

		//std::cout << "Value: " << Value  << " Dragging: " << Dragging << std::endl;
	}

	void OnClick(Vector2 mousePosition) override
	{
		if(Type !=  SliderType::Drag)
			Handle->OnClick(mousePosition);

		Dragging = true;

		PosStart = mousePosition;

		std::cout << "OnClick " << "State: " << (int)State << std::endl;
	}

	void OnClickReleased(Vector2 mousePosition) override
	{
		if (Type != SliderType::Drag)
			Handle->OnClickReleased(mousePosition);
		Dragging = false;

		std::cout << "OnClickReleased" << std::endl;
	}
private:

};