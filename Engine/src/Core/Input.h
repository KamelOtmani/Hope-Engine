#pragma once
#include "Core/Core.h"
#include "Events/KeyCodes.h"

namespace HEngine {
class Input {
public:
    static bool IsKeyPressed(const KeyCode code);
    static bool IsMouseButtonPressed(const MouseCode code);
    static std::pair<float, float> GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};

}
