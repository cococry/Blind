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

		Ref<Texture2D> GetFolderIcon() const { return m_FolderIcon; }
		void SetFolderIcon(const Ref<Texture2D>& icon) { m_FolderIcon = icon; }

		Ref<Texture2D> GetFileIcon() const { return m_FileIcon; }
		void SetFileIcon(const Ref<Texture2D>& icon) { m_FileIcon = icon; }
	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
	};
}