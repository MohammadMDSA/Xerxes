#pragma once
#include <string>
#include <unordered_map>

#include "IManager.h"
#include "boost/filesystem.hpp"

template<class T>
struct GameResource
{
	int						id;
	std::string				name;
	bool					isLoaded;
	std::unique_ptr<T>		resource;
	boost::filesystem::path	path;
};

class ResourceManager : public IManager
{
public:
	ResourceManager();

	int CreateModel(std::wstring path);

	void SetDeviceContext(ID3D11DeviceContext* context);
	void SetDevice(ID3D11Device* device);

	GameResource<DirectX::Model>* GetModel(int id);
	std::vector<GameResource<DirectX::Model>*> GetAllModels();

	// Inherited via IManager
	virtual void OnInit() override;
	virtual void OnShutdown() override;

private:
	int CreateSDKMESHModel(boost::filesystem::path path);
	int GetNewId();

	int lastId;

	std::unordered_map<int, GameResource<DirectX::Model>*>	models;
	std::unordered_map<int, GameResource<DirectX::IEffectFactory>*> effectFactories;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
};

