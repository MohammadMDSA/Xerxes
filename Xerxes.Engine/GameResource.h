#pragma once
#include "boost/filesystem.hpp"
#include "IInspectorDrawer.h"

#ifndef GAME_RESOURCE_ID_TYPE
#    include <cstdint>
#    define GAME_RESOURCE_ID_TYPE std::int32_t
#endif

using GameResourceIdType = GAME_RESOURCE_ID_TYPE;

typedef GameResourceIdType GameResourceId;

struct GameResourceBase : public IInspectorDrawer
{
public:
	GameResourceId			GetId() { return id; }
	const std::string		GetName() const { return name; }
	bool					IsLoaded() { return loaded; }
	const boost::filesystem::path& GetPath() const { return path; }
	const std::string		GetType() const { return type; }
	inline bool				IsSystemResource() { return systemResource; }
	inline void				SetSystemResource(bool val) { systemResource = val; }

	virtual void			OnInspector() = 0;
	virtual void			Initialize(ID3D11DeviceContext* context) = 0;
	virtual void			Shutdown() = 0;

protected:
	GameResourceId			id;
	std::string				name;
	std::string				type;
	bool					loaded = false;
	boost::filesystem::path	path;
	bool					isDefault = false;
	bool					systemResource = false;
};

class ResourceManager;

template<class M>
struct GameResource : public GameResourceBase
{
public:
	~GameResource();

	virtual M* GetResource() { return resource.get(); }

	// Inherited via GameResourceBase
	virtual void OnInspector() = 0;

protected:
	friend class ResourceManager;
	std::unique_ptr<M>		resource;

};

template<class M>
inline GameResource<M>::~GameResource()
{
	resource.release();
}
