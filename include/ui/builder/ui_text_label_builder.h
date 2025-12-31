#pragma once
#include "ui_base_builder.h"
#include <glm/glm.hpp>

namespace TheEngine::UI
{

	class UITextLabelBuilder : public UIBaseBuilder
	{

	private:


	public:

		UITextLabelBuilder(UICoreSystem& uiCoreSystem);


		UITextLabelBuilder& withPosition(glm::vec3 position);
		UITextLabelBuilder& withRectDimensions(const float width, const float height);


		//Text specific methods
		UITextLabelBuilder& withColour(glm::vec4 color);
		UITextLabelBuilder& withFont(const std::string& fontName);
		UITextLabelBuilder& withFontSize(const float fontSize);
		UITextLabelBuilder& withText(const std::string& text);

	};



}