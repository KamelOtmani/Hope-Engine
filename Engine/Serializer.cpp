#include "hpch.h"
#include "Serializer.h"

#include <fstream>

#include "HTypes.h"
#include <yaml-cpp/yaml.h>

#include "ECS/Entity.h"
#include "ECS/Components.h"

namespace YAML {

    template<>
    struct convert<Vec3>
    {
        static Node encode(const Vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, Vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Vec4>
    {
        static Node encode(const Vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, Vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

}

namespace HEngine
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const Vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const Vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }


    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        : m_Scene(scene)
    {

    }
    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;

        if (entity.HasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap; // TagComponent

            auto& tag = entity.GetComponent<TagComponent>().tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap; // TagComponent
        }

        if (entity.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap; // TransformComponent

            auto& tc = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Position" << YAML::Value << tc.Position;
            out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

            out << YAML::EndMap; // TransformComponent
        }

        if (entity.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap; // CameraComponent

            auto& camera = entity.GetComponent<CameraComponent>();

            out << YAML::Key << "FOV" << YAML::Value << camera.fov;
            out << YAML::Key << "NearPlane" << YAML::Value << camera.nearPlane;
            out << YAML::Key << "FarPlane" << YAML::Value << camera.farPlane;
            out << YAML::Key << "AspectRatio" << YAML::Value << camera.aspectRatio;
            out << YAML::Key << "orthographicSize" << YAML::Value << camera.orthographicSize;
            out << YAML::Key << "orthographicNearPlane" << YAML::Value << camera.orthographicNearPlane;
            out << YAML::Key << "orthographicFarPlane" << YAML::Value << camera.orthographicFarPlane;
            out << YAML::Key << "PrymaryCamera" << YAML::Value << camera.bPrimary;

            out << YAML::EndMap; // CameraComponent
        }
        
        if (entity.HasComponent<SpriteRenderingComponent>())
        {
            out << YAML::Key << "SpriteRenderingComponent";
            out << YAML::BeginMap; // CameraComponent
            out << YAML::Key << "Color" << YAML::Value << entity.GetComponent<SpriteRenderingComponent>().m_Color;
            out << YAML::EndMap; // CameraComponent
        }
    }

    void SceneSerializer::Serialize(const std::string& path)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_SceneName;
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        m_Scene->m_Registry.each([&](auto entityID)
            {
                Entity entity = { entityID, m_Scene.get() };
                if (!entity)
                    return;

                SerializeEntity(out, entity);
            });
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(path);
        fout << out.c_str();
    }

    bool SceneSerializer::Deserialize(const std::string& path)
    {
        return false;
    }

}