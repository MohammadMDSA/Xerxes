#pragma once
#include "TextureResource.h"
#include "EffectResource.h"
#include "VertexPositionOffsetColorTexture.h"
#include "XPreprocessors.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>

#define SwapParticles(i, j) \
particleList[i].position = particleList[j].position; \
particleList[i].color = particleList[j].color; \
particleList[i].velocity = particleList[j].velocity; \
particleList[i].active = particleList[j].active; \
particleList[i].creationTime = particleList[j].creationTime; \
particleList[i].size = particleList[j].size;

#define XParticleProperty(type, name) \
public: \
inline const type& Get##name() const { return name; } \
inline void Set##name(type value) { this-> name = value; } \
private: \
friend class ParticleSystemComponent; \
type name;

#define XParticleInitialzieVec3PropertyWithDeviation(partName, partPropName, propName) \
partName.partPropName = propName + Vector3(propName##Deviation.x * XRAND, propName##Deviation.y * XRAND, propName##Deviation.z * XRAND)

#define XParticleInitialzieVec2PropertyWithNDeviation(partName, partPropName, propName) \
partName.partPropName = propName + Vector2(propName##Deviation.x * XNRAND, propName##Deviation.y * XNRAND)

#define XParticleInitialzieVec3PropertyWithNDeviation(partName, partPropName, propName) \
partName.partPropName = propName + Vector3(propName##Deviation.x * XNRAND, propName##Deviation.y * XNRAND, propName##Deviation.z * XNRAND)

#define XParticleInitialzieVec4PropertyWithNDeviation(partName, partPropName, propName) \
partName.partPropName = propName + Vector4(propName##Deviation.x * XNRAND, propName##Deviation.y * XNRAND, propName##Deviation.z * XNRAND, propName##Deviation.w * XNRAND)

#define XParticleVec2Property(name) XParticleProperty(DirectX::SimpleMath::Vector2, name)

#define XParticleVec3Property(name) XParticleProperty(DirectX::SimpleMath::Vector3, name)

#define XParticleVec4Property(name) XParticleProperty(DirectX::SimpleMath::Vector4, name)

#define XParticleVec2VarProperty(propName) \
XParticleVec2Property(propName)## \
XParticleVec2Property(propName##Deviation)

#define XParticleVec3VarProperty(propName) \
XParticleVec3Property(propName)## \
XParticleVec3Property(propName##Deviation)

#define XParticleVec4VarProperty(propName) \
XParticleVec4Property(propName)## \
XParticleVec4Property(propName##Deviation)


using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

using VertexType = Xerxes::Engine::Graphics::VertexTypes::VertexPositionOffsetColorTexture;

class ParticleSystemComponent;

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
				friend class boost::serialization::access;
				
				struct ParticleType
				{
					Vector3 position;
					Vector4 color;
					Vector3 velocity;
					bool active;
					float creationTime;
					Vector2 size;
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

				template<class Archive>
				void serialize(Archive& ar, const unsigned int version)
				{
				}

				DirectX::SimpleMath::Vector3 particleDeviation;

				XParticleVec2VarProperty(particleInitialSize)
				XParticleVec3VarProperty(particleInitialVelocity)
				XParticleVec4VarProperty(particleInitialColor)

				XParticleProperty(float, particlePerSecond)
				int							maxParticles;
				float						particleLifeTime;

				int							currentParticleCount;
				float						emitAccumulatedTime;
				float						accumulatedTime;

				GameResourceId				textureReourceId;
				
				ParticleType*				particleList;
				
				int							vertexCount;
				int							indexCount;
				VertexType*					vertices;
				
				ComPtr<ID3D11Buffer>		vertexBuffer;
				ComPtr<ID3D11Buffer>		indexBuffer;
				std::unique_ptr<DirectX::CommonStates> states;

				friend class ParticleSystemComponent;
			};

		}
	}
}