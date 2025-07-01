#pragma once
namespace TheEngine::UI
{
	class  View;

	class IView
	{

	private:

	public:

		virtual ~IView() = default; // Important for polymorphism

		virtual void add(View* view) = 0;
		virtual void remove(View* view) = 0;

	protected:
		IView() = default;
	};
}