#pragma once
#include <string>
#include <unordered_map>

#include "IManager.h"
#include "boost/filesystem.hpp"
#include "ResourceGroup.h"
#include "GameResource.h"
#include "TextureResource.h"
#include "ModelResource.h"
#include "EffectResource.h"
#include "GeometricPrimitiveResource.h"
#include "IDeviceNotify.h"

using namespace Xerxes::Engine::Graphics::Device;

class ResourceManager : public IManager, public ResourceGroup<EffectResource>, public ResourceGroup<ModelResource>, public ResourceGroup<TextureResource>, public ResourceGroup<GeometricPrimitiveResource>, public IDeviceNotify
{
public:

	enum ResourceType
	{
		ResourceType_None = 0x0,
		ResourceType_Texture = 0x1,
		ResourceType_Model = 0x2,
		ResourceType_Effect = 0x4
	};

	ResourceManager();

	GameResourceBase* CreateResource(std::string path, ResourceType type);
	GameResourceBase* CreateModel(std::string path);
	GameResourceBase* CreateEffect(std::string type, std::string name);
	GameResourceBase* CreateTexture(std::string path);

	void SetDeviceContext(ID3D11DeviceContext* context);
	void SetDevice(ID3D11Device* device);
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11Device* GetDevice();

	void InitializeResources();

	// Inherited via IManager
	virtual void OnInit() override;
	virtual void OnShutdown() override;

	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetDefaultBatch();

	void LoadAllSubdirectoriesResources(std::string root);

	inline DirectX::CommonStates* GetCommonStates() { return states.get(); }

	// Inherited via IDeviceNotify
	virtual void OnDeviceLost() override;

	virtual void OnDeviceRestored(ID3D11DeviceContext* context, ID3D11Device* device) override;

private:

	GameResourceBase* CreateNormalMapEffect(std::string name);
	GameResourceBase* CreateBasicEffect(std::string name);

	GeometricPrimitiveResource* AddPrimitive(std::string name);
	
	void AddDefaultGeometricPrimitives();
	void AddDefaultEffects();
	void AddDefaultBatcch();

	void LoadAllDirectoryResources(boost::filesystem::path path);

	ResourceType IsResourceSupported(std::string extension);

	bool createdDefaultResources;

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	std::unique_ptr<DirectX::CommonStates> states;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> dBatch;

};

