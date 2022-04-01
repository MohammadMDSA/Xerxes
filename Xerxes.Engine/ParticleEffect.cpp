#include "pch.h"
#include "ParticleEffect.h"
#include "RootManager.h"
#include "XPreprocessors.h"

using namespace DirectX;

namespace
{
	constexpr uint32_t DirtyConstantBuffer = 0x1;
	constexpr uint32_t DirtyWVPMatrix = 0x2;
}

Xerxes::Engine::Graphics::Effects::ParticleEffect::ParticleEffect(ID3D11Device* device) :
	dirtyFlags(uint32_t(-1)),
	constantBuffer(device)
{
	static_assert((sizeof(ParticleEffect::ParticleEffectConstants) % 16) == 0, "CB size alignment");

	vsBlob = DX::ReadData(L"Particle_VS.cso");
	DX::ThrowIfFailed(device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, vs.ReleaseAndGetAddressOf()));

	auto psBlob = DX::ReadData(L"Particle_PS.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, ps.ReleaseAndGetAddressOf()));

	D3D11_SAMPLER_DESC samplerDesc;
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	DX::ThrowIfFailed(device->CreateSamplerState(&samplerDesc, &sampleState));

}

Xerxes::Engine::Graphics::Effects::ParticleEffect::~ParticleEffect()
{
	vs.Reset();
	ps.Reset();
	delete sampleState;
}

void Xerxes::Engine::Graphics::Effects::ParticleEffect::Apply(ID3D11DeviceContext* context)
{
	if (dirtyFlags & DirtyWVPMatrix)
	{
		worldViewProj = world * view * proj;
		Vector3 tmp;
		Quaternion rot;
		world.Decompose(tmp, rot, tmp);
		rot.Inverse(inverseRotation);
		dirtyFlags &= ~DirtyWVPMatrix;
		dirtyFlags |= DirtyConstantBuffer;
	}

	if (dirtyFlags & DirtyConstantBuffer)
	{
		Matrix iv = this->view.Invert();
		iv._41 = 0.f;
		iv._42 = 0.f;
		iv._43 = 0.f;
		iv._14 = 0.f;
		iv._24 = 0.f;
		iv._34 = 0.f;

		ParticleEffectConstants constants;
		constants.worldViewProj = worldViewProj.Transpose();
		constants.iViewRot = iv.Transpose();
		constants.iRot = Matrix::CreateFromQuaternion(inverseRotation).Transpose();
		constantBuffer.SetData(context, constants);

		dirtyFlags &= ~DirtyConstantBuffer;
	}

	auto cb = constantBuffer.GetBuffer();
	context->VSSetConstantBuffers(0, 1, &cb);

	auto texResource = XResourceMTexture()::GetById(textureId);

	if (texResource && texResource->IsLoaded())
		context->PSSetShaderResources(0, 1, texResource->GetResourceAddress());
	else
	{
		ID3D11ShaderResourceView* srvs[1] = { 0 };
		context->PSSetShaderResources(0, 1, srvs);
	}

	context->VSSetShader(vs.Get(), nullptr, 0);
	context->PSSetShader(ps.Get(), nullptr, 0);

	context->PSSetSamplers(0, 1, &sampleState);
}

void Xerxes::Engine::Graphics::Effects::ParticleEffect::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
{
	assert(pByteCodeLength != nullptr && pByteCodeLength != nullptr);
	*pShaderByteCode = vsBlob.data();
	*pByteCodeLength = vsBlob.size();
}

void Xerxes::Engine::Graphics::Effects::ParticleEffect::SetWorld(FXMMATRIX value)
{
	this->world = value;
	dirtyFlags |= DirtyWVPMatrix;
}

void Xerxes::Engine::Graphics::Effects::ParticleEffect::SetView(FXMMATRIX value)
{
	this->view = value;

	dirtyFlags |= DirtyWVPMatrix;
}

void Xerxes::Engine::Graphics::Effects::ParticleEffect::SetProjection(FXMMATRIX value)
{
	this->proj = value;
	dirtyFlags |= DirtyWVPMatrix;
}

void XM_CALLCONV Xerxes::Engine::Graphics::Effects::ParticleEffect::SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection)
{
	this->world = world;
	this->SetView(view);
	this->proj = projection;
	dirtyFlags |= DirtyWVPMatrix;
}

void Xerxes::Engine::Graphics::Effects::ParticleEffect::SetTexture(GameResourceId textureId)
{
	this->textureId = textureId;
}
