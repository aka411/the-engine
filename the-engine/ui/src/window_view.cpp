
#include "../include/window_view.h"

using namespace TheEngine::UI;

void WindowView::add(View* view) {

	m_child.push_back(view);// add the child component to the vector

}

void WindowView::remove(View* view)
{

}