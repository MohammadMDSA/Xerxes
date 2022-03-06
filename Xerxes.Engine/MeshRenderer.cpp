#include "pch.h"
#include "MeshRenderer.h"
#include <string>
#include "RootManager.h"

using namespace DirectX;
using namespace std;

MeshRenderer::MeshRenderer() :
	address(nullptr),
	modelResourceId(-1),
	resource(nullptr)
{
}

void MeshRenderer::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext1* context)
{
	if(resource)
	resource->resource->Draw(context, *m_states, gameObject->transform.GetWorldMatrix(), view, proj);
}

void MeshRenderer::OnStart(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	m_states = std::make_unique<CommonStates>(device);
}

void MeshRenderer::OnAwake()
{
}

void MeshRenderer::OnUpdate(float deltaTime)
{
	resource = RootManager::GetInstance()->GetResourceManager()->GetModel(modelResourceId);
}

void MeshRenderer::OnGizmo()
{
}

void MeshRenderer::OnInspector()
{
}

void MeshRenderer::OnDestroy()
{
}

std::string MeshRenderer::GetName()
{
	return "Mesh Renderer";
}

void MeshRenderer::SetModelResourceId(int id)
{
	this->modelResourceId = id;
}
