#pragma once
#include "Window.h"

class  App
{
	public:
		App();
		int Start();
		~App();
	private:
		void GenFrame();
	private:
		Window wnd;
		std::vector<std::unique_ptr<class Box>> boxes;
};