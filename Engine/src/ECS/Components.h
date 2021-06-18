#pragma once

#include "HTypes.h"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <Renderer\Commun.h>
#include <Renderer\Shader.h>
#include "Renderer/Core/VertexArray.h"

#include "Utility/AssetImporter.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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

        Mat4 Matrix() {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Position) * rotation *
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
            vertexBuffer.reset(VertexBuffer::Create(vertices, static_cast<uint32_t>(vertices.size() * sizeof(Vec3)+ sizeof(Vec3) + sizeof(Vec2)+ sizeof(Vec4))));
            BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float3, "a_Normal" },
                { ShaderDataType::Float2, "a_TexCoord" },
                { ShaderDataType::Float4, "a_Color" }
            };
            vertexBuffer->SetLayout(layout);
            vertexArray->AddVertexBuffer(vertexBuffer);

            Ref<IndexBuffer> indexBuffer;
            indexBuffer.reset(IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size())));
            vertexArray->SetIndexBuffer(indexBuffer);
        }
        ~MeshRendererComponent() = default;

        void UpdateMesh()
        {
            auto& res = AssetImporter::ImportModel(path);
            _data = res.verts;
            _indices = res.indices;

            vertexArray.reset(VertexArray::Create());
            Ref<VertexBuffer> vertexBuffer;
            vertexBuffer.reset(VertexBuffer::Create(_data, _data.size() * static_cast<uint32_t>(sizeof(Vec3) + sizeof(Vec3) + sizeof(Vec2) + sizeof(Vec4))));
            BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float3, "a_Normal" },
                { ShaderDataType::Float2, "a_TexCoord" },
                { ShaderDataType::Float4, "a_Color" }
            };
            vertexBuffer->SetLayout(layout);
            vertexArray->AddVertexBuffer(vertexBuffer);

            Ref<IndexBuffer> indexBuffer;
            indexBuffer.reset(IndexBuffer::Create(_indices.data(), static_cast<uint32_t>(_indices.size())));
            vertexArray->SetIndexBuffer(indexBuffer);
            bEmpty = false;
        };

        std::string path = std::string();
        Ref<VertexArray> vertexArray;
        Ref<Material> material;
        Ref<VertexArray>& getVertexArray() { return vertexArray; };

        bool bSouldRender = true;
        bool bEmpty = true;
    private:
        std::vector<FVertex> _data;
        std::vector<uint32_t> _indices;
    };

    struct SpriteRendererComponent
    {
        Vec4 m_Color = Vec4{ 1.0f };
        Shader* m_Shader = nullptr;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent& other) = default;
        SpriteRendererComponent(const Vec4& Color)
            : m_Color(Color) {}
        ~SpriteRendererComponent() = default;

    };

    enum class CameraProjectionType { Perspective = 0, Orthographic = 1 };

    struct CameraComponent
    {
        CameraComponent() = default;

        CameraProjectionType projType = CameraProjectionType::Perspective;
    	float fov = 90.f;
    	float nearPlane = 0.1f;
    	float farPlane = 1000.0f;
    	float aspectRatio = 16.0f / 9.0f;

        float orthographicSize = 10.f;
        float orthographicNearPlane = -1.0f;
        float orthographicFarPlane = 1.0f;

        bool bPrimary = false;

    	Mat4 Projection()
    	{
            switch (projType)
            {
            case HEngine::CameraProjectionType::Perspective:
                return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
                break;
            case HEngine::CameraProjectionType::Orthographic:
            {
                float orthoLeft = -orthographicSize * aspectRatio * 0.5f;
                float orthoRight = orthographicSize * aspectRatio * 0.5f;
                float orthoBottom = -orthographicSize * 0.5f;
                float orthoTop = orthographicSize * 0.5f;
                return glm::ortho(orthoLeft, orthoRight,
                    orthoBottom, orthoTop, orthographicNearPlane, orthographicFarPlane);
                break;
            }
            default:
                HASSERT(false,"SOMETHING WENT WRONG WHEN SETTING CAMERA PROJECTION ! (LINE 125 Component.h)");
                return Mat4(1.0f);
                break;
            }
    		//return glm::ortho(0.0f, hight, 0.0f, width, nearPlane, farPlane);
    	}
    };

    struct PointLightComponent
    {
        Vec4 m_Color = Vec4{ 1.0f };
        float m_Intensity = 1.0f;
        bool bAffectWorld = true;

        PointLightComponent() = default;
        PointLightComponent(const PointLightComponent& other) = default;
        ~PointLightComponent() = default;

    };
}