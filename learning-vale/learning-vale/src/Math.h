#pragma once
#include <iostream>

namespace Game
{
	class Window
	{
	public:
		enum class WindowState
		{
			NONE = 0,
			Opened, Closed, Running
		};
		Window(uint16_t width, uint16_t height, const char* name)
		{
			this->name = name;
			this->width = width;
			this->height = height;
			this->currentState = WindowState::NONE;
		}
		void Print()
		{
			std::cout << name << ": (" << width << "x" << height << ")\n";
		}
		inline const char* GetName()	{ return this->name; }
		inline uint16_t GetWidth()		{ return this->width; }
		inline uint16_t GetHeight()		{ return this->height; }

		WindowState currentState;
	private:
		const char* name;
		uint16_t width;
		uint16_t height;
	};

	class Application
	{
	public:
		
		Application()
		{
			this->window = nullptr;
		}
		inline Window GetWindow() { return *this->window; }
		inline void SetWindow(Window window) { this->window = &window; }

		void Run()
		{
			this->window->currentState = Game::Window::WindowState::Running;
			while (true);
		}
		

	private:
		Window* window;
	};
}

namespace Math
{
	class Vector3
	{
	public:
		Vector3(float atrb1, float atrb2, float atrb3)
			:	r(atrb1), g(atrb2), b(atrb3) {}

		inline Vector3 operator+(Vector3 vec) { return Vector3(this->x += vec.x, this->y += vec.y, this->z += vec.z); }
		inline Vector3 operator-(Vector3 vec) { return Vector3(this->x -= vec.x, this->y -= vec.y, this->z -= vec.z); }
		
		union 
		{
			struct
			{
				float x, y, z;
			};
			struct
			{
				float r, g, b;
			};
			struct
			{
				float atrb1, atrb2, atrb3;
			};
		};
	};
	inline std::ostream& operator<<(std::ostream& out, Vector3 vec) { return out << vec.x << vec.y << vec.z; }
}