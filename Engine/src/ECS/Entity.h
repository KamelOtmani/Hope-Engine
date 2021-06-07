#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace HEngine {

	class Entity
	{
	public:
        Entity() = default;
        Entity(const Entity & other) = default;
		Entity(entt::entity id, Scene* scn);

        template<typename T>
        bool HasComponent()
        {
            return scene->m_Registry.any_of<T>(m_id);
        }

        template<typename T>
        T& GetComponent()
        {
            //HASSERT(HasComponent<T>(), "Enttity does not have this component");
            return scene->m_Registry.get<T>(m_id);
        }

        template<typename T>
        void RemoveComponent()
        {
            //HASSERT(!HasComponent<T>(), "Entity already has component!");
            return scene->m_Registry.remove<T>(m_id);
        }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            //HASSERT(!HasComponent<T>(), "Enttity does not have this component");
            return scene->m_Registry.emplace<T>(m_id, std::forward<Args>(args)...);
        }
		
        operator uint32_t() const { return (uint32_t)m_id; }
        operator entt::entity() const { return m_id; }

        operator bool() const { return m_id != entt::null; }

        bool operator==(const Entity& other) const
        {
            return m_id == other.m_id && scene == other.scene;
        }

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity m_id = entt::null;
		Scene* scene = nullptr;
	};

	//struct CameraComponent : Component
	//{
	//	float fov = 90.f;
	//	float nearPlane = 0.1f;
	//	float farPlane = 10.0f;
	//	float hight = 1600.0f;
	//	float width = 900.0f;
	//	float aspectRatio = 16.0f / 9.0f;

	//	Mat4 Projection()
	//	{
	//		return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	//		//return glm::ortho(0.0f, hight, 0.0f, width, nearPlane, farPlane);
	//	}
	//};

	
	//class Camera
	//{
	//public:
	//	Camera() { transform.Position = Vec3(0.0f, 0.0f, 3.0f); };
	//	virtual ~Camera() {};

	//	TransformComponent transform;
	//	CameraComponent cam;

	//	Mat4 ViewProjectionMatrix()
	//	{
	//		return cam.Projection() * glm::inverse(transform.ModelMatrix());
	//	}
	//};

}