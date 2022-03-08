#include "pch.h"
#include "ResourceWindow.h"
#include "Libs/imgui/imgui.h"
#include "RootManager.h"

ResourceWindow::ResourceWindow(int id):
	EditorWindow(id, "Resources"),
	selectedResource(nullptr)
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
			for (auto model : resourceManager->ResourceGroup<ModelResource>::GetAll())
			{
				ImGui::PushID(++index);
				
				if (ImGui::Selectable((model->GetName() + " (" + model->GetType() + ")").c_str(), selectedResource == model))
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
			for (auto effect : resourceManager->ResourceGroup<EffectResource>::GetAll())
			{
				ImGui::PushID(++index);

				if (ImGui::Selectable((effect->GetName() + " (" + effect->GetType() + ")").c_str(), selectedResource == effect))
				{
					selectedResource = effect;
				}

				ImGui::PopID();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Primitives"))
		{
			int index = 0;
			for (auto primitive : resourceManager->ResourceGroup<GeometricPrimitiveResource>::GetAll())
			{
				ImGui::PushID(++index);

				if (ImGui::Selectable(primitive->GetName().c_str(), selectedResource == primitive))
				{
					selectedResource = primitive;
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
