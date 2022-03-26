#include "pch.h"
#include "TextureResource.h"

const std::string TextureResource::XTextureResourceType_DDS = "DDSTexture";
const std::string TextureResource::XTextureResourceType_BMP = "BMPTexture";

TextureResource::TextureResource()
{
}

void TextureResource::OnInspector()
{

}

void TextureResource::Initialize(ID3D11DeviceContext* context)
{
	if (loaded)
		return;

	ID3D11Device* device;
	context->GetDevice(&device);

	if (type == XTextureResourceType_BMP)
	{
		DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, path.c_str(), nullptr, &resource));
	}
	else if (type == XTextureResourceType_DDS)
	{
		DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, path.c_str(), nullptr, &resource));
	}
	else
		return;
	loaded = true;
}

void TextureResource::Shutdown()
{
	loaded = false;
	resource.Reset();
}

ID3D11ShaderResourceView* TextureResource::GetResource()
{
	return resource.Get();
}

ID3D11ShaderResourceView* const* TextureResource::GetResourceAddress()
{
	return resource.GetAddressOf();
}
