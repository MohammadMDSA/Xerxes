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

	auto grBox = AddPrimitive("Box");
	grBox->resource = GeometricPrimitive::CreateBox(context, DirectX::SimpleMath::Vector3::One);
	geometricPrimitives.insert({ grBox->id, grBox });

	auto grCone = AddPrimitive("Cone");
	grCone->resource = GeometricPrimitive::CreateCone(context);
	geometricPrimitives.insert({ grCone->id, grCone });

	auto grCube = AddPrimitive("Cube");
	grCube->resource = GeometricPrimitive::CreateCube(context);
	geometricPrimitives.insert({ grCube->id, grCube });

	auto grCylinder = AddPrimitive("Cylinder");
	grCylinder->resource = GeometricPrimitive::CreateCylinder(context);
	geometricPrimitives.insert({ grCylinder->id, grCylinder });

	auto grDodecahedron = AddPrimitive("Dodecahedron");
	grDodecahedron->resource = GeometricPrimitive::CreateDodecahedron(context);
	geometricPrimitives.insert({ grDodecahedron->id, grDodecahedron });

	auto grGeoSphere = AddPrimitive("GeoSphere");
	grGeoSphere->resource = GeometricPrimitive::CreateGeoSphere(context);
	geometricPrimitives.insert({ grGeoSphere->id, grGeoSphere });

	auto grIcosahedron = AddPrimitive("Icosahedron");
	grIcosahedron->resource = GeometricPrimitive::CreateIcosahedron(context);
	geometricPrimitives.insert({ grIcosahedron->id, grIcosahedron });

	auto grOctahedron = AddPrimitive("Octahedron");
	grOctahedron->resource = GeometricPrimitive::CreateOctahedron(context);
	geometricPrimitives.insert({ grOctahedron->id, grOctahedron });

	auto grSphere = AddPrimitive("Sphere");
	grSphere->resource = GeometricPrimitive::CreateSphere(context);
	geometricPrimitives.insert({ grSphere->id, grSphere });

	auto grTeapot = AddPrimitive("Teapot");
	grTeapot->resource = GeometricPrimitive::CreateTeapot(context);
	geometricPrimitives.insert({ grTeapot->id, grTeapot });

	auto grTetrahedron = AddPrimitive("Tetrahedron");
	grTetrahedron->resource = GeometricPrimitive::CreateTetrahedron(context);
	geometricPrimitives.insert({ grTetrahedron->id, grTetrahedron });

	auto grTorus = AddPrimitive("Torus");
	grTorus->resource = GeometricPrimitive::CreateTorus(context);
	geometricPrimitives.insert({ grTorus->id, grTorus });
}

void ResourceManager::SetDevice(ID3D11Device* device)
{
	this->device = device;
}

GameResource<DirectX::Model>* ResourceManager::GetModel(int id)
{
	if (!models.contains(id))
		return nullptr;
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

	auto effectFactory = std::make_unique<EffectFactory>(device);
	effectFactory->SetDirectory(path.parent_path().wstring().c_str());
	auto modelResource = new GameResource<Model>();
	modelResource->id = GetNewId();
	modelResource->name = std::string("model_") + path.filename().string();
	modelResource->isLoaded = true;
	modelResource->path = path;
	modelResource->type = "SDKMeshModel";
	modelResource->resource = Model::CreateFromSDKMESH(
		device,
		path.wstring().c_str(),
		*effectFactory,
		ModelLoader_IncludeBones
	);
	models.insert({ modelResource->id, modelResource });
	return modelResource->id;
}

int ResourceManager::GetNewId()
{
	return lastId++;
}

std::vector<GameResource<DirectX::IEffect>*> ResourceManager::GetAllEffects()
{
	std::vector<GameResource<DirectX::IEffect>*> result;
	for (auto& it : effects) {
		result.push_back(it.second);
	}
	return result;
}

std::vector<GameResource<DirectX::GeometricPrimitive>*> ResourceManager::GetAllGeometricPrimitives()
{
	std::vector<GameResource<DirectX::GeometricPrimitive>*> result;
	for (auto& it : geometricPrimitives) {
		result.push_back(it.second);
	}
	return result;
}

GameResource<DirectX::IEffect>* ResourceManager::GetEffect(int id)
{
	if (!effects.contains(id))
		return nullptr;
	return effects[id];
}

GameResource<DirectX::GeometricPrimitive>* ResourceManager::GetPrimitive(int id)
{
	if (!geometricPrimitives.contains(id))
		return nullptr;
	return geometricPrimitives[id];
}

void ResourceManager::OnInit()
{
}

GameResource<GeometricPrimitive>* ResourceManager::AddPrimitive(std::string name)
{
	auto gr = new GameResource<GeometricPrimitive>();
	gr->id = GetNewId();
	gr->name = name;
	gr->path = "";
	gr->type = "GeometricPrimitive";
	return gr;
}

void ResourceManager::OnShutdown()
{
	for (auto& it : models) {
		delete it.second;
	}
	models.clear();
	for (auto& it : effects)
	{
		delete it.second;
	}
	models.clear();
	for (auto& it : geometricPrimitives)
	{
		delete it.second;
	}
	geometricPrimitives.clear();
}
































