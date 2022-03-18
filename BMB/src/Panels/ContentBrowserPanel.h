#pragma once
#include <filesystem>
#include <Blind/Renderer/Texture.h>

namespace Blind
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiDraw();
	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
	};
}