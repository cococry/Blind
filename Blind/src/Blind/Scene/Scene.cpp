#include "blindpch.h"
#include "Scene.h"

#include <Blind/Renderer/Renderer2D.h>
#include "Components.h"
#include "ScriptableEntity.h"
#include <glm/glm.hpp>
#include "Entity.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Blind
{
	static b2BodyType InternalRigidBodyTypeToBox2d(RigidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case RigidBody2DComponent::BodyType::Static: return b2_staticBody;
			case RigidBody2DComponent::BodyType::Dynamic: return b2_dynamicBody; 
			case RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody; 
		}
		BLIND_ENGINE_ASSERT(false, "Unknown rigidbody type!");
		return b2_staticBody;
	}
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	template<typename Comp>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<Comp>();
		for (auto e : view)
		{
			UUID uuid = src.get<IDComponent>(e).ID;
			entt::entity dstEnttID = enttMap.at(uuid);
			auto& component = src.get<Comp>(e);
			dst.emplace_or_replace<Comp>(dstEnttID, component);
		}
	}
	template<typename Comp>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Comp>())
			dst.AddOrReplaceComponent<Comp>(src.GetComponent<Comp>());
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;
	
		auto& srcEnttRegistry = other->m_Registry;
		auto& destEnttRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> uuidToEnttMap;

		auto entityIDs = srcEnttRegistry.view<IDComponent>();
		for (auto e : entityIDs)
		{
			UUID uuid = srcEnttRegistry.get<IDComponent>(e).ID;
			const auto& name = srcEnttRegistry.get<TagComponent>(e).Tag;
			newScene->CreateEntityWithUUID(uuid, name);
			uuidToEnttMap[uuid] = e;
		}

		CopyComponent<TransformComponent>(destEnttRegistry, srcEnttRegistry, uuidToEnttMap);
		CopyComponent<SpriteRendererComponent>(destEnttRegistry, srcEnttRegistry, uuidToEnttMap);
		CopyComponent<CameraComponent>(destEnttRegistry, srcEnttRegistry, uuidToEnttMap);
		CopyComponent<RigidBody2DComponent>(destEnttRegistry, srcEnttRegistry, uuidToEnttMap);
		CopyComponent<BoxCollider2DComponent>(destEnttRegistry, srcEnttRegistry, uuidToEnttMap);
		CopyComponent<NativeScriptComponent>(destEnttRegistry, srcEnttRegistry, uuidToEnttMap);

		return newScene;
	}
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::OnRuntimeStart()
	{
		m_Box2DWorld = new b2World({ 0.0f, -9.81f });
		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& tc = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = InternalRigidBodyTypeToBox2d(rb2d.Type);
			bodyDef.position.Set(tc.Translation.x, tc.Translation.y);
			bodyDef.angle = tc.Rotation.z;

			b2Body* body = m_Box2DWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * tc.Scale.x, bc2d.Size.y * tc.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;

				body->CreateFixture(&fixtureDef);
			}
		}
	}
	void Scene::OnRuntimeStop()
	{
		delete m_Box2DWorld;
		m_Box2DWorld = nullptr;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);

	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();

		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<RigidBody2DComponent>(newEntity, entity);
		CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
		CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Scripting
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{

					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}
		// Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_Box2DWorld->Step(ts, velocityIterations, positionIterations);

			auto view = m_Registry.view<RigidBody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& tc = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				tc.Translation.x = position.x;
				tc.Translation.y = position.y;
				tc.Rotation.z = body->GetAngle();
			}
		}
		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					if(!m_Registry.has<MeshComponent>(entity))
					{
						auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

						Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
					}
				}
			}
			{
				auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					if (!m_Registry.has<MeshComponent>(entity))
					{
						auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

						Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
					}
				}
			}
			Renderer2D::EndScene();
		}

	}
	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				if (!m_Registry.has<MeshComponent>(entity))
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}
			}
		}
		{
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				if (!m_Registry.has<MeshComponent>(entity))
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}
			}
		}
		
		Renderer2D::EndScene();
	}
	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}
	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}
	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component) {}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}
	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}
	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component) {}
	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {}
	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {}
	template<>
	void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component) {}
}