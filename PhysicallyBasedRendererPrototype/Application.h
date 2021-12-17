#pragma once

namespace TSFYP
{
	class Renderer;
	class Scene;

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

	private:
		Renderer* renderer;
		Scene* scene;
	};
}