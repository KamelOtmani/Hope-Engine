#pragma once

#include "Core.h"
#include "Core/Window.h"
#include "Events/Event.h"
#include "Events/AppEvent.h"
#include "Layers/LayerStack.h"
#include "Layers/ImGuiLayer.h"

#include "Renderer/Shader.h"

#include <Renderer/Core/Buffer.h>
#include <Renderer/Core/VertexArray.h>

namespace HEngine {

	class HOPE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		//std::unique_ptr<Scene> m_Window;
		// Old stuff
		// TODO : Remove all the stuff that is not needed for the app

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

        Ref<Shader> m_Shader;
        Ref<VertexArray> m_VertexArray;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
	
}