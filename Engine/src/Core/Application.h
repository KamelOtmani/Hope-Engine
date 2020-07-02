#pragma once

#include "Core.h"
#include <Core\Window.h>

namespace HEngine {

	class HOPE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
	
}