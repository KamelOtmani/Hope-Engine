#include "hpch.h"
#include "Entity.h"

#include <Renderer\Core\VertexArray.h>

namespace HEngine {



    void Model::updateMesh()
    {
		vao.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(mesh._data, mesh._data.size() * (sizeof(Vec3) + sizeof(Vec4))));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		vao->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(mesh._indices.data(), mesh._indices.size()));
		vao->SetIndexBuffer(indexBuffer);
    }

}