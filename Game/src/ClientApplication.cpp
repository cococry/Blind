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
		if (Blind::Input::IsKeyPressed(BL_KEY_TAB))
			BLIND_CLIENT_INFO("Tab key is pressed! ");
	}
};

class SandboxApp : public Blind::Application
{
public:
	SandboxApp()
	{
	}
	~SandboxApp()
	{

	}
};

Blind::Application* Blind::CreateApplication()
{
	return new SandboxApp();
}
