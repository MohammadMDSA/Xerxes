#include "pch.h"
#include "ResourceManager.h"
#include <exception>

using namespace boost::filesystem;
using namespace DirectX;

ResourceManager::ResourceManager()
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

int ResourceManager::CreateEffect(IEffect* effect)
{
	auto basic = dynamic_cast<BasicEffect*>(effect);
	if (basic)
		return CreateBasicEffect(basic);
}

int ResourceManager::CreateBasicEffect(DirectX::BasicEffect* effect)
{
	auto resource = new EffectResource();
	resource->id = GetNewId();
	resource->path = "";
	resource->name = "Scene Grid Effect";
	resource->type = "BasicEffect";
	resource->isLoaded = true;
	resource->resource = std::unique_ptr<BasicEffect>(effect);
	ResourceGroup<EffectResource>::group.insert({ resource->id, resource });
	return resource->id;
}

void ResourceManager::SetDeviceContext(ID3D11DeviceContext* context)
{
	this->context = context;

	AddDefaultGeometricPrimitives();
	AddDefaultEffects();
	AddDefaultBatcch();
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
	auto modelResource = new ModelResource();
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
	ResourceGroup<ModelResource>::group.insert({ modelResource->id, modelResource });
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
	grBox->resource = GeometricPrimitive::CreateBox(context, DirectX::SimpleMath::Vector3::One);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grBox->id, grBox });

	auto grCone = AddPrimitive("Cone");
	grCone->resource = GeometricPrimitive::CreateCone(context);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grCone->id, grCone });

	auto grCube = AddPrimitive("Cube");
	grCube->resource = GeometricPrimitive::CreateCube(context);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grCube->id, grCube });

	auto grCylinder = AddPrimitive("Cylinder");
	grCylinder->resource = GeometricPrimitive::CreateCylinder(context);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grCylinder->id, grCylinder });

	auto grDodecahedron = AddPrimitive("Dodecahedron");
	grDodecahedron->resource = GeometricPrimitive::CreateDodecahedron(context);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grDodecahedron->id, grDodecahedron });

	auto grGeoSphere = AddPrimitive("GeoSphere");
	grGeoSphere->resource = GeometricPrimitive::CreateGeoSphere(context);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grGeoSphere->id, grGeoSphere });

	auto grIcosahedron = AddPrimitive("Icosahedron");
	grIcosahedron->resource = GeometricPrimitive::CreateIcosahedron(context);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grIcosahedron->id, grIcosahedron });

	auto grOctahedron = AddPrimitive("Octahedron");
	grOctahedron->resource = GeometricPrimitive::CreateOctahedron(context);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grOctahedron->id, grOctahedron });

	auto grSphere = AddPrimitive("Sphere");
	grSphere->resource = GeometricPrimitive::CreateSphere(context);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grSphere->id, grSphere });

	auto grTeapot = AddPrimitive("Teapot");
	grTeapot->resource = GeometricPrimitive::CreateTeapot(context);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grTeapot->id, grTeapot });

	auto grTetrahedron = AddPrimitive("Tetrahedron");
	grTetrahedron->resource = GeometricPrimitive::CreateTetrahedron(context);
	ResourceGroup<GeometricPrimitiveResource>::group.insert({ grTetrahedron->id, grTetrahedron });

	auto grTorus = AddPrimitive("Torus");
	grTorus->resource = GeometricPrimitive::CreateTorus(context);
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
	gr->type = "BasicEffect";
	gr->resource = std::unique_ptr<BasicEffect>(new BasicEffect(device));
	auto effect = dynamic_cast<BasicEffect*>(gr->resource.get());
	effect->SetTextureEnabled(false);
	effect->SetPerPixelLighting(true);
	effect->SetLightingEnabled(true);
	effect->SetLightEnabled(0, true);
	dynamic_cast<BasicEffect*>(gr->resource.get())->SetLightDirection(0, -DirectX::SimpleMath::Vector3::UnitZ);

	ResourceGroup<GeometricPrimitiveResource>::group.begin()->second->resource->CreateInputLayout(gr->resource.get(), &dInputLayout);
	ResourceGroup<EffectResource>::group.insert({ gr->id, gr });

	// Default position color effect
	auto gre = new EffectResource();
	gre->id = GetNewId();
	gre->name = "Position Color Effect";
	gre->path = "";
	gre->type = "BasicEffect";
	gre->resource = std::make_unique<BasicEffect>(device);

	auto eff = gre->resource.get();
	eff->SetVertexColorEnabled(true);
	CreateInputLayoutFromEffect<VertexPositionColor>(device, eff, &vertexPositionInputLayout);
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

ID3D11InputLayout* ResourceManager::GetDefaultInputLayout()
{
	return dInputLayout.Get();
}

ID3D11InputLayout* ResourceManager::GetVertexPositionColorInputLayout()
{
	return vertexPositionInputLayout.Get();
}

DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* ResourceManager::GetDefaultBatch()
{
	return dBatch.get();
}
