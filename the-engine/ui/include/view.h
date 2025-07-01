#pragma once

#include "iview.h"
#include <vector>
#include <string>

namespace TheEngine::UI
{
	class View :public IView
	{

	protected:
		std::vector<View> m_child;// child components
	public:



		// These can have default empty implementations here if appropriate for the base class
		// void add(View* view) override {};
	   //  void remove(View* view) override {};

		//implementation specific
		//void addChild(View* view); // More controlled addition
		//void removeChild(View* view); // More controlled removal



	};
}