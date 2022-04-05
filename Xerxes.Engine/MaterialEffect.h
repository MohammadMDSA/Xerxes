#pragma once

#include "GameResource.h"
#include "IXEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
using namespace Xerxes::Engine::Graphics::Effects;

namespace Xerxes
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Effects
			{

				class MaterialEffect : public IEffect, public IEffectLights, public IEffectMatrices, IXEffect
				{
				public:
					MaterialEffect(ID3D11Device* device);
					MaterialEffect(const MaterialEffect& other) = default;
					~MaterialEffect();

					// Inherited via IEffect
					virtual void Apply(ID3D11DeviceContext* deviceContext) override;

					virtual void GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override;


					// Inherited via IEffectLights
					virtual void SetLightingEnabled(bool value) override;

					virtual void SetPerPixelLighting(bool value) override;

					virtual void XM_CALLCONV SetAmbientLightColor(FXMVECTOR value) override;

					virtual void SetLightEnabled(int whichLight, bool value) override;

					virtual void XM_CALLCONV SetLightDirection(int whichLight, FXMVECTOR value) override;

					virtual void XM_CALLCONV SetLightDiffuseColor(int whichLight, FXMVECTOR value) override;

					virtual void XM_CALLCONV SetLightSpecularColor(int whichLight, FXMVECTOR value) override;

					virtual void __cdecl EnableDefaultLighting() override;


					// Inherited via IEffectMatrices
					virtual void XM_CALLCONV SetWorld(FXMMATRIX value) override;

					virtual void XM_CALLCONV SetView(FXMMATRIX value) override;

					virtual void XM_CALLCONV SetProjection(FXMMATRIX value) override;

					virtual void XM_CALLCONV SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection) override;

					virtual void OnInspector() override;

				private:
					uint32_t dirtyFlags;

					struct __declspec(align(16)) MatrixConstants
					{
						XMMATRIX		world;
						XMMATRIX		view;
						XMMATRIX		proj;
					} matrixValues;

					struct __declspec(align(16)) LightConstants
					{
						Vector4		diffuseColor = Vector4::One;
						Vector4		specularColor = Vector4(1, 1, 0, 1);
						float		specularPower = 1.f;
						Vector3		lightDirection = Vector3::One;
					} lightValues;

					ConstantBuffer<MatrixConstants>		matrixBuffer;
					ConstantBuffer<LightConstants>		lightBuffer;

					ComPtr<ID3D11VertexShader>			vs;
					ComPtr<ID3D11PixelShader>			ps;

					GameResourceId						baseTextureId = 28;
					GameResourceId						normalTextureId = 16;
					GameResourceId						specularTextureId = 26;

					std::vector<uint8_t>				vsBlob;

					ID3D11SamplerState*					sampleStates;

				};

			}
		}
	}
}
