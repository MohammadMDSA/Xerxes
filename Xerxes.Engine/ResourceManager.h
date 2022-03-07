#pragma once
#include <string>
#include <unordered_map>

#include "IManager.h"
#include "boost/filesystem.hpp"

class ResourceManager;

struct GameResourceBase
{
public:
	int						GetId() { return id; }
	const std::string		GetName() const { return name; }
	bool					IsLoaded() { return isLoaded; }
	const boost::filesystem::path& GetPath() const { return path; }
	const std::string		GetType() const { return type; }

protected:
	int						id;
	std::string				name;
	std::string				type;
	bool					isLoaded;
	boost::filesystem::path	path;
};

template<class T>
struct GameResource : public GameResourceBase
{
	T* GetResource() { return resource.get(); }

private:
	friend class ResourceManager;
	std::unique_ptr<T>		resource;

};

template<class T>
class ResourceGroup
{
public:
	inline GameResource<T>* GetById(int id)
	{
		if (!group.contains(id))
			return nullptr;
		return group[id];
	}

	inline std::vector<GameResource<T>*> GetAll()
	{
		std::vector<GameResource<T>*> result;
		for (auto& it : group) {
			result.push_back(it.second);
		}
		return result;
	}

private:
	friend class ResourceManager;
	std::unordered_map<int, GameResource<T>*> group;
};

class ResourceManager : public IManager, public ResourceGroup<DirectX::IEffect>, public ResourceGroup<DirectX::Model>, public ResourceGroup<DirectX::GeometricPrimitive>
{
public:
	ResourceManager();

	int CreateModel(std::wstring path);

	void SetDeviceContext(ID3D11DeviceContext* context);
	void SetDevice(ID3D11Device* device);
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11Device* GetDevice();

	// Inherited via IManager
	virtual void OnInit() override;
	virtual void OnShutdown() override;

private:
	int CreateSDKMESHModel(boost::filesystem::path path);
	int GetNewId();
	GameResource<DirectX::GeometricPrimitive>* AddPrimitive(std::string name);

	int lastId;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
};

