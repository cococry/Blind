#include <BlindEngine.h>

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
