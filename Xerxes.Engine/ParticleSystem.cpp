#include "pch.h"
#include "ParticleSystem.h"
#include "RootManager.h"
#include "ResourceGroup.h"
#include "ParticleEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Xerxes::Engine::Graphics::Effects;

Xerxes::Engine::Graphics::ParticleSystem::ParticleSystem() :
	textureReourceId(-1),
	particleList(nullptr),
	vertices(nullptr),
	vertexBuffer(nullptr),
	indexBuffer(nullptr),
	accumulatedTime(0.f),
	emitAccumulatedTime(0.f),
	particleInitialVelocity(0.f, 1.f, 0.f),
	particleInitialVelocityDeviation(0.2f),
	particleDeviation(2.f),
	particlePerSecond(70),
	particleLifeTime(1.f),
	particleInitialColor(0.f, 0.f, 0.f, 1.f),
	particleInitialColorDeviation(1.f, 1.f, 1.f, 0.f),
	particleInitialSize(0.2),
	particleInitialSizeDeviation(0.f)
{

}

bool Xerxes::Engine::Graphics::ParticleSystem::Initialize(ID3D11Device* device)
{
	bool result;

	// Initialize the particle system
	result = InitializeParticleSystem();
	if (!result)
		return false;

	// Create the buffers that will be usesd to render the particle with
	result = InitializeBuffers(device);
	if (!result)
		return false;

	states = std::make_unique<DirectX::CommonStates>(device);

	return true;
}

void Xerxes::Engine::Graphics::ParticleSystem::Shutdown()
{
	// Release buffers
	ShutdownBuffers();

	// Release the particle system
	ShutdownParticleSystem();
	states.release();
	//blend.Reset();
	//delete depthStencil;

	return;
}

bool Xerxes::Engine::Graphics::ParticleSystem::Update(float time, ID3D11DeviceContext* context)
{
	bool result;

	accumulatedTime += time;

	// Release old particles
	KillParticles();

	// Emit new particles
	EmitParticles(time);

	// Update the position of the particles
	UpdateParticles(time);

	// Update the dynamic vertex buffer with the new position of each particle
	result = UpdateBuffers(context);
	if (!result)
		return false;

	return true;
}

