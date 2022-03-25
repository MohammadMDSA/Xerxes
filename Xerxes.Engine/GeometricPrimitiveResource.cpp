#include "pch.h"
#include "GeometricPrimitiveResource.h"

using namespace DirectX;

void GeometricPrimitiveResource::OnInspector()
{
}

void GeometricPrimitiveResource::Initialize(ID3D11DeviceContext* context)
{
	loaded = false;
	this->resource.reset();
	if (name == "Box")
	{
		this->resource = GeometricPrimitive::CreateBox(context, DirectX::SimpleMath::Vector3::One);
	}
	else if (name == "Cone")
	{
		this->resource = GeometricPrimitive::CreateCone(context);
	}
	else if (name == "Cube")
	{
		this->resource = GeometricPrimitive::CreateCube(context);
	}
	else if (name == "Cylinder")
	{
		this->resource = GeometricPrimitive::CreateCylinder(context);
	}
	else if (name == "Dodecahedron")
	{
		this->resource = GeometricPrimitive::CreateDodecahedron(context);
	}
	else if (name == "GeoSphere")
	{
		this->resource = GeometricPrimitive::CreateGeoSphere(context);
	}
	else if (name == "Icosahedron")
	{
		this->resource = GeometricPrimitive::CreateIcosahedron(context);
	}
	else if (name == "Octahedron")
	{
		this->resource = GeometricPrimitive::CreateOctahedron(context);
	}
	else if (name == "Sphere")
	{
		this->resource = GeometricPrimitive::CreateSphere(context, 1.f, 64);
	}
	else if (name == "Teapot")
	{
		this->resource = GeometricPrimitive::CreateTeapot(context);
	}
	else if (name == "Tetrahedron")
	{
		this->resource = GeometricPrimitive::CreateTetrahedron(context);
	}
	else if (name == "Torus")
	{
		this->resource = GeometricPrimitive::CreateTorus(context);
	}

	loaded = true;

}

void GeometricPrimitiveResource::Shutdown()
{
	loaded = false;
	resource.release();
}
