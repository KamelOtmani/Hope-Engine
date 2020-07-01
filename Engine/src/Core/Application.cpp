#include "PCH/hpch.h"
#include "Application.h"
#include "Log.h"
#include "Events/AppEvent.h"

#include <iostream>

HEngine::Application::Application()
{
}

HEngine::Application::~Application()
{
}

void HEngine::Application::Run()
{
	WindowResizeEvent e(1280, 720);
	if (e.IsInCategory(EventCategoryApplication))
	{
		HLOG(e);
	}
	if (e.IsInCategory(EventCategoryInput))
	{
		HLOG(e);
	}

	while (true)
	{
		// Loop
	}
}
