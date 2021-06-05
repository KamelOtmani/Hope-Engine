#include "hpch.h"
#include "Buffer.h"


#include "Renderer.h"

#include "Platform/OpenGL/Renderer/GLBuffer.h"

namespace HEngine {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RHI::API::None:    HENGINE_ASSERT(false, "RHI::None is currently not supported!"); return nullptr;
		case RHI::API::OpenGL:  return new GLVertexBuffer(vertices, size);
		}

		HENGINE_ASSERT(false, "Unknown RHI!");
		return nullptr;
	}

	VertexBuffer* VertexBuffer::Create(std::vector<FVertex>& vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RHI::API::None:    HENGINE_ASSERT(false, "RHI::None is currently not supported!"); return nullptr;
		case RHI::API::OpenGL:  return new GLVertexBuffer(vertices, size);
		}

		HENGINE_ASSERT(false, "Unknown RHI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RHI::API::None:    HENGINE_ASSERT(false, "RHI::None is currently not supported!"); return nullptr;
		case RHI::API::OpenGL:  return new GLIndexBuffer(indices, size);
		}

		HENGINE_ASSERT(false, "Unknown RHI!");
		return nullptr;
	}

}