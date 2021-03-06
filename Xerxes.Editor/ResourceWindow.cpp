#include "pch.h"
#include "ResourceWindow.h"
#include "Libs/imgui/imgui.h"
#include "RootManager.h"

using namespace Xerxes::Editor::Panels;

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
	auto selection = RootManager::GetInstance()->GetSelectionManager();
	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	if (ImGui::BeginTabBar("ResourceType"))
	{
		if (ImGui::BeginTabItem("Models"))
		{
			int index = 0;
			for (auto model : resourceManager->ResourceGroup<ModelResource>::GetAll())
			{
				ImGui::PushID(++index);
				
				if (ImGui::Selectable(model->GetName().c_str(), selectedResource == model))
				{
					selectedResource = model;
					selection->SetSelectedResource(selectedResource);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip((model->GetType() + " (" + std::to_string(model->GetId()) + ")").c_str());

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

				if (ImGui::Selectable(effect->GetName().c_str(), selectedResource == effect))
				{
					selectedResource = effect;
					selection->SetSelectedResource(selectedResource);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip((effect->GetType() + " (" + std::to_string(effect->GetId()) + ")").c_str());

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
					selection->SetSelectedResource(selectedResource);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip((primitive->GetType() + " (" + std::to_string(primitive->GetId()) + ")").c_str());

				ImGui::PopID();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Textures"))
		{
			int index = 0;
			for (auto texture : resourceManager->ResourceGroup<TextureResource>::GetAll())
			{
				ImGui::PushID(++index);

				if (ImGui::Selectable(texture->GetName().c_str(), selectedResource == texture))
				{
					selectedResource = texture;
					selection->SetSelectedResource(selectedResource);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip((texture->GetType() + " (" + std::to_string(texture->GetId()) + ")").c_str());

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
