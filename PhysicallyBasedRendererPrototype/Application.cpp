#include "Application.h"

// External includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_glfw.h>

// Internal includes
#include "Cube.h"
#include "EnvironmentLoader.h"
#include "GuiLayer.h"
#include "MaterialLoader.h"
#include "Renderer.h"
#include "ShaderCompiler.h"
#include "Sphere.h"
#include "Scene.h"
#include "Texture2D.h"
#include "Window.h"

// Preprocessor definitions
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "PBR Material Viewer"

#define FRAME_CAP 60
#define TARGET_DELTA_TIME 1.0f / (float)FRAME_CAP

#define PI 3.141592653589793f
#define ORBIT_SPEED 40.0f
#define Y_MARGIN 2.9f

#define MOVE_SPEED 1000.0f
#define ZOOM_SPEED 10.0f

#define MIN_ZOOM -0.1f
#define MAX_ZOOM 5.5f

namespace TSFYP
{
	// Other prototypes
	bool InitialiseGLFW(GLFWwindow** windowPtr);
	bool InitialiseGlad();

	// GLFW callbacks
	void GLFWErrorCallback(int error, const char* description);
	void GLFWWindowCloseCallback(GLFWwindow* window);

	Application::Application()
		: mRenderer(nullptr)
		, mScene(nullptr)
		, mWindow(nullptr)
		, mQuit(false)
		, mLastMouseX(0.0f)
		, mLastMouseY(0.0f)
		, mGuiContext(nullptr)
		, mGuiIO(nullptr)
		, mGuiLayer(nullptr)
		, mCurrentZoom(1.0f)
		, mShowGUI(true)
	{}

	Application::~Application()
	{
		delete mRenderer;
		mRenderer = nullptr;

		delete mScene;
		mScene = nullptr;

		glfwSetWindowShouldClose(mWindow->glfwWindow, GLFW_TRUE);
		glfwTerminate();
		mWindow->glfwWindow = nullptr;
	}

	void Application::Run()
	{
		float deltaTime = 0.0f;

		while (!mQuit)
		{
			while (deltaTime < TARGET_DELTA_TIME)
			{
				deltaTime += mDeltaTimeClock.GetDeltaTime();
			}

			mDeltaTimeClock.Update();

			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (mShowGUI)
			{
				mGuiLayer->CreateGui();
			}

			HandleInput(deltaTime);

			mScene->Update(deltaTime);

			mRenderer->Render();

			deltaTime = 0.0f;
		}
	}

	void Application::HandleInput(float deltaTime)
	{
		ImVec2 mousePos = ImGui::GetMousePos();

		// Press escape to toggle GUI
		if (ImGui::IsKeyPressed(ImGuiKey_Escape))
		{
			mShowGUI = !mShowGUI;
		}

		// Arcball camera rotation
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left)/* && !ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()*/)
		{
			Camera& camera = mScene->mCamera;

			glm::vec4 camPos(camera.pos(), 1.0f);
			glm::vec4 camPivot(camera.at(), 1.0f);

			// Calculate rotation amount based on mouse movement
			float deltaX = ORBIT_SPEED * (2 * PI / mWindow->width) * deltaTime; // Left to right = 360 degrees (2 * PI)
			float deltaY = ORBIT_SPEED * PI / mWindow->height * deltaTime; // Top to bottom = 180 degrees (PI)

			float xAngle = (mLastMouseX - mousePos.x) * deltaX;
			float yAngle = (mLastMouseY - mousePos.y) * deltaY;
			yAngle = -yAngle; // Invert

			// Handle edge case (camera dir == camera up)
			float cosAngle = glm::dot(camera.forward(), camera.up()); // Is it right to use camera up or should it be constant (0, 1, 0)?
			if (cosAngle * glm::sign(deltaY) > 0.99f)
			{
				deltaY = 0.0f;
			}

			// Rotate camera around pivot on first axis (yaw)
			glm::mat4 rotationMatrixX(1.0f);
			rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, camera.up());
			camPos = (rotationMatrixX * (camPos - camPivot)) + camPivot;

			// Rotate camera around pivot on second axis (pitch)
			glm::mat4x4 rotationMatrixY(1.0f);
			glm::vec3 right = camera.right();
			rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, right);
			glm::vec3 finalPos;
			if ((yAngle >= 0.0f && camPos.y < Y_MARGIN) || ((yAngle < 0.0f) && camPos.y / mCurrentZoom > -Y_MARGIN / mCurrentZoom))
			{
				finalPos = (rotationMatrixY * (camPos - camPivot)) + camPivot;
			}
			else
			{
				finalPos = camPos;
			}

			// Update camera
			camera.SetPos(finalPos);
		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Right)/* && !ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()*/)
		{
			SceneObject& object = mScene->mObject;
			
			// Calculate rotation amount based on mouse movement
			float deltaX = MOVE_SPEED * (2 * PI / mWindow->width) * deltaTime; // Left to right = 360 degrees (2 * PI)
			//float deltaY = ORBIT_SPEED * PI / mWindow->height; // Top to bottom = 180 degrees (PI)

			float xAngle = (mLastMouseX - mousePos.x) * deltaX;
			//float yAngle = (mLastMouseY - mousePos.y) * deltaY;
			//yAngle = -yAngle; // Invert

			// Rotate object
			object.transform->SetRotationAngle(object.transform->rotationAngle() + glm::radians(xAngle));

			//object.transform->SetRotationAngle();
		}

		float scrollY = mGuiIO->MouseWheel;
		if (scrollY != 0.0f)
		{
			Camera& camera = mScene->mCamera;

			glm::vec4 camPos(camera.pos(), 1.0f);
			glm::vec3 camForward(camera.forward());

			float scrollAmount = scrollY * ZOOM_SPEED * deltaTime;

			if (mCurrentZoom + scrollAmount > MAX_ZOOM)
			{
				mCurrentZoom = MAX_ZOOM;
			}
			else if (mCurrentZoom + scrollAmount < MIN_ZOOM)
			{
				mCurrentZoom = MIN_ZOOM;
			}
			else
			{
				mCurrentZoom += scrollAmount;

				glm::mat4 translateMatrix(1.0f);
				translateMatrix = glm::translate(translateMatrix, camForward * scrollAmount);

				glm::vec3 finalPos = translateMatrix * camPos;

				camera.SetPos(finalPos);
			}
		}

		mLastMouseX = mousePos.x;
		mLastMouseY = mousePos.y;
	}

	bool Application::Initialise()
	{
		if (!InitialiseGL()) { return false; }
		if (!InitialiseImGui()) { return false; }
		if (!InitialiseScene()) { return false; }

		// Initialise renderer
		mRenderer = new Renderer();
		if (!mRenderer->Initialise(mWindow, mScene)) { return false; }
		mRenderer->SetClearColour(glm::vec3(0.2f, 0.2f, 0.2f));
		
		// Creates environment and irradiance cubemaps and gives their ids to the scene
		mScene->mSkyboxMesh = CreateCube();
		mScene->mEnvironment = LoadEnvironment("RidgecrestRoad", mWindow);
		//CreateEnvironment();

		// Initialise gui layer
		mGuiLayer = new GuiLayer(mRenderer, mScene, mWindow);

		return true;
	}

	bool Application::InitialiseGL()
	{
		// Initialise Window object
		mWindow = new Window();
		mWindow->width = WINDOW_WIDTH;
		mWindow->height = WINDOW_HEIGHT;

		if (!InitialiseGLFW(&mWindow->glfwWindow)) { return false; }
		if (!InitialiseGlad()) { return false; }

		// Link this Application object to GLFW callbacks to allow interaction
		glfwSetWindowUserPointer(mWindow->glfwWindow, this);

		return true;
	}

	bool InitialiseGLFW(GLFWwindow** windowPtr)
	{
		if (!glfwInit())
		{
			return false;
		}

		// Set error callback before doing anything else with GLFW
		glfwSetErrorCallback(GLFWErrorCallback);

		// Set window hints before creating window
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Using OpenGL 4.6
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Using OpenGL Core
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Window not resizable

		// Create window
		*windowPtr = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
		if (!*windowPtr)
		{
			return false;
		}
		glfwMakeContextCurrent(*windowPtr);

		// Set callbacks
		glfwSetWindowCloseCallback(*windowPtr, GLFWWindowCloseCallback);

		return true;
	}

	bool InitialiseGlad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return false;
		}

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		//glEnable(GL_MULTISAMPLE);

		return true;
	}

	bool Application::InitialiseImGui()
	{
		// Create context objects
		mGuiContext = ImGui::CreateContext();
		mGuiIO = &ImGui::GetIO();
		mGuiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;

		// Call initialisation functions
		if (!ImGui_ImplGlfw_InitForOpenGL(mWindow->glfwWindow, true)) { return false; }
		if (!ImGui_ImplOpenGL3_Init("#version 460")) { return false; }

		// Set style
		ImGui::StyleColorsDark();

		return true;
	}

	bool Application::InitialiseScene()
	{
		mScene = new Scene();

		// Initialise scene camera
		{
			glm::vec3 pos = glm::vec3(0.0f, 0.0f, -3.0f);
			glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

			float fov = 45.0f;
			glm::vec2 viewportSize = glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
			float near = 0.1f;
			float far = 100.0f;

			mScene->mCamera = Camera(pos, at, up, fov, viewportSize, near, far);
		}

		// Initialise scene object
		{
			SceneObject& object = mScene->mObject;

			// Initialise material
			object.material = LoadMaterial("BambooWood");

			// Initialise mesh
			float radius = 1.0f;
			unsigned int segmentCircumference = 128;
			object.mesh = CreateSphere(radius, segmentCircumference);
			 
			// Initialise transform
			glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
			float rotationAngle = 0.0f;
			glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
			object.transform = new Transform(pos, rotationAngle, rotationAxis, scale);
		}

		return true;
	}

	void GLFWErrorCallback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}

	void GLFWWindowCloseCallback(GLFWwindow* window)
	{
		Application* application = (Application*)glfwGetWindowUserPointer(window);
		application->Quit();
	}
}