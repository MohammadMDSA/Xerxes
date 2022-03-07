#pragma once
#include <string>
#include <unordered_map>

#include "IManager.h"
#include "boost/filesystem.hpp"

class ResourceManager;

struct GameResourceBase
{
public:
	int						GetId() { return id; }
	std::string				GetName() { return name; }
	bool					IsLoaded() { return isLoaded; }
	const boost::filesystem::path& GetPath() const { return path; }

protected:
	int						id;
	std::string				name;
	bool					isLoaded;
	boost::filesystem::path	path;
};

template<class T>
struct GameResource : public GameResourceBase
{
	T*						GetResource() { return resource.get(); }

private:
	friend class ResourceManager;
	std::unique_ptr<T>		resource;

};

class ResourceManager : public IManager
{
public:
	ResourceManager();

	int CreateModel(std::wstring path);

	void SetDeviceContext(ID3D11DeviceContext* context);
	void SetDevice(ID3D11Device* device);
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11Device* GetDevice();

	GameResource<DirectX::Model>* GetModel(int id);
	std::vector<GameResource<DirectX::Model>*> GetAllModels();
	std::vector<GameResource<DirectX::IEffectFactory>*> GetAllEffects();

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

