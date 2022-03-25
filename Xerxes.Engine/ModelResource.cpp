#include "pch.h"
#include "ModelResource.h"

const std::string ModelResource::XModelResourceType_SDKMESH = "SDKMESHModel";

void ModelResource::OnInspector()
{
}

void ModelResource::Initialize(ID3D11DeviceContext* context)
{
	if (loaded)
		return;
	ID3D11Device* device;
	context->GetDevice(&device);
	if (type == XModelResourceType_SDKMESH)
		LoadSDKMESHModel(device);
	loaded = true;
}

void ModelResource::Shutdown()
{
	loaded = false;
	resource.release();
}

void ModelResource::LoadSDKMESHModel(ID3D11Device* device)
{
	auto effectFactory = std::make_unique<DirectX::EffectFactory>(device);
	effectFactory->SetDirectory(path.parent_path().wstring().c_str());
	resource = DirectX::Model::CreateFromSDKMESH(
		device,
		path.wstring().c_str(),
		*effectFactory,
		DirectX::ModelLoader_IncludeBones | DirectX::ModelLoader_Clockwise
	);
}
