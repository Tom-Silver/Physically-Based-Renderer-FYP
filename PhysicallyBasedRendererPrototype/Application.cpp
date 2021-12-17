#include "Application.h"

// Internal includes
#include "Renderer.h"

namespace TSFYP
{
	Application::Application()
		: renderer(new Renderer())
		, scene(new Scene())
	{}

	Application::~Application()
	{}

	void Application::Run()
	{
		renderer->Render(*scene, 1920, 1080);
	}
}