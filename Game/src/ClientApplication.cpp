#include <BlindEngine.h>

class ExampleLayer : public Blind::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		BLIND_CLIENT_INFO("ExampleLayer::Update");
	}
	void OnEvent(Blind::Event& e) override
	{
		BLIND_CLIENT_TRACE("{0}", e);
	}
};

class SandboxApp : public Blind::Application
{
public:
	SandboxApp()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Blind::ImGuiLayer());
	}
	~SandboxApp()
	{

	}
};

Blind::Application* Blind::CreateApplication()
{
	return new SandboxApp();
}
