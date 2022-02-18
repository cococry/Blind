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
	}
	void OnEvent(Blind::Event& e) override
	{
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
