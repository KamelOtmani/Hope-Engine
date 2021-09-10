#pragma once
#include "Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
struct GLFWwindow;

namespace HEngine {

class GLContext : public GraphicsContext {
public:
    GLContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle;
};

}
