#pragma once

#include "Scene.h"
#include <Blind/Core/Log.h>
#include "Components.h"
#include "Blind/Core/UUID.h"

#include <entt.hpp>

namespace Blind
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			BLIND_ENGINE_ASSERT(!HasComponent<T>(), "Entity already has this component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			BLIND_ENGINE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}
		
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			BLIND_ENGINE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const uint32_t GetIntID() const { return (uint32_t)m_EntityHandle; }

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }
		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_EntityHandle = { entt::null };
		Scene* m_Scene = nullptr; 
	};

}