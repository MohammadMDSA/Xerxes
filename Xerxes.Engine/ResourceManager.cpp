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

GameResourceModel* ResourceManager::GetModel(int id)
{
	return models[id];
}

int ResourceManager::CreateSDKMESHModel(boost::filesystem::path path)
{

	auto effectResource = new GameResourceEffect();
	effectResource->id = GetNewId();
	effectResource->name = std::string("effect_") + path.filename().string();
	effectResource->isLoaded = true;
	effectResource->path = path;
	effectResource->resource = std::make_unique<EffectFactory>(device);
	((EffectFactory*)effectResource->resource.get())->SetDirectory(path.parent_path().wstring().c_str());
	auto modelResource = new GameResourceModel();
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