void Xerxes::Engine::Graphics::ParticleSystem::Render(ID3D11DeviceContext* context, EffectResource* effectRes)
{
	// Put the vertex and index buffers on the graphic pipeline to prepare them for drawing
	RenderBuffers(context);

	context->IASetInputLayout(effectRes->GetInputLayout());
	context->OMSetBlendState(states->Additive(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullCounterClockwise());

	if (auto particleEffect = dynamic_cast<ParticleEffect*>(effectRes->GetResource()); particleEffect)
	{
		particleEffect->SetTexture(textureReourceId);
		particleEffect->Apply(context);
	}

	context->DrawIndexed(indexCount, 0, 0);
}

ID3D11ShaderResourceView* Xerxes::Engine::Graphics::ParticleSystem::GetTexture()
{
	auto textureResource = RootManager::GetInstance()->GetResourceManager()->ResourceGroup<TextureResource>::GetById(textureReourceId);
	if (!textureResource)
		return nullptr;
	return textureResource->GetResource();
}

int Xerxes::Engine::Graphics::ParticleSystem::GetIndexCount()
{
	return indexCount;
}

void Xerxes::Engine::Graphics::ParticleSystem::SetTextureId(GameResourceId id)
{
	this->textureReourceId = id;
}

bool Xerxes::Engine::Graphics::ParticleSystem::InitializeParticleSystem()
{
	int i;

	// Set the maximum number of particles allowed in the particle system
	maxParticles = 5000;

	// Create the particle list
	particleList = new ParticleType[maxParticles];
	memset(particleList, 0, maxParticles * sizeof(ParticleType));
	if (!particleList)
		return false;

	// Initialize the partile list
	for (int i = 0; i < maxParticles; i++)
	{
		particleList->active = false;
	}

	// Initialize the current particle count to zero since none are emitted yet
	currentParticleCount = 0;
	emitAccumulatedTime = 0.f;

	return true;
}

void Xerxes::Engine::Graphics::ParticleSystem::ShutdownParticleSystem()
{
	// Release the partile list
	if (particleList)
	{
		delete[] particleList;
		particleList = 0;
	}
}

bool Xerxes::Engine::Graphics::ParticleSystem::InitializeBuffers(ID3D11Device* device)
{
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the maximum number of vertices in the vertex array
	vertexCount = maxParticles * 4;

	// Set the maximum number of indices in the index array
	indexCount = maxParticles * 6;

	// Create the vertex array for the particle that will be rendered
	vertices = new VertexType[vertexCount];
	if (!vertices)
		return false;

	// Create the index array
	indices = new unsigned long[indexCount];
	if (!indices)
		return false;

	// Initialize vertex array to zeros at first
	memset(vertices, 0, (sizeof(VertexType) * vertexCount));

	// Initialize the index array
	for (int i = 0; i < maxParticles; i++)
	{
		int index = i * 6;
		indices[index++] = i * 4;
		indices[index++] = i * 4 + 1;
		indices[index++] = i * 4 + 2;
		indices[index++] = i * 4 + 2;
		indices[index++] = i * 4 + 1;
		indices[index++] = i * 4 + 3;
	}

	// Setup the description of the dynamic vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finaly create the vetex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.ReleaseAndGetAddressOf());
	DX::ThrowIfFailed(result);

	// Setup the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.ReleaseAndGetAddressOf());
	DX::ThrowIfFailed(result);

	// Release the index array since it is no longer needed
	delete[] indices;
	indices = nullptr;

	return true;
}

void Xerxes::Engine::Graphics::ParticleSystem::ShutdownBuffers()
{
	// Release the index buffer
	if (indexBuffer)
	{
		indexBuffer.Reset();
		indexBuffer = nullptr;
	}

	// Release the vetex buffer
	if (vertexBuffer)
	{
		vertexBuffer.Reset();
		vertexBuffer = nullptr;
	}
}

void Xerxes::Engine::Graphics::ParticleSystem::EmitParticles(float time)
{
	// Increment the frame time
	emitAccumulatedTime += time;

	// Set emit particle to false for now
	bool emitParticle = false;

	auto particleTime = 1.f / particlePerSecond;
	auto emitCount = (int)(emitAccumulatedTime / particleTime);
	if (emitCount > 0)
	{
		emitAccumulatedTime = fmodf(emitAccumulatedTime, particleTime);
		emitParticle = true;
	}

	for (int i = 0; i < emitCount; i++)
	{

		// If there are particles to emit then emit one per time
		if (currentParticleCount < (maxParticles - 1))
		{
			currentParticleCount++;

			// Now generate the randomized particle properties
			ParticleType part;
			Vector3 pos(
				XRAND,
				XRAND,
				XRAND
			);
			pos *= particleDeviation;
			part.position = pos;

			XParticleInitialzieVec2PropertyWithNDeviation(part, size, particleInitialSize);

			XParticleInitialzieVec3PropertyWithDeviation(part, velocity, particleInitialVelocity);

			XParticleInitialzieVec4PropertyWithNDeviation(part, color, particleInitialColor);

			// Now since the particles neede to be rendered from back to front for blending we have to sort the particle array
			// We will sort using Z depth so we need to find where in the list the particle should be inserted
			int index = 0;
			bool found = false;
			while (!found)
			{
				if (!particleList[index].active || (particleList[index].position.z < part.position.z))
					found = true;
				else
					index++;
			}

			// Now that we know the location to insert into we need to copy the array over by one position from the index to make room for the new particles
			int i = currentParticleCount;
			int j = i - 1;

			while (i > index)
			{
				SwapParticles(i, j);
				i--;
				j--;
			}

			// Now insert it into the particle array in the correct depth order
			particleList[index] = part;
			particleList[index].active = true;
			particleList[index].creationTime = accumulatedTime;
		}
		else
			break;
	}
}

void Xerxes::Engine::Graphics::ParticleSystem::UpdateParticles(float time)
{
	// Each frame we update all the particles by making them move downwards using their position, velocity and the frame time
	for (int i = 0; i < currentParticleCount; i++)
	{
		particleList[i].position += particleList[i].velocity * time;
	}
}

void Xerxes::Engine::Graphics::ParticleSystem::KillParticles()
{
	// Kill all the particles that have gone below a certain height range
	for (int i = 0; i < maxParticles; i++)
	{
		if (particleList[i].active && (accumulatedTime - xmax(particleList[i].creationTime, 0)) > particleLifeTime)
		{
			particleList[i].active = false;
			currentParticleCount--;

			// Now shift all the live particles back up the array to erase the destroyed paticle and keep the array sorted correctly
			for (int j = i; j < maxParticles - 1; j++)
			{
				SwapParticles(j, j + 1);
			}
			i--;
		}
	}
}

bool Xerxes::Engine::Graphics::ParticleSystem::UpdateBuffers(ID3D11DeviceContext* context)
{
	int index;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Initialize vertex array to zeros at first
	memset(vertices, 0, (sizeof(VertexType) * vertexCount));

	// Now build the vertex array from the particle list array
	// Each particle is a quat made out of two triangle
	index = 0;
	for (int i = 0; i < currentParticleCount; i++)
	{
		// Bottom left
		vertices[index].position = particleList[i].position;
		vertices[index].offset = Vector3(-particleList[i].size.x, -particleList[i].size.y, 0.f);
		vertices[index].textureCoordinate = Vector2(0.f, 1.f);
		vertices[index].color = Vector4(particleList[i].color.x, particleList[i].color.y, particleList[i].color.z, 1.f);
		index++;

		// Top left
		vertices[index].position = particleList[i].position;
		vertices[index].offset = Vector3(-particleList[i].size.x, particleList[i].size.y, 0.f);
		vertices[index].textureCoordinate = Vector2(0.f, 0.f);
		vertices[index].color = Vector4(particleList[i].color.x, particleList[i].color.y, particleList[i].color.z, 1.f);
		index++;

		// Bottom right
		vertices[index].position = particleList[i].position;
		vertices[index].offset = Vector3(particleList[i].size.x, -particleList[i].size.y, 0.f);
		vertices[index].textureCoordinate = Vector2(1.f, 1.f);
		vertices[index].color = Vector4(particleList[i].color.x, particleList[i].color.y, particleList[i].color.z, 1.f);
		index++;

		// Top right
		vertices[index].position = particleList[i].position;
		vertices[index].offset = Vector3(particleList[i].size.x, particleList[i].size.y, 0.f);
		vertices[index].textureCoordinate = Vector2(1.f, 0.f);
		vertices[index].color = Vector4(particleList[i].color.x, particleList[i].color.y, particleList[i].color.z, 1.f);
		index++;
	}

	// Lock the vertex buffer
	result = context->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	DX::ThrowIfFailed(result);

	// Get a pointer to the data in the vertex buffer
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * vertexCount));

	// Unlock the vertex buffer
	context->Unmap(vertexBuffer.Get(), 0);

	return true;
}

void Xerxes::Engine::Graphics::ParticleSystem::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to activev in the input assembler so it can be rendered
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
