#pragma once

#include "HTypes.h"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <Renderer\Commun.h>
#include <Renderer\Shader.h>
#include "Renderer/Core/VertexArray.h"

namespace HEngine
{
    struct TagComponent
    {
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& name)
            : tag(name) {};

        std::string tag;
    };

    struct TransformComponent
    {
        Vec3 Position{ 0.0f };
        Vec3 Rotation{ 0.0f };
        Vec3 Scale{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const Vec3& pos, const Vec3& rot, const Vec3& scale)
            : Position(pos), Rotation(rot), Scale(scale) {}

        Mat4 Matrix () {
            return glm::translate(glm::mat4(1.0f), Position) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(1.0f, 0.0f, 1.0f)) *
                glm::scale(glm::mat4(1.0f), Scale);
        }

        operator Mat4&() {
            return glm::translate(glm::mat4(1.0f), Position) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(1.0f, 0.0f, 1.0f)) *
                glm::scale(glm::mat4(1.0f), Scale);
        }
        operator const Mat4& () const
        {
            return glm::translate(glm::mat4(1.0f), Position) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(1.0f, 0.0f, 1.0f)) *
                glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct MeshRendererComponent
    {
        MeshRendererComponent() = default;
        MeshRendererComponent(const std::vector<FVertex>& vertices, std::vector<uint32_t>& indices)
        {
            _data = vertices;
            _indices = indices;
            vertexArray.reset(VertexArray::Create());
            Ref<VertexBuffer> vertexBuffer;
            vertexBuffer.reset(VertexBuffer::Create(vertices, vertices.size() * (sizeof(Vec3) + sizeof(Vec4))));
            BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" }
            };
            vertexBuffer->SetLayout(layout);
            vertexArray->AddVertexBuffer(vertexBuffer);

            Ref<IndexBuffer> indexBuffer;
            indexBuffer.reset(IndexBuffer::Create(indices.data(), indices.size()));
            vertexArray->SetIndexBuffer(indexBuffer);
        }
        ~MeshRendererComponent() = default;
        Ref<VertexArray> vertexArray;
        Shader* shader = nullptr;
        Ref<VertexArray>& getVertexArray() { return vertexArray; };
    private:
        std::vector<FVertex> _data;
        std::vector<uint32_t> _indices;
    };

    struct CameraComponent
    {
        CameraComponent() = default;

    	float fov = 90.f;
    	float nearPlane = 0.1f;
    	float farPlane = 10.0f;
    	float hight = 1600.0f;
    	float width = 900.0f;
    	float aspectRatio = 16.0f / 9.0f;
        bool bPrimary = false;

    	Mat4 Projection()
    	{
    		return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    		//return glm::ortho(0.0f, hight, 0.0f, width, nearPlane, farPlane);
    	}
    };
}