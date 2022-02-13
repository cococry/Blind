#pragma once
#include <Core.h>

namespace Blind
{
	class BLIND_API Application
	{
	/* \Blind Application Class
		This class is used to create a Application with Blind. It is implemented client side for now.
		It is DLL exported by Blind into the Client. The class will be created only by client side. */
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
	};
	Application* CreateApplication();
}