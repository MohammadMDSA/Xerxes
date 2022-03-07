#include "pch.h"
#include "ResourceWindow.h"
#include "Libs/imgui/imgui.h"
#include "RootManager.h"

ResourceWindow::ResourceWindow(int id):
	EditorWindow(id, "Resources")
{
}

ResourceWindow::~ResourceWindow()
{
}

void ResourceWindow::Update(float deltaTime)
{
}

void ResourceWindow::OnGUI()
{
	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	if (ImGui::BeginTabBar("ResourceType"))
	{
		if (ImGui::BeginTabItem("Models"))
		{
			int index = 0;
			for (auto model : resourceManager->GetAllModels())
			{
				ImGui::PushID(++index);
				
				if (ImGui::Selectable(model->GetName().c_str(), selectedResource == model))
				{
					selectedResource = model;
				}

				ImGui::PopID();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Effects"))
		{
			int index = 0;
			for (auto effectFactory : resourceManager->GetAllEffects())
			{
				ImGui::PushID(++index);

				if (ImGui::Selectable(effectFactory->GetName().c_str(), selectedResource == effectFactory))
				{
					selectedResource = effectFactory;
				}

				ImGui::PopID();
			}
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

int ResourceWindow::GetCustomWindowFlags()
{
	return 0;
}
