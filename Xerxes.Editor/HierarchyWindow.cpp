#include "pch.h"
#include "HierarchyWindow.h"
#include "RootManager.h"
#include <algorithm>

HierarchyWindow::HierarchyWindow(int id)
	: EditorWindow(id, "Hierarchy")
{
	backgroundAlpha = 0.7f;
}

HierarchyWindow::~HierarchyWindow()
{
}

void HierarchyWindow::Update(float deltaTime)
{
}

void HierarchyWindow::OnGUI()
{
	auto sceneManager = RootManager::GetInstance()->GetSceneManager();
	auto selection = RootManager::GetInstance()->GetSelectionManager();
	auto scene = sceneManager->GetCurrentScene();
	auto allObjs = scene->GetGameObjects();
	std::vector<GameObject*> rootObjects;
	std::copy_if(allObjs.begin(), allObjs.end(), std::back_inserter(rootObjects), [](GameObject* obj)
		{
			return !obj->transform().GetParent();
		});
	DrawNodesHierarchy(&rootObjects, selection);
}

int HierarchyWindow::GetCustomWindowFlags()
{
	return 0;
}

void HierarchyWindow::DrawNodesHierarchy(std::vector<GameObject*>* objects, SelectionManager* selection)
{
	int i = 0;
	for (auto go : *objects)
	{
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		i++;
		auto hasChildren = go->transform().GetChildren()->size() > 0;

		if (!hasChildren)
			nodeFlags |= ImGuiTreeNodeFlags_Leaf;
		if (selection->GetSelectedInspectorDrawer() == go)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, go->GetName().c_str());

		// Handling Drag and drop
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(typeid(go).name(), &go, sizeof(GameObject*), ImGuiCond_Always);
			ImGui::Text((go->GetName() + "(GameObject)").c_str());
			ImGui::EndDragDropSource();
		}

		// Handling drag and drop target
		if (ImGui::BeginDragDropTarget())
		{
			auto payload = ImGui::GetDragDropPayload();
			if (payload)
			{
				if (payload->IsDataType(typeid(GameObject*).name()))
				{
					GameObject* payloadGo = *(GameObject**)payload->Data;
					if (!IsGameObjectInTargetHierarchy(payloadGo, &go->transform()))
					{
						if (payload = (ImGui::AcceptDragDropPayload(typeid(GameObject*).name())))
						{
							payloadGo->transform().SetParent(&go->transform());
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Handling selection
		if (ImGui::IsItemClicked())
		{
			selection->SetSelectedGameObject(go);
		}
		// Drawing children
		if (nodeOpen)
		{
			std::vector<GameObject*> childGameObjects;
			auto children = go->transform().GetChildren();
			std::transform(children->begin(), children->end(), back_inserter(childGameObjects), [](Transform* t)
				{
					return t->GetGameObject();
				});
			DrawNodesHierarchy(&childGameObjects, selection);
			try
			{
				ImGui::TreePop();

			}
			catch (const std::exception&)
			{

			}
		}
	}
}

bool HierarchyWindow::IsGameObjectInTargetHierarchy(GameObject* obj, Transform* target)
{
	auto sourceTransform = &obj->transform();
	while (target)
	{
		if (target == sourceTransform)
			return true;
		target = target->GetParent();
	}
	return false;
}
