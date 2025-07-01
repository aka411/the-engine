#pragma once

#include "view.h" 

#include <string>
#include <vector>

namespace TheEngine::UI
{

	class WindowView : public View
	{
	public:
		WindowView(std::string uName);
		virtual ~WindowView();

		// Override the add method to allow adding children
		void add(View* view) override;

		// Override the remove method to allow removing children
		void remove(View* view) override;

		// WindowView might have its own specific behavior, e.g., rendering
		void render();

		// The getChildName method is inherited from View and will use the base class's logic

	private:
		// The child vector is inherited from the Component base class
	};

}
