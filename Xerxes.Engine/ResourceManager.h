#pragma once
#include <string>
#include <unordered_map>

#include "boost/filesystem.hpp"

struct GameResourceModel
{
	int						id;
	std::string				name;
	bool					isLoaded;
	std::unique_ptr<DirectX::Model>		resource;
	boost::filesystem::path	path;
};

struct GameResourceEffect
{
	int						id;
	std::string				name;
	bool					isLoaded;
	std::unique_ptr<DirectX::IEffectFactory>		resource;
	boost::filesystem::path	path;
};

class ResourceManager
{
public:
	ResourceManager();

	int CreateModel(std::wstring path);

	void SetDeviceContext(ID3D11DeviceContext* context);
	void SetDevice(ID3D11Device* device);

	GameResourceModel* GetModel(int id);

private:
	int CreateSDKMESHModel(boost::filesystem::path path);
	int GetNewId();

	int lastId;

	std::unordered_map<int, GameResourceModel*>	models;
	std::unordered_map<int, GameResourceEffect*> effectFactories;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
};

