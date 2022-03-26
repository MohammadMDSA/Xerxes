#pragma once
#include "TextureResource.h"
#include "EffectResource.h"

using VertexType = DirectX::VertexPositionColorTexture;

namespace Xerxes
{
	namespace Engine
	{
		namespace Graphics
		{

			class ParticleSystem
			{
			public:
				ParticleSystem();
				ParticleSystem(const ParticleSystem& other) = default;
				~ParticleSystem() = default;

				bool						Initialize(ID3D11Device* device);
				void						Shutdown();
				bool						Update(float time, ID3D11DeviceContext* context);
				void						Render(ID3D11DeviceContext* context, EffectResource* effectRes);

				ID3D11ShaderResourceView*	GetTexture();
				int							GetIndexCount();
				void						SetTextureId(GameResourceId id);

			private:

				struct ParticleType
				{
					DirectX::SimpleMath::Vector3 position;
					DirectX::SimpleMath::Vector3 color;
					float velocity;
					bool active;
					float creationTime;
				};

				bool						InitializeParticleSystem();
				void						ShutdownParticleSystem();

				bool						InitializeBuffers(ID3D11Device* device);
				void						ShutdownBuffers();

				void						EmitParticles(float time);
				void						UpdateParticles(float time);
				void						KillParticles();

				bool						UpdateBuffers(ID3D11DeviceContext* context);
				
				void						RenderBuffers(ID3D11DeviceContext* context);

				DirectX::SimpleMath::Vector3 particleDeviation;
				float						particleVelocity;
				float						particleVelocityVariation;
				float						particleSize;
				float						particlePerSecond;
				int							maxParticles;
				float						lifeTime;

				int							currentParticleCount;
				float						emitAccumulatedTime;
				float						accumulatedTime;

				GameResourceId				textureReourceId;
				
				ParticleType*				particleList;
				
				int							vertexCount;
				int							indexCount;
				VertexType*					vertices;
				ID3D11Buffer*				vertexBuffer;
				ID3D11Buffer*				indexBuffer;

			};

		}
	}
}