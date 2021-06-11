#include "hpch.h"
#include "Serializer.h"

#include "ECS/Entity.h"
#include "ECS/Components.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

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
namespace HEngine {

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

    Serializer::Serializer(const Ref<Scene>& scene)
        : m_Scene(scene)
    {
    }

    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here

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

            auto& cam = entity.GetComponent<CameraComponent>();

            out << YAML::Key << "FOV" << YAML::Value << cam.fov;
            out << YAML::Key << "AspectRatio" << YAML::Value << cam.aspectRatio;
            out << YAML::Key << "FarPlane" << YAML::Value << cam.farPlane;
            out << YAML::Key << "NearPlane" << YAML::Value << cam.nearPlane;
            out << YAML::Key << "Projection" << YAML::Value << (int)cam.projType;
            out << YAML::Key << "OrthographicFarPlane" << YAML::Value << cam.orthographicFarPlane;
            out << YAML::Key << "OrthographicNearPlane" << YAML::Value << cam.orthographicNearPlane;
            out << YAML::Key << "OrthographicSize" << YAML::Value << cam.orthographicSize;
            out << YAML::Key << "PrimaryCamera" << YAML::Value << cam.bPrimary;

            out << YAML::EndMap; // CameraComponent
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap; // SpriteRendererComponent

            auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.m_Color;

            out << YAML::EndMap; // SpriteRendererComponent
        }

        if (entity.HasComponent<MeshRendererComponent>())
        {
            out << YAML::Key << "MeshRendererComponenet";
            out << YAML::BeginMap; // MeshRendererComponenet

            auto& mesh = entity.GetComponent<MeshRendererComponent>();
            out << YAML::Key << "Path" << YAML::Value << mesh.path;

            out << YAML::EndMap; // MeshRendererComponenet
        }


        out << YAML::EndMap; // Entity
    }

    void Serializer::Serialize(const std::string& filepath)
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

        std::ofstream fout(filepath);
        fout << out.c_str();
    }


    bool Serializer::Deserialize(const std::string& filepath)
    {
        std::ifstream stream(filepath);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Scene"])
            return false;

        std::string sceneName = data["Scene"].as<std::string>();
        m_Scene->m_SceneName = sceneName;
        HEngineLOG("Deserializing scene '{0}'", sceneName);

        auto entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent)
                    name = tagComponent["Tag"].as<std::string>();

                HEngineLOG("Deserialized entity with ID = {0}, name = {1}", uuid, name);

                Entity deserializedEntity = m_Scene->CreateEntity(name);

                auto transformComponent = entity["TransformComponent"];
                if (transformComponent)
                {
                    // Entities always have transforms
                    auto& tc = deserializedEntity.GetComponent<TransformComponent>();
                    tc.Position = transformComponent["Position"].as<Vec3>();
                    tc.Rotation = transformComponent["Rotation"].as<Vec3>();
                    tc.Scale = transformComponent["Scale"].as<Vec3>();
                }

                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent)
                {
                    auto& cc = deserializedEntity.AddComponent<CameraComponent>();
                    cc.fov = cameraComponent["FOV"].as<float>();
                    cc.aspectRatio = cameraComponent["AspectRatio"].as<float>();
                    cc.farPlane = cameraComponent["FarPlane"].as<float>();
                    cc.nearPlane = cameraComponent["NearPlane"].as<float>();
                    cc.projType = (CameraProjectionType)cameraComponent["Projection"].as<int>();
                    cc.orthographicFarPlane = cameraComponent["OrthographicFarPlane"].as<float>();
                    cc.orthographicNearPlane = cameraComponent["OrthographicNearPlane"].as<float>();
                    cc.orthographicSize = cameraComponent["OrthographicSize"].as<float>();
                    cc.bPrimary = cameraComponent["PrimaryCamera"].as<bool>();
                    
                }

                auto spriteRendererComponent = entity["SpriteRendererComponent"];
                if (spriteRendererComponent)
                {
                    auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
                    src.m_Color = spriteRendererComponent["Color"].as<Vec4>();
                }

                auto meshRendererComponent = entity["MeshRendererComponent"];
                if (spriteRendererComponent)
                {
                    auto& src = deserializedEntity.AddComponent<MeshRendererComponent>();
                    src.path = meshRendererComponent["Path"].as<std::string>();
                    if (!src.path.empty())
                        src.UpdateMesh(src.path);
                }


            }
        }

        return true;
    }


}
