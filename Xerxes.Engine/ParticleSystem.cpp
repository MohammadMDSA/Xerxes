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
	emitAccumulatedTime(0.f)
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

	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	//result = device->CreateBlendState(&blendStateDescription, blend.ReleaseAndGetAddressOf());
	//DX::ThrowIfFailed(result);

	states = std::make_unique<DirectX::CommonStates>(device);
	//depthStencil = states->DepthRead();


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

bool Xerxes::Engine::Graphics::ParticleSystem::Update(float time, ID3D11DeviceContext* context, const Matrix& view)
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
	result = UpdateBuffers(context, view);
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

	// Set the random deviation ofwhere the particles can be located when emitted
	particleDeviation = Vector3(2.f, 2.f, 2.f);

	// Set the speed and speed variation of particles
	particleVelocity = 2.f;
	particleVelocityVariation = .2f;

	// Set the physical size of the particles
	particleSize = .2f;

	// Set number of particles to emit per second
	particlePerSecond = 250.f;

	// Set the maximum number of particles allowed in the particle system
	maxParticles = 5000;

	lifeTime = 1;

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

	if (emitAccumulatedTime > (1.f / particlePerSecond))
	{
		emitAccumulatedTime = 0.f;
		emitParticle = true;
	}

	// If there are particles to emit then emit one per time
	if (emitParticle && (currentParticleCount < (maxParticles - 1)))
	{
		currentParticleCount++;

		// Now generate the randomized particle properties
		ParticleType part;
		Vector3 pos(
			((float)rand() - (float)rand()) / RAND_MAX,
			((float)rand() - (float)rand()) / RAND_MAX,
			((float)rand() - (float)rand()) / RAND_MAX
		);
		pos *= particleDeviation;
		part.position = pos;

		part.velocity = particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * particleVelocityVariation;

		Vector3 color(
			((float)rand() - (float)rand()) / RAND_MAX + .5f,
			((float)rand() - (float)rand()) / RAND_MAX + .5f,
			((float)rand() - (float)rand()) / RAND_MAX + .5f
		);
		part.color = color;

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
			particleList[i].position = particleList[j].position;
			particleList[i].color = particleList[j].color;
			particleList[i].velocity = particleList[j].velocity;
			particleList[i].active = particleList[j].active;
			particleList[i].creationTime = particleList[j].creationTime;
			i--;
			j--;
		}

		// Now insert it into the particle array in the correct depth order
		particleList[index].position = part.position;
		particleList[index].color = part.color;
		particleList[index].velocity = part.velocity;
		particleList[index].active = true;
		particleList[index].creationTime = accumulatedTime;
	}
}

void Xerxes::Engine::Graphics::ParticleSystem::UpdateParticles(float time)
{
	// Each frame we update all the particles by making them move downwards using their position, velocity and the frame time
	for (int i = 0; i < currentParticleCount; i++)
	{
		particleList[i].position.y -= particleList[i].velocity * time;
	}
}

void Xerxes::Engine::Graphics::ParticleSystem::KillParticles()
{
	// Kill all the particles that have gone below a certain height range
	for (int i = 0; i < maxParticles; i++)
	{
		if (particleList[i].active && (particleList[i].position.y < -3.f || (accumulatedTime - xmax(particleList[i].creationTime, 0)) > lifeTime))
		{
			particleList[i].active = false;
			currentParticleCount--;

			// Now shift all the live particles back up the array to erase the destroyed paticle and keep the array sorted correctly
			for (int j = i; j < maxParticles - 1; j++)
			{
				particleList[j].position = particleList[j + 1].position;
				particleList[j].color = particleList[j + 1].color;
				particleList[j].velocity = particleList[j + 1].velocity;
				particleList[j].active = particleList[j + 1].active;
				particleList[j].creationTime = particleList[j + 1].creationTime;
			}
			i--;
		}
	}
}

bool Xerxes::Engine::Graphics::ParticleSystem::UpdateBuffers(ID3D11DeviceContext* context, const Matrix& view)
{
	int index;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Initialize vertex array to zeros at first
	memset(vertices, 0, (sizeof(VertexType) * vertexCount));

	Matrix inView;
	view.Invert(inView);
	inView._14 = 0;
	inView._24 = 0;
	inView._34 = 0;
	inView._41 = 0;
	inView._42 = 0;
	inView._43 = 0;

	// Now build the vertex array from the particle list array
	// Each particle is a quat made out of two triangle
	index = 0;
	for (int i = 0; i < currentParticleCount; i++)
	{
		// Bottom left
		vertices[index].position = particleList[i].position + Vector3::Transform(Vector3(-particleSize, -particleSize, 0.f), inView);
		vertices[index].textureCoordinate = Vector2(0.f, 1.f);
		vertices[index].color = Vector4(particleList[i].color.x, particleList[i].color.y, particleList[i].color.z, 1.f);
		index++;

		// Top left
		vertices[index].position = particleList[i].position + Vector3::Transform(Vector3(-particleSize, particleSize, 0.f), inView);
		vertices[index].textureCoordinate = Vector2(0.f, 0.f);
		vertices[index].color = Vector4(particleList[i].color.x, particleList[i].color.y, particleList[i].color.z, 1.f);
		index++;

		// Bottom right
		vertices[index].position = particleList[i].position + Vector3::Transform(Vector3(particleSize, -particleSize, 0.f), inView);
		vertices[index].textureCoordinate = Vector2(1.f, 1.f);
		vertices[index].color = Vector4(particleList[i].color.x, particleList[i].color.y, particleList[i].color.z, 1.f);
		index++;

		// Top right
		vertices[index].position = particleList[i].position + Vector3::Transform(Vector3(particleSize, particleSize, 0.f), inView);
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
