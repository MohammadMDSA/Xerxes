#include "pch.h"
#include "ResourceManager.h"
#include <exception>
#include <algorithm>
#include <cctype>

using namespace boost::filesystem;
using namespace DirectX;

ResourceManager::ResourceManager() :
	createdDefaultResources(0),
	lastId(-1)
{
}

GameResourceBase* ResourceManager::CreateResource(std::string path, ResourceType type)
{
	switch (type)
	{
	case ResourceManager::ResourceType_Texture:
		return CreateTexture(path);
	case ResourceManager::ResourceType_Model:
		return CreateModel(path);
	default:
		return nullptr;
	}
}

GameResourceBase* ResourceManager::CreateModel(std::string p)
{
	auto path = boost::filesystem::path(p);

	if (!exists(path))
		throw std::exception("File not found");

	auto modelResource = new ModelResource();
	modelResource->id = GetNewId();
	modelResource->name = std::string("model_") + path.filename().string();
	modelResource->path = path;

	if (extension(path) == ".sdkmesh")
		modelResource->type = ModelResource::XModelResourceType_SDKMESH;
	else
		throw std::exception("Not supported extension");

	ResourceGroup<ModelResource>::group.insert({ modelResource->id, modelResource });
	return modelResource;

}

GameResourceBase* ResourceManager::CreateEffect(std::string type, std::string name)
{
	if (type == EffectResource::XEffectResourceType_NormalMap)
		return CreateNormalMapEffect(name);
}

GameResourceBase* ResourceManager::CreateTexture(std::string path)
{
	auto pPath = boost::filesystem::path(path);
	auto resource = new TextureResource();
	resource->id = GetNewId();
	resource->path = path;
	resource->name = pPath.filename().string();
	auto ext = extension(path);
	if (ext == ".dds")
		resource->type = TextureResource::XTextureResourceType_DDS;
	else if (ext == ".bmp")
		resource->type = TextureResource::XTextureResourceType_BMP;
	else
		throw std::exception(("Not supported texture extension: " + ext).c_str());

	ResourceGroup<TextureResource>::group.insert({ resource->id, resource });
	return resource;
}

ResourceManager::ResourceType ResourceManager::IsResourceSupported(std::string extension)
{
	std::transform(extension.begin(), extension.end(), extension.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (extension == ".sdkmesh")
		return ResourceType_Model;
	if (extension == ".dds")
		return ResourceType_Texture;
	if (extension == ".bmp")
		return ResourceType_Texture;
	return ResourceType_None;
}

GameResourceBase* ResourceManager::CreateNormalMapEffect(std::string name)
{
	auto resource = new EffectResource();
	resource->id = GetNewId();
	resource->path = "";
	resource->name = name;
	resource->type = EffectResource::XEffectResourceType_NormalMap;
	ResourceGroup<EffectResource>::group.insert({ resource->id, resource });
	return resource;
}

void ResourceManager::SetDeviceContext(ID3D11DeviceContext* context)
{
	this->context = context;
	InitializeResources();
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

void ResourceManager::InitializeResources()
{
	if (!createdDefaultResources)
	{
		AddDefaultGeometricPrimitives();
		AddDefaultEffects();
		AddDefaultBatcch();
		createdDefaultResources = true;
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
	for (auto& it : ResourceGroup<TextureResource>::group)
	{
		it.second->Initialize(context);
	}
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
	{
		// Default shape effect
		auto gr = new EffectResource();
		gr->isDefault = true;
		gr->id = GetNewId();
		gr->name = "Default Effect";
		gr->path = "";
		gr->type = EffectResource::XEffectResourceType_Basic;
		ResourceGroup<EffectResource>::group.insert({ gr->id, gr });
	}

	{
		// Default position color effect
		auto gre = new EffectResource();
		gre->id = GetNewId();
		gre->name = "Position Color Effect";
		gre->path = "";
		gre->type = EffectResource::XEffectResourceType_Basic;
		gre->SetVertexType(EffectResource::XEffectVertexType_VertexPositionColor);
		gre->SetVertextColorEnabled(true);
		gre->SetSystemResource(true);
		ResourceGroup<EffectResource>::group.insert({ gre->id, gre });
	}

	{
		// Particle effect
		auto greParticle = new EffectResource();
		greParticle->id = GetNewId();
		greParticle->name = "Particle Effect";
		greParticle->path = "";
		greParticle->type = EffectResource::XEffectResourceType_Particle;
		greParticle->SetVertexType(EffectResource::XEffectVertexType_VertexPositionColorTexture);
		greParticle->SetVertextColorEnabled(true);
		greParticle->SetSystemResource(true);
		ResourceGroup<EffectResource>::group.insert({ greParticle->id, greParticle });
	}
}

void ResourceManager::AddDefaultBatcch()
{
	dBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
}

void ResourceManager::LoadAllSubdirectoriesResources(std::string root)
{
	LoadAllDirectoryResources(path(root));
}

void ResourceManager::LoadAllDirectoryResources(path path)
{
	if (!exists(path))
		return;
	directory_iterator end_itr; // default construction yields past-the-end
	for (directory_iterator itr(path); itr != end_itr; ++itr)
	{
		if (is_directory(itr->status()))
		{
			LoadAllDirectoryResources(itr->path());
		}
		auto path = itr->path();
		auto type = IsResourceSupported(path.extension().string());
		if (type)
		{
			CreateResource(path.string(), type);
		}
	}
}

void ResourceManager::OnShutdown()
{
	for (auto& it : ResourceGroup<ModelResource>::group) {
		it.second->Shutdown();
		delete it.second;
	}
	ResourceGroup<ModelResource>::group.clear();
	for (auto& it : ResourceGroup<EffectResource>::group)
	{
		it.second->Shutdown();
		delete it.second;
	}
	ResourceGroup<EffectResource>::group.clear();
	for (auto& it : ResourceGroup<GeometricPrimitiveResource>::group)
	{
		it.second->Shutdown();
		delete it.second;
	}
	ResourceGroup<GeometricPrimitiveResource>::group.clear();
	for (auto& it : ResourceGroup<TextureResource>::group)
	{
		it.second->Shutdown();
		delete it.second;
	}
	ResourceGroup<TextureResource>::group.clear();
}

DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* ResourceManager::GetDefaultBatch()
{
	return dBatch.get();
}
