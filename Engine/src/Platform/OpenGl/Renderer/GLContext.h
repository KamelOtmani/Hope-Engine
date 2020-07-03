#pragma once
#include "Renderer/GraphicsContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
struct GLFWwindow;

namespace HEngine {

    class GLContext :  public GraphicsContext
    {
    public:
        GLContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* m_WindowHandle;
    };

}