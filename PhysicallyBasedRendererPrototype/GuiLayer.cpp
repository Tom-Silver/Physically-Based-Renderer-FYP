#include "GuiLayer.h"

// External includes
#include <ImGui/imgui.h>

// Internal includes
#include "Environment.h"
#include "EnvironmentLoader.h"
#include "Material.h"
#include "MaterialLoader.h"
#include "Renderer.h"
#include "Scene.h"

#define MAX_RECENT_MATERIALS 5
#define MAX_RECENT_ENVIRONMENTS 5

namespace TSFYP
{
	void AddNameToUniqueQueueList(std::list<std::string>* list, std::string name, const unsigned int maxItems);

	struct LoadMaterialWindow
		: public IGuiObject
	{
	public:
		LoadMaterialWindow(Material** const materialPtr, std::list<std::string>* recentMaterialsPtr)
			: pMaterial(materialPtr)
			, pRecentMaterials(recentMaterialsPtr)
			, input("")
		{}
		
		virtual void CreateGui() override
		{
			if (ImGui::Begin("Load material"))
			{
				if (ImGui::InputText("Material name", input, IM_ARRAYSIZE(input), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					LoadFromInput();
				}

				if (ImGui::Button("Load material"))
				{
					LoadFromInput();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel"))
				{
					close = true;
				}

				ImGui::End();
			}
		}

	private:
		void LoadFromInput()
		{
			std::string name = std::string(input);

			Material* newMaterial = LoadMaterial(name);
			if (newMaterial)
			{
				delete* pMaterial;
				*pMaterial = newMaterial;
			
				AddNameToUniqueQueueList(pRecentMaterials, name, MAX_RECENT_MATERIALS);
			}

			close = true;
		}

		Material** const pMaterial;
		std::list<std::string>* pRecentMaterials;
		char input[128];
	};

	struct LoadEnvironmentWindow
		: public IGuiObject
	{
	public:
		LoadEnvironmentWindow(Environment** const environmentPtr, std::list<std::string>* recentEnvironmentsPtr, Window* windowPtr)
			: pEnvironment(environmentPtr)
			, pRecentEnvironments(recentEnvironmentsPtr)
			, input("")
			, pWindow(windowPtr)
		{}

		virtual void CreateGui() override
		{
			if (ImGui::Begin("Load environment"))
			{
				if (ImGui::InputText("Environment name", input, IM_ARRAYSIZE(input), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					LoadFromInput();
				}

				if (ImGui::Button("Load environment"))
				{
					LoadFromInput();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel"))
				{
					close = true;
				}

				ImGui::End();
			}
		}

	private:
		void LoadFromInput()
		{
			std::string name = std::string(input);

			Environment* newEnvironment = LoadEnvironment(name, pWindow);
			if (newEnvironment)
			{
				delete* pEnvironment;
				*pEnvironment = newEnvironment;

				AddNameToUniqueQueueList(pRecentEnvironments, name, MAX_RECENT_ENVIRONMENTS);
			}

			close = true;
		}

		Environment** const pEnvironment;
		Window* pWindow;
		std::list<std::string>* pRecentEnvironments;
		char input[128];
	};

	struct TextWindow
		: public IGuiObject
	{
	public:
		TextWindow(const std::string& titleOfWindow, const std::string& textToDisplay)
			: text(textToDisplay)
			, title(titleOfWindow)
		{}

		virtual void CreateGui() override
		{
			if (ImGui::Begin(title.c_str()))
			{
				ImGui::Text(text.c_str());

				if (ImGui::Button("Done"))
				{
					close = true;
				}

				ImGui::End();
			}
		}

	private:
		std::string title, text;
	};

	GuiLayer::GuiLayer(Renderer* renderer, Scene* scene, Window* window)
		: pRenderer(renderer)
		, pScene(scene)
		, pWindow(window)
	{}

	void GuiLayer::CreateGui()
	{
		// Create window-sized docking object 
		{
			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

			static bool windowOpen = true;

			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::SetNextWindowBgAlpha(0.0f);
			ImGui::Begin("MainDockingWindow", &windowOpen, windowFlags);

			ImGuiID dockspace_id = ImGui::GetID("MainDockingWindow");
			ImGui::SetNextWindowBgAlpha(0.0f);
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		// Create gui for each separate gui object
		for (int i = 0; i < mGuiObjects.size(); i++)
		{
			IGuiObject* guiObject = mGuiObjects[i];

			guiObject->CreateGui();

			if (guiObject->close) // If this window is ready to close, delete object
			{
				mGuiObjects.erase(mGuiObjects.begin() + i);
				delete guiObject;
				guiObject = nullptr;
			}
		}

		// Create top menu gui
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load material..."))
				{
					mGuiObjects.emplace_back(new LoadMaterialWindow(&pScene->mObject.material, &mRecentMaterials));
				}

				if (ImGui::BeginMenu("Load recent material"))
				{
					int i = 0; // Predeclare the iterator variable so we can increment it beforehand

					// Display all recent items
					std::string toAdd = "";
					for (std::string name : mRecentMaterials)
					{
						if (ImGui::MenuItem(name.c_str()))
						{
							Material* newMaterial = LoadMaterial(name);
							if (newMaterial)
							{
								delete pScene->mObject.material;
								pScene->mObject.material = newMaterial;
							}

							toAdd = name;
						}

						i++; // Increment for each slot used
					}

					if (!toAdd.empty())
					{
						AddNameToUniqueQueueList(&mRecentMaterials, toAdd, MAX_RECENT_MATERIALS);
					}

					// Display any remaining slots as empty using preincremented iterator variable
					for (i; i < MAX_RECENT_MATERIALS; i++)
					{
						if (ImGui::MenuItem("-")) {}
					}


					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Load environment..."))
				{
					mGuiObjects.emplace_back(new LoadEnvironmentWindow(&pScene->mEnvironment, &mRecentEnvironments, pWindow));
				}

				if (ImGui::BeginMenu("Load recent environment"))
				{
					int i = 0; // Predeclare the iterator variable so we can increment it beforehand

					// Display all recent items
					std::string toAdd = "";
					for (std::string name : mRecentEnvironments)
					{
						if (ImGui::MenuItem(name.c_str()))
						{
							Environment* newEnvironment = LoadEnvironment(name, pWindow);
							if (newEnvironment)
							{
								delete pScene->mEnvironment;
								pScene->mEnvironment = newEnvironment;
							}

							toAdd = name;
						}

						i++; // Increment for each slot used
					}

					if (!toAdd.empty())
					{
						AddNameToUniqueQueueList(&mRecentEnvironments, toAdd, MAX_RECENT_ENVIRONMENTS);
					}

					// Display any remaining slots as empty using preincremented iterator variable
					for (i; i < MAX_RECENT_ENVIRONMENTS; i++)
					{
						if (ImGui::MenuItem("-")) {}
					}


					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Controls"))
				{
					mGuiObjects.emplace_back(new TextWindow("Controls", "Hold left click and drag to orbit the camera around the material object\nHold right click and drag to rotate the material object on the Y axis\nScroll the mousewheel to zoom in and out"));
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}


		pScene->CreateGui();

		if (ImGui::Begin("Renderer info"))
		{
			pRenderer->CreateGui();

			ImGui::End();
		}

		ImGui::End(); // End main docking window
	}

	void AddNameToUniqueQueueList(std::list<std::string>* list, std::string name, const unsigned int maxItems)
	{
		list->erase(std::remove(list->begin(), list->end(), name), list->end()); // Remove possible previous instance of this name (thus moving it to the front)
		list->push_front(name);
		if (list->size() > maxItems)
		{
			list->pop_back();
		}
	}
}