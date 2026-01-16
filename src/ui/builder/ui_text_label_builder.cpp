#include "ui/builder/ui_text_label_builder.h"
#include <glm/ext/vector_float3.hpp>
#include "ui/core/ui_component.h"

namespace TheEngine::UI
{


	UITextLabelBuilder::UITextLabelBuilder(UICoreSystem& uiCoreSystem) : UIBaseBuilder(uiCoreSystem)
	{
		UITextComponent uiTextComponent;
		UITextMeshComponent uiTextMeshComponent;

		

		m_ecsEngine.addComponentToEntity<UITextComponent>(m_uiElement.getEntityId(), uiTextComponent);
		m_ecsEngine.addComponentToEntity<UITextMeshComponent>(m_uiElement.getEntityId(), uiTextMeshComponent);


		m_ecsEngine.processBufferedCommands();

	}


	UITextLabelBuilder& UITextLabelBuilder::withPosition(glm::vec3 position)
	{

		UIBaseBuilder::withPosition(position);
		return *this;

	}



	UITextLabelBuilder& UITextLabelBuilder::withRectDimensions(const float width, const float height)
	{

		UIBaseBuilder::withRectDimensions(width, height);
		return *this;
	}


	//Text specific methods
	UITextLabelBuilder& UITextLabelBuilder::withColour(glm::vec4 color)
	{
		UITextComponent* uiTextComponent = m_uiElement.getComponent<UITextComponent>();

		assert(uiTextComponent != nullptr && "UITextComponent not found in entity");

		uiTextComponent->color = color;

		return *this;


	}

	UITextLabelBuilder& UITextLabelBuilder::withFont(const std::string& fontName)
	{
		UITextComponent* uiTextComponent = m_uiElement.getComponent<UITextComponent>();

		assert(uiTextComponent != nullptr && "UITextComponent not found in entity");
		uiTextComponent->fontName = fontName;

		return *this;
	}



	UITextLabelBuilder& UITextLabelBuilder::withFontSize(const float fontSize)
	{
		UITextComponent* uiTextComponent = m_uiElement.getComponent<UITextComponent>();
		assert(uiTextComponent != nullptr && "UITextComponent not found in entity");
		uiTextComponent->fontSize = fontSize;
		return *this;
	}



	UITextLabelBuilder& UITextLabelBuilder::withText(const std::string& text)
	{
		UITextComponent* uiTextComponent = m_uiElement.getComponent<UITextComponent>();

		assert(uiTextComponent != nullptr && "UITextComponent not found in entity");

		uiTextComponent->setText(text);

		return *this;
	}



}