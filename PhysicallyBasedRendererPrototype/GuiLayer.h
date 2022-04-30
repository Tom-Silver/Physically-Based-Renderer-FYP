#pragma once

// External includes
#include <list>
#include <string>
#include <vector>

namespace TSFYP
{
	class Renderer;
	struct Scene;
	struct Window;

	struct IGuiObject
	{
		IGuiObject()
			: close(false)
		{}
		virtual ~IGuiObject() {}

		virtual void CreateGui() = 0;

		bool close;
	};

	class GuiLayer
	{
	public:
		GuiLayer(Renderer* renderer, Scene* scene, Window* window);

		void CreateGui();

	private:
		// Stores pointers to gui objects for dynamic adding/removing of windows
		std::vector<IGuiObject*> mGuiObjects;

		std::list<std::string> mRecentMaterials; // Stores recently loaded materials for quick access
		std::list<std::string> mRecentEnvironments; // Stores recently loaded environments for quick access

		// Pointers for access to other areas of the application
		Renderer* pRenderer;
		Window* pWindow;
		Scene* pScene;
	};
}