#include "hpch.h"
#include "GLContext.h"
namespace HEngine {
	GLContext::GLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		//HEngineASSERT(windowHandle, "Window handle is null!")
	}

	void GLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HENGINE_ASSERT(status, "Failed to initialize Glad!");

		HEngineINFO("OpenGL Info:");
		HEngineINFO("  Vendor: {0}", glGetString(GL_VENDOR));
		HEngineINFO("  Renderer: {0}", glGetString(GL_RENDERER));
		HEngineINFO("  Version: {0}", glGetString(GL_VERSION));
		
	}

	void GLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}