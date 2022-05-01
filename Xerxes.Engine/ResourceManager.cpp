#include "pch.h"
#include "ResourceManager.h"
#include <exception>
#include <algorithm>
#include <cctype>

using namespace boost::filesystem;
using namespace DirectX;

ResourceManager::ResourceManager() :
	createdDefaultResources(0)
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
	modelResource->name = std::string("model_") + path.filename().string();
	modelResource->path = path;

	if (extension(path) == ".sdkmesh")
		modelResource->type = ModelResource::XModelResourceType_SDKMESH;
	else if (extension(path) == ".cmo")
		modelResource->type = ModelResource::XModelResourceType_CMO;
	else
		throw std::exception("Not supported extension");

	ResourceGroup<ModelResource>::group.insert({ modelResource->get_id(), modelResource });
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
	resource->path = path;
	resource->name = pPath.filename().string();
	auto ext = extension(path);
	if (ext == ".dds")
		resource->type = TextureResource::XTextureResourceType_DDS;
	else if (ext == ".bmp")
		resource->type = TextureResource::XTextureResourceType_BMP;
	else
		throw std::exception(("Not supported texture extension: " + ext).c_str());

	ResourceGroup<TextureResource>::group.insert({ resource->get_id(), resource });
	return resource;
}

ResourceManager::ResourceType ResourceManager::IsResourceSupported(std::string extension)
{
	std::transform(extension.begin(), extension.end(), extension.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (extension == ".sdkmesh")
		return ResourceType_Model;
	if (extension == ".cmo")
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
	resource->path = "";
	resource->name = name;
	resource->type = EffectResource::XEffectResourceType_NormalMap;
	ResourceGroup<EffectResource>::group.insert({ resource->get_id(), resource });
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
	states.reset();
	states = std::make_unique<DirectX::CommonStates>(device);
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
		createdDefaultResources = true;
	}
	AddDefaultBatcch();
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

void ResourceManager::OnInit()
{
	context = nullptr;
	device = nullptr;
}

GeometricPrimitiveResource* ResourceManager::AddPrimitive(std::string name)
{
	auto gr = new GeometricPrimitiveResource();
	gr->name = name;
	gr->path = "";
	gr->type = "GeometricPrimitive";
	gr->isDefault = true;
	return gr;
}

void ResourceManager::AddDefaultGeometricPrimitives()
{
	auto grBox = AddPrimitive("Box");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grBox->get_id(), grBox});

	auto grCone = AddPrimitive("Cone");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grCone->get_id(), grCone });

	auto grCube = AddPrimitive("Cube");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grCube->get_id(), grCube });

	auto grCylinder = AddPrimitive("Cylinder");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grCylinder->get_id(), grCylinder });

	auto grDodecahedron = AddPrimitive("Dodecahedron");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grDodecahedron->get_id(), grDodecahedron });

	auto grGeoSphere = AddPrimitive("GeoSphere");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grGeoSphere->get_id(), grGeoSphere });

	auto grIcosahedron = AddPrimitive("Icosahedron");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grIcosahedron->get_id(), grIcosahedron });

	auto grOctahedron = AddPrimitive("Octahedron");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grOctahedron->get_id(), grOctahedron });

	auto grSphere = AddPrimitive("Sphere");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grSphere->get_id(), grSphere });

	auto grTeapot = AddPrimitive("Teapot");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grTeapot->get_id(), grTeapot });

	auto grTetrahedron = AddPrimitive("Tetrahedron");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grTetrahedron->get_id(), grTetrahedron });

	auto grTorus = AddPrimitive("Torus");
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grTorus->get_id(), grTorus });
}

void ResourceManager::AddDefaultEffects()
{
	{
		// Default shape effect
		auto gr = new EffectResource();
		gr->isDefault = true;
		gr->name = "Default Effect";
		gr->path = "";
		gr->type = EffectResource::XEffectResourceType_Basic;
		ResourceGroup<EffectResource>::group.insert({ gr->get_id(), gr });
	}

	{
		// Default position color effect
		auto gre = new EffectResource();
		gre->name = "Position Color Effect";
		gre->path = "";
		gre->type = EffectResource::XEffectResourceType_Basic;
		gre->SetVertexType(EffectResource::XEffectVertexType_VertexPositionColor);
		gre->SetVertextColorEnabled(true);
		gre->SetSystemResource(true);
		ResourceGroup<EffectResource>::group.insert({ gre->get_id(), gre });
	}

	{
		// Particle effect
		auto greParticle = new EffectResource();
		greParticle->name = "Particle Effect";
		greParticle->path = "";
		greParticle->type = EffectResource::XEffectResourceType_Particle;
		greParticle->SetVertexType(EffectResource::XEffectVertexType_VertexPositionOffsetColorTexture);
		greParticle->SetVertextColorEnabled(true);
		greParticle->SetSystemResource(true);
		ResourceGroup<EffectResource>::group.insert({ greParticle->get_id(), greParticle });
	}

	{
		// Material effect
		auto res = new EffectResource();
		res->name = "Material effect";
		res->path = "";
		res->type = EffectResource::XEffectResourceType_Material;
		res->SetVertexType(EffectResource::XEffectVertexType_VertexPositionNormalTexture);
		ResourceGroup<EffectResource>::group.insert({ res->get_id(), res });
	}
}

void ResourceManager::AddDefaultBatcch()
{
	if (dBatch)
		dBatch.reset();
	dBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
}

void ResourceManager::LoadAllSubdirectoriesResources(std::string root)
{
	LoadAllDirectoryResources(path(root));
}

void ResourceManager::OnDeviceLost()
{
}

void ResourceManager::OnDeviceRestored(ID3D11DeviceContext* context, ID3D11Device* device)
{
	SetDevice(device);
	SetDeviceContext(context);
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
