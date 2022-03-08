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

	AddDefaultGeometricPrimitives();
	AddDefaultEffects();
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
	ResourceGroup<DirectX::Model>::group.insert({ modelResource->id, modelResource });
	return modelResource->id;
}

int ResourceManager::GetNewId()
{
	return lastId++;
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
	gr->isDefault = true;
	return gr;
}

void ResourceManager::AddDefaultGeometricPrimitives()
{
	auto grBox = AddPrimitive("Box");
	grBox->resource = GeometricPrimitive::CreateBox(context, DirectX::SimpleMath::Vector3::One);
	ResourceGroup<GeometricPrimitive>::group.insert({ grBox->id, grBox });

	auto grCone = AddPrimitive("Cone");
	grCone->resource = GeometricPrimitive::CreateCone(context);
	ResourceGroup<GeometricPrimitive>::group.insert({ grCone->id, grCone });

	auto grCube = AddPrimitive("Cube");
	grCube->resource = GeometricPrimitive::CreateCube(context);
	ResourceGroup<GeometricPrimitive>::group.insert({ grCube->id, grCube });

	auto grCylinder = AddPrimitive("Cylinder");
	grCylinder->resource = GeometricPrimitive::CreateCylinder(context);
	ResourceGroup<GeometricPrimitive>::group.insert({ grCylinder->id, grCylinder });

	auto grDodecahedron = AddPrimitive("Dodecahedron");
	grDodecahedron->resource = GeometricPrimitive::CreateDodecahedron(context);
	ResourceGroup<GeometricPrimitive>::group.insert({ grDodecahedron->id, grDodecahedron });

	auto grGeoSphere = AddPrimitive("GeoSphere");
	grGeoSphere->resource = GeometricPrimitive::CreateGeoSphere(context);
	ResourceGroup<GeometricPrimitive>::group.insert({ grGeoSphere->id, grGeoSphere });

	auto grIcosahedron = AddPrimitive("Icosahedron");
	grIcosahedron->resource = GeometricPrimitive::CreateIcosahedron(context);
	ResourceGroup<GeometricPrimitive>::group.insert({ grIcosahedron->id, grIcosahedron });

	auto grOctahedron = AddPrimitive("Octahedron");
	grOctahedron->resource = GeometricPrimitive::CreateOctahedron(context);
	ResourceGroup<GeometricPrimitive>::group.insert({ grOctahedron->id, grOctahedron });

	auto grSphere = AddPrimitive("Sphere");
	grSphere->resource = GeometricPrimitive::CreateSphere(context);
	ResourceGroup<GeometricPrimitive>::group.insert({ grSphere->id, grSphere });

	auto grTeapot = AddPrimitive("Teapot");
	grTeapot->resource = GeometricPrimitive::CreateTeapot(context);
	ResourceGroup<GeometricPrimitive>::group.insert({ grTeapot->id, grTeapot });

	auto grTetrahedron = AddPrimitive("Tetrahedron");
	grTetrahedron->resource = GeometricPrimitive::CreateTetrahedron(context);
	ResourceGroup<GeometricPrimitive>::group.insert({ grTetrahedron->id, grTetrahedron });

	auto grTorus = AddPrimitive("Torus");
	grTorus->resource = GeometricPrimitive::CreateTorus(context);
	ResourceGroup<GeometricPrimitive>::group.insert({grTorus->id, grTorus});
}

void ResourceManager::AddDefaultEffects()
{
	auto gr = new GameResource<IEffect>();
	gr->isDefault = true;
	gr->id = GetNewId();
	gr->isLoaded = true;
	gr->name = "Default Effect";
	gr->path = "";
	gr->type = "Basic Effect";
	gr->resource = std::unique_ptr<IEffect>(new BasicEffect(device));
	dynamic_cast<BasicEffect*>(gr->resource.get())->SetTextureEnabled(true);
	dynamic_cast<BasicEffect*>(gr->resource.get())->SetPerPixelLighting(true);
	dynamic_cast<BasicEffect*>(gr->resource.get())->SetLightingEnabled(true);
	dynamic_cast<BasicEffect*>(gr->resource.get())->SetLightEnabled(0, true);
	dynamic_cast<BasicEffect*>(gr->resource.get())->SetLightDiffuseColor(0, Colors::Red);
	dynamic_cast<BasicEffect*>(gr->resource.get())->SetLightDirection(0, -DirectX::SimpleMath::Vector3::UnitZ);

	ResourceGroup<DirectX::GeometricPrimitive>::group.begin()->second->resource->CreateInputLayout(gr->resource.get(), &dInputLayout);
	ResourceGroup<DirectX::IEffect>::group.insert({ gr->id, gr });
}

void ResourceManager::OnShutdown()
{
	for (auto& it : ResourceGroup<DirectX::Model>::group) {
		delete it.second;
	}
	ResourceGroup<DirectX::Model>::group.clear();
	for (auto& it : ResourceGroup<DirectX::IEffect>::group)
	{
		delete it.second;
	}
	ResourceGroup<DirectX::IEffect>::group.clear();
	for (auto& it : ResourceGroup<DirectX::GeometricPrimitive>::group)
	{
		delete it.second;
	}
	ResourceGroup<DirectX::GeometricPrimitive>::group.clear();
}