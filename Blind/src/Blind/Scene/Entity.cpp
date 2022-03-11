#include <blindpch.h>
#include "Entity.h"

Blind::Entity::Entity(entt::entity handle, Scene* scene)
	:m_EntityHandle(handle), m_Scene(scene)
{

}

Blind::Entity::Entity(const Entity& other)
{

}
