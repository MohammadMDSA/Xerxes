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

class ResourceManager : public IManager, public ResourceGroup<EffectResource>, public ResourceGroup<ModelResource>, public ResourceGroup<TextureResource>, public ResourceGroup<GeometricPrimitiveResource>
{
public:
	ResourceManager();

	int CreateModel(std::wstring path);
	int CreateEffect(std::string type, std::string name);

	void SetDeviceContext(ID3D11DeviceContext* context);
	void SetDevice(ID3D11Device* device);
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11Device* GetDevice();

	// Inherited via IManager
	virtual void OnInit() override;
	virtual void OnShutdown() override;

	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetDefaultBatch();

private:
	int CreateSDKMESHModel(boost::filesystem::path path);
	int CreateNormalMapEffect(std::string name);
	int CreateBasicEffect(std::string name);

	int GetNewId();
	GeometricPrimitiveResource* AddPrimitive(std::string name);
	
	void AddDefaultGeometricPrimitives();
	void AddDefaultEffects();
	void AddDefaultBatcch();

	int lastId;
	bool createdResources;

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> dBatch;

};

