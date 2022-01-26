#include "Application.h"

// Internal includes
#include "Renderer.h"
#include "Sphere.h"
#include "SceneObject.h"

#define IMAGE_WIDTH 1920
#define IMAGE_HEIGHT 1080

namespace TSFYP
{
	Application::Application()
		: mRenderer(new Renderer())
		, mScene(nullptr)
	{
		SceneObject* object = new SceneObject();
		object->shape = new Sphere(2.0f);
		object->transform = new Transform(Vector3(0.0f, 0.0f, -10.0f), Vector3(), Vector3());
		object->colour = Colour::Cyan;

		std::vector<SceneObject> objects;
		objects.push_back(*object);

		Light* light = new Light(Point3(10.0f, 5.0f, 0.0f), Colour::White);

		std::vector<Light> lights;
		lights.push_back(*light);

		mScene = new Scene(objects, lights);
	}

	Application::~Application()
	{
		delete mRenderer;
		mRenderer = nullptr;

		delete mScene;
		mScene = nullptr;
	}

	void Application::Run()
	{
		mRenderer->Render(*mScene, 1920, 1080);
	}
}