#include "pch.h"
#include "MaterialEffect.h"
#include "ReadData.h"
#include "RootManager.h"
#include "XPreprocessors.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


namespace
{
	constexpr uint32_t DirtyMatrixBuffer = 0x1;
	constexpr uint32_t DirtyLightBuffer = 0x2;
	using VertexType = VertexPositionNormalColorTexture;
}

Xerxes::Engine::Graphics::Effects::MaterialEffect::MaterialEffect(ID3D11Device* device) :
	dirtyFlags(uint32_t(-1)),
	matrixBuffer(device),
	lightBuffer(device)
{
	vsBlob = DX::ReadData(L"SpecularMap_VS.cso");
	DX::ThrowIfFailed(device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, vs.ReleaseAndGetAddressOf()));

	auto psBlob = DX::ReadData(L"SpecularMap_PS.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, ps.ReleaseAndGetAddressOf()));

	auto states = CommonStates(device);
	sampleStates = states.LinearWrap();
}

Xerxes::Engine::Graphics::Effects::MaterialEffect::~MaterialEffect()
{
	vs.Reset();
	ps.Reset();
	delete sampleStates;
}

void __cdecl Xerxes::Engine::Graphics::Effects::MaterialEffect::Apply(ID3D11DeviceContext* context)
{
	if (dirtyFlags & DirtyMatrixBuffer)
	{
		matrixBuffer.SetData(context, matrixValues);
		dirtyFlags &= ~DirtyMatrixBuffer;
	}

	if (dirtyFlags & DirtyLightBuffer)
	{
		lightBuffer.SetData(context, lightValues);
		dirtyFlags &= ~DirtyLightBuffer;
	}

	auto mb = matrixBuffer.GetBuffer();
	context->VSSetConstantBuffers(0, 1, &mb);

	auto lb = lightBuffer.GetBuffer();
	context->PSSetConstantBuffers(0, 1, &lb);

	auto resourceManager = XResourceM();

	auto baseTextureResource = resourceManager->ResourceGroup<TextureResource>::GetById(baseTextureId);
	auto normalTextureResource = resourceManager->ResourceGroup<TextureResource>::GetById(normalTextureId);
	auto specularTextureResource = resourceManager->ResourceGroup<TextureResource>::GetById(specularTextureId);

	ID3D11ShaderResourceView* srvs[3] = { 0, 0, 0 };

	if (baseTextureResource && baseTextureResource->IsLoaded())
		srvs[0] = baseTextureResource->GetResource();

	if (normalTextureResource && normalTextureResource->IsLoaded())
		srvs[1] = normalTextureResource->GetResource();
	
	if (specularTextureResource && specularTextureResource->IsLoaded())
		srvs[2] = specularTextureResource->GetResource();

	context->PSSetShaderResources(0, 3, srvs);
	
	context->VSSetShader(vs.Get(), nullptr, 0);
	context->PSSetShader(ps.Get(), nullptr, 0);

	context->PSSetSamplers(0, 1, &sampleStates);
}

void __cdecl Xerxes::Engine::Graphics::Effects::MaterialEffect::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
{
	assert(pByteCodeLength != nullptr, && pByteCodeLength != nullptr);
	*pShaderByteCode = vsBlob.data();
	*pByteCodeLength = vsBlob.size();
}

void __cdecl Xerxes::Engine::Graphics::Effects::MaterialEffect::SetLightingEnabled(bool value)
{
}

void __cdecl Xerxes::Engine::Graphics::Effects::MaterialEffect::SetPerPixelLighting(bool value)
{
}

void XM_CALLCONV Xerxes::Engine::Graphics::Effects::MaterialEffect::SetAmbientLightColor(FXMVECTOR value)
{
}

void __cdecl Xerxes::Engine::Graphics::Effects::MaterialEffect::SetLightEnabled(int whichLight, bool value)
{
}

void XM_CALLCONV Xerxes::Engine::Graphics::Effects::MaterialEffect::SetLightDirection(int whichLight, FXMVECTOR value)
{
	lightValues.lightDirection = value;
	dirtyFlags |= DirtyLightBuffer;
}

void XM_CALLCONV Xerxes::Engine::Graphics::Effects::MaterialEffect::SetLightDiffuseColor(int whichLight, FXMVECTOR value)
{
	lightValues.diffuseColor = value;
	dirtyFlags |= DirtyLightBuffer;
}

void XM_CALLCONV Xerxes::Engine::Graphics::Effects::MaterialEffect::SetLightSpecularColor(int whichLight, FXMVECTOR value)
{
}

void __cdecl Xerxes::Engine::Graphics::Effects::MaterialEffect::EnableDefaultLighting()
{
}

void XM_CALLCONV Xerxes::Engine::Graphics::Effects::MaterialEffect::SetWorld(FXMMATRIX value)
{
	matrixValues.world = XMMatrixTranspose(value);
	dirtyFlags |= DirtyMatrixBuffer;
}

void XM_CALLCONV Xerxes::Engine::Graphics::Effects::MaterialEffect::SetView(FXMMATRIX value)
{
	matrixValues.view = XMMatrixTranspose(value);
	dirtyFlags |= DirtyMatrixBuffer;
}

void XM_CALLCONV Xerxes::Engine::Graphics::Effects::MaterialEffect::SetProjection(FXMMATRIX value)
{
	matrixValues.proj = XMMatrixTranspose(value);
	dirtyFlags |= DirtyMatrixBuffer;
}

void XM_CALLCONV Xerxes::Engine::Graphics::Effects::MaterialEffect::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
	SetWorld(world);
	SetView(view);
	SetProjection(projection);
}
