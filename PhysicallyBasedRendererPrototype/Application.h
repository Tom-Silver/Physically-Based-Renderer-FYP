#pragma once

namespace TSFYP
{
	class Renderer;
	class Scene;

	// Main class that stores the subsystems of the project
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

	private:
		Renderer* mRenderer;
		Scene* mScene;
	};
}