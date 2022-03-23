#include "pch.h"
#include "ResourceManager.h"
#include <exception>

using namespace boost::filesystem;
using namespace DirectX;

ResourceManager::ResourceManager() :
	createdResources(0),
	lastId(-1)
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

int ResourceManager::CreateEffect(std::string type, std::string name)
{
	if (type == EffectResource::XEffectResourceType_NormalMap)
		return CreateNormalMapEffect(name);
}

int ResourceManager::CreateNormalMapEffect(std::string name)
{
	auto resource = new EffectResource();
	resource->id = GetNewId();
	resource->path = "";
	resource->name = name;
	resource->type = EffectResource::XEffectResourceType_NormalMap;
	resource->isLoaded = true;
	ResourceGroup<EffectResource>::group.insert({ resource->id, resource });
	return resource->id;
}

void ResourceManager::SetDeviceContext(ID3D11DeviceContext* context)
{
	this->context = context;
	if (!createdResources)
	{
		AddDefaultGeometricPrimitives();
		AddDefaultEffects();
		AddDefaultBatcch();
		createdResources = true;
	}
	for (auto& it : ResourceGroup<ModelResource>::group)
	{
		it.second->Initialize(context);;
	}
	for (auto& it : ResourceGroup<EffectResource>::group)
	{
		it.second->Initialize(context);
	}
	for (auto& it : ResourceGroup<GeometricPrimitiveResource>::group)
	{
		it.second->Initialize(context);
	}
}

void ResourceManager::SetDevice(ID3D11Device* device)
{
	this->device = device;
}

ID3D11DeviceContext* ResourceManager::GetDeviceContext()
{
	return context;
}

ID3D11Device* ResourceManager::GetDevice()
{
	return device;
}

int ResourceManager::CreateSDKMESHModel(boost::filesystem::path path)
{

	auto modelResource = new ModelResource();
	modelResource->id = GetNewId();
	modelResource->name = std::string("model_") + path.filename().string();
	modelResource->isLoaded = true;
	modelResource->path = path;
	modelResource->type = ModelResource::XModelResourceType_SDKMESH;
	ResourceGroup<ModelResource>::group.insert({ modelResource->id, modelResource });
	modelResource->Initialize(context);
	return modelResource->id;
}

int ResourceManager::GetNewId()
{
	return lastId++;
}

void ResourceManager::OnInit()
{
	lastId = 0;
	context = nullptr;
	device = nullptr;
}

GeometricPrimitiveResource* ResourceManager::AddPrimitive(std::string name)
{
	auto gr = new GeometricPrimitiveResource();
	gr->id = GetNewId();
	gr->name = name;
	gr->path = "";
	gr->type = "GeometricPrimitive";
	gr->isDefault = true;
	return gr;
}

void ResourceManager::AddDefaultGeometricPrimitives()
{
	auto grBox = AddPrimitive("Box");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grBox->id, grBox });

	auto grCone = AddPrimitive("Cone");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grCone->id, grCone });

	auto grCube = AddPrimitive("Cube");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grCube->id, grCube });

	auto grCylinder = AddPrimitive("Cylinder");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grCylinder->id, grCylinder });

	auto grDodecahedron = AddPrimitive("Dodecahedron");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grDodecahedron->id, grDodecahedron });

	auto grGeoSphere = AddPrimitive("GeoSphere");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grGeoSphere->id, grGeoSphere });

	auto grIcosahedron = AddPrimitive("Icosahedron");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grIcosahedron->id, grIcosahedron });

	auto grOctahedron = AddPrimitive("Octahedron");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grOctahedron->id, grOctahedron });

	auto grSphere = AddPrimitive("Sphere");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grSphere->id, grSphere });

	auto grTeapot = AddPrimitive("Teapot");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grTeapot->id, grTeapot });

	auto grTetrahedron = AddPrimitive("Tetrahedron");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grTetrahedron->id, grTetrahedron });

	auto grTorus = AddPrimitive("Torus");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grTorus->id, grTorus });
}

void ResourceManager::AddDefaultEffects()
{
	// Default shape effect
	auto gr = new EffectResource();
	gr->isDefault = true;
	gr->id = GetNewId();
	gr->isLoaded = true;
	gr->name = "Default Effect";
	gr->path = "";
	gr->type = EffectResource::XEffectResourceType_Basic;
	auto effect = dynamic_cast<BasicEffect*>(gr->resource.get());

	ResourceGroup<EffectResource>::group.insert({ gr->id, gr });

	// Default position color effect
	auto gre = new EffectResource();
	gre->id = GetNewId();
	gre->name = "Position Color Effect";
	gre->path = "";
	gre->type = EffectResource::XEffectResourceType_Basic;
	gre->SetVertexType(EffectResource::XEffectVertexType_VertexPositionColor);
	gre->SetVertextColorEnabled(true);
	gre->SetSystemResource(true);
	auto eff = dynamic_cast<BasicEffect*>(gre->resource.get());
	ResourceGroup<EffectResource>::group.insert({ gre->id, gre });
}

void ResourceManager::AddDefaultBatcch()
{
	dBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
}

void ResourceManager::OnShutdown()
{
	for (auto& it : ResourceGroup<ModelResource>::group) {
		delete it.second;
	}
	ResourceGroup<ModelResource>::group.clear();
	for (auto& it : ResourceGroup<EffectResource>::group)
	{
		delete it.second;
	}
	ResourceGroup<EffectResource>::group.clear();
	for (auto& it : ResourceGroup<GeometricPrimitiveResource>::group)
	{
		delete it.second;
	}
	ResourceGroup<GeometricPrimitiveResource>::group.clear();
}

DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* ResourceManager::GetDefaultBatch()
{
	return dBatch.get();
}
