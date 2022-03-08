#include <BlindEngine.h>
#include <Blind/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Blind
{
	class BMB : public Application
	{
	public:
		BMB()
			: Application("Blindman's Buff")
		{
			PushLayer(new EditorLayer());
		}
		~BMB()
		{

		}
	};

	Application* CreateApplication()
	{
		return new BMB();
	}
}

