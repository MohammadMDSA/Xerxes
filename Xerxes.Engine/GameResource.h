#pragma once
#include "IInspectorDrawer.h"
#include "XReference.h"

#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>

#ifndef GAME_RESOURCE_ID_TYPE
#    include <cstdint>
#    define GAME_RESOURCE_ID_TYPE boost::uuids::uuid;
#endif

using namespace boost::uuids;
using namespace Xerxes::Engine::Core::Reference;
using GameResourceIdType = boost::uuids::uuid;

typedef GameResourceIdType GameResourceId;

struct GameResourceBase : public XReference, public IInspectorDrawer
{
public:
	inline const std::string	GetName() const { return name; }
	inline bool				IsLoaded() { return loaded; }
	inline const boost::filesystem::path& GetPath() const { return path; }
	inline const std::string	GetType() const { return type; }
	inline bool				IsSystemResource() { return systemResource; }
	inline void				SetSystemResource(bool val) { systemResource = val; }

	virtual void			OnInspector() = 0;
	virtual void			Initialize(ID3D11DeviceContext* context) = 0;
	virtual void			Shutdown() = 0;

protected:
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
