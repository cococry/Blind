# Blind 

![](git-assets/images/blind-logo.png)

## About
Blind is an early state 2D and 3D Game Engine for creating interactive applications and currently with a 2D Batch Rendering implementation.
The Engine uses [**C++ 17**](https://en.wikipedia.org/wiki/C%2B%2B17) as its core language, but the plan is to implement [**C#**](https://de.wikipedia.org/wiki/C-Sharp)
Scripting for working in the engine. [**OpenGL**](https://de.wikipedia.org/wiki/OpenGL) is the currently in-use Rendering-API and  [**Premake 5**](https://premake.github.io/)
is used as the build system. The current state of the Application contains a flexible [OpenGL Data & Vertex Buffer Layout System](https://github.com/cococry/Blind/blob/main/Blind/src/Blind/Renderer/Buffer.h), an easy to use [Shader Library System](https://github.com/cococry/Blind/blob/main/Blind/src/Blind/Renderer/Shader.h), [Texture loading](https://github.com/cococry/Blind/blob/main/Blind/src/Platform/OpenGL/OpenGLTexture.cpp) an own currently BLOCKING [Event System](https://github.com/cococry/Blind/tree/main/Blind/src/Blind/Events). The Engine contains a fully working Rendering API (OpenGL/Vulkan...) agnostic Renderer API.
All of the System are being improved Day by Day and not even nearly finished. The engine is as stated, currently a work in progress.

***
## Usage

DISCLAIMER: Blind Engine is developed and distributed in [**Visual Studio 2022**](https://visualstudio.microsoft.com/de/vs/). Its recommended to use Visual Studio 2019 or higher for the best experience. Currently the only platform supported is Windows x64.

<ins>**1. Clone the repository:**</ins>

To load all data of this repository to your local disc, clone the repository with [**Git**](https://git-scm.com/downloads) with: `git clone --recursive https://github.com/cococry/Blind`

Now, when your local folder is filled with the repository files, execute [Win-GenProjects.bat](https://github.com/cococry/Blind/blob/main/Scripts/Win-GenProjects.bat) file in the 'Scripts' folder and open the [.sln](https://github.com/cococry/Blind/blob/main/Blind.sln) file with Visual Studio 2019/22.
Thats it. Your done with setting up the project and can start taking a deeper look into the engine code or try Blind.


