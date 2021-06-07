#include "hpch.h"
#include "Application.h"
#include "Log.h"
#include "Events/AppEvent.h"


#include "Renderer/Core/Renderer.h"

namespace HEngine {


	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		HENGINE_ASSERT(!s_Instance,"Application already exists , shouild be a singelton !")
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// TEMPORARY RENDERING TRIANGLE

		m_VertexArray.reset(VertexArray::Create());

		std::vector<FVertex> verts{
			{ Vec3{-0.5f, -0.5f, 0.0f},Vec4{0.8f, 0.2f, 0.8f, 1.0f} },
			{ Vec3{0.5f, -0.5f, 0.0f},Vec4{0.2f, 0.3f, 0.8f, 1.0f} },
			{ Vec3{0.0f, -0.0f, 0.0f},Vec4{0.8f, 0.8f, 0.2f, 1.0f} } };


		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f, 0.0f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		auto s1 = sizeof(vertices);
		auto s2 = sizeof(Vec3);
		auto s5 = sizeof(Vec4);
		auto s4 = sizeof(verts);
		auto s3 = verts.size();

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(verts, verts.size()*(sizeof(Vec3)+ sizeof(Vec4))));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		auto d1 = sizeof(indices);
		auto d2 = sizeof(uint32_t);
		auto d3 = verts.size();

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Color * 0.5 + 0.5);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		
	}

	void Application::Run()
	{
		while (m_Running)
		{

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

    void Application::Close()
    {
		m_Running = false;
    }

    void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}