#include "pch.h"
#include "ResourceManager.h"
#include <exception>

using namespace boost::filesystem;
using namespace DirectX;

ResourceManager::ResourceManager() :
	lastId(0),
	context(nullptr),
	device(nullptr)
{
}

int ResourceManager::CreateModel(std::wstring path)
{
	auto filePath = boost::filesystem::path(path);
	if (!exists(filePath))
		throw std::exception("File not found");

	if (extension(filePath) == ".sdkmesh")
		return CreateSDKMESHModel(filePath);
	throw std::exception("Not supported extension");
}

void ResourceManager::SetDeviceContext(ID3D11DeviceContext* context)
{
	this->context = context;
}

void ResourceManager::SetDevice(ID3D11Device* device)
{
	this->device = device;
}

GameResource<DirectX::Model>* ResourceManager::GetModel(int id)
{
	return models[id];
}

ID3D11DeviceContext* ResourceManager::GetDeviceContext()
{
	return context;
}

ID3D11Device* ResourceManager::GetDevice()
{
	return device;
}

std::vector<GameResource<DirectX::Model>*> ResourceManager::GetAllModels()
{
	std::vector<GameResource<DirectX::Model>*> result;
	for (auto& it : models) {
		result.push_back(it.second);
	}
	return result;
}

int ResourceManager::CreateSDKMESHModel(boost::filesystem::path path)
{

	auto effectResource = new GameResource<IEffectFactory>();
	effectResource->id = GetNewId();
	effectResource->name = std::string("effect_") + path.filename().string();
	effectResource->isLoaded = true;
	effectResource->path = path;
	effectResource->resource = std::make_unique<EffectFactory>(device);
	((EffectFactory*)effectResource->resource.get())->SetDirectory(path.parent_path().wstring().c_str());
	auto modelResource = new GameResource<Model>();
	modelResource->id = GetNewId();
	modelResource->name = std::string("model_") + path.filename().string();
	modelResource->isLoaded = true;
	modelResource->path = path;
	modelResource->resource = Model::CreateFromSDKMESH(
		device,
		path.wstring().c_str(),
		*effectResource->resource,
		ModelLoader_IncludeBones
	);
	effectFactories.insert({ effectResource->id, effectResource });
	models.insert({ modelResource->id, modelResource });
	return modelResource->id;
}

int ResourceManager::GetNewId()
{
	return lastId++;
}

std::vector<GameResource<DirectX::IEffectFactory>*> ResourceManager::GetAllEffects()
{
	std::vector<GameResource<DirectX::IEffectFactory>*> result;
	for (auto& it : effectFactories) {
		result.push_back(it.second);
	}
	return result;
}

void ResourceManager::OnInit()
{
}

void ResourceManager::OnShutdown()
{
	for (auto& it : models) {
		delete it.second;
	}
	models.clear();
	for (auto& it : effectFactories)
	{
		delete it.second;
	}
	effectFactories.clear();
}
