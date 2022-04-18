#pragma once
#include <SimpleMath.h>
#include "GameResource.h"
#include "ReadData.h"
#include "IXEffect.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace Xerxes
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Effects
			{

				class ParticleEffect : public DirectX::IEffect, DirectX::IEffectMatrices, public Xerxes::Engine::Graphics::Effects::IXEffect
				{
				public:
					explicit ParticleEffect(ID3D11Device* device);
					ParticleEffect(const ParticleEffect& other) = default;
					~ParticleEffect();

					// Inherited via IEffect
					virtual void Apply(ID3D11DeviceContext* deviceContext) override;
					virtual void GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override;

					// Inherited via IEffectMatrices
					virtual void XM_CALLCONV SetWorld(FXMMATRIX value) override;
					virtual void XM_CALLCONV SetView(FXMMATRIX value) override;
					virtual void XM_CALLCONV SetProjection(FXMMATRIX value) override;
					virtual void XM_CALLCONV SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection) override;

					void SetTexture(GameResourceId textureId);

					void OnInspector() override {};

				private:
					uint32_t dirtyFlags;

					struct __declspec(align(16)) ParticleEffectConstants
					{
						DirectX::XMMATRIX worldViewProj;
						DirectX::XMMATRIX iViewRot;
						DirectX::XMMATRIX iRot;
					};

					DirectX::ConstantBuffer<ParticleEffectConstants> constantBuffer;

					ComPtr<ID3D11VertexShader>	vs;
					ComPtr<ID3D11PixelShader>	ps;
					GameResourceId				textureId;
					std::vector<uint8_t>		vsBlob;

					DirectX::SimpleMath::Matrix world;
					DirectX::SimpleMath::Matrix view;
					DirectX::SimpleMath::Matrix proj;
					DirectX::SimpleMath::Matrix worldViewProj;
					DirectX::SimpleMath::Quaternion inverseRotation;

					ID3D11SamplerState*			sampleState;

				};

			}
		}
	}
}