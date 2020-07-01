#pragma once

#include "Core.h"

namespace HEngine {

	class HOPE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}