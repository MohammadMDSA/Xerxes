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

}

void TextureResource::Shutdown()
{
	loaded = false;
	resource.release();
}
