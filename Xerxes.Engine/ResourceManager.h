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
	int CreateEffect(DirectX::IEffect* effect);

	void SetDeviceContext(ID3D11DeviceContext* context);
	void SetDevice(ID3D11Device* device);
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11Device* GetDevice();

	// Inherited via IManager
	virtual void OnInit() override;
	virtual void OnShutdown() override;

	ID3D11InputLayout* GetDefaultInputLayout();
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetDefaultBatch();

	Microsoft::WRL::ComPtr<ID3D11InputLayout> vertexPositionInputLayout;
private:
	int CreateSDKMESHModel(boost::filesystem::path path);
	int CreateBasicEffect(DirectX::BasicEffect* effect);

	int GetNewId();
	GeometricPrimitiveResource* AddPrimitive(std::string name);
	
	void AddDefaultGeometricPrimitives();
	void AddDefaultEffects();
	void AddDefaultBatcch();

	int lastId;

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> dInputLayout;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> dBatch;

};

