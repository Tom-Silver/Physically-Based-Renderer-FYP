#pragma once

// Internal includes
#include "DeltaTimeClock.h"

struct ImGuiContext;
struct ImGuiIO;

namespace TSFYP
{
	class Renderer;
	struct Scene;
	struct Window;

	// Main class that stores the subsystems of the project
	class Application
	{
	public:
		Application();
		~Application();

		bool Initialise();
		void Run();

	private:
		void CreateGui();
		void HandleInput(float deltaTime);

		bool InitialiseGL();
		bool InitialiseImGui();
		bool InitialiseScene();

		Window* mWindow;
		Renderer* mRenderer;
		Scene* mScene;
		DeltaTimeClock mDeltaTimeClock;
		bool mQuit;

		float mLastMouseX, mLastMouseY;
		float mLastScrollY;

		ImGuiContext* mGuiContext;
		ImGuiIO* mGuiIO;
	};
}