#pragma once

// External includes
#include <list>
#include <string>
#include <vector>

namespace TSFYP
{
	class Renderer;
	class Scene;

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
		GuiLayer(Renderer* renderer, Scene* scene);

		void CreateGui();

	private:
		// Stores pointers to gui objects for dynamic adding/removing of windows
		std::vector<IGuiObject*> mGuiObjects;

		std::list<std::string> mRecentMaterials; // Stores last 3 loaded materials

		// Pointers for calling CreateGui()
		Renderer* pRenderer;
		Scene* pScene;
	};
}