#pragma once
#include "Core/Core.h"

namespace HEngine
{
	class HOPE_API Input
	{
	public:

		static bool IsKeyPressed(int KeyCode);
		inline static bool IsMouseButtonPressed(int button);
		inline static std::pair<float, float> GetMousePosition();
        inline static float GetMouseX();
        inline static float GetMouseY();


	};


}