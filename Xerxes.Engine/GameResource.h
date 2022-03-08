#pragma once
#include "boost/filesystem.hpp"
#include "IInspectorDrawer.h"

struct GameResourceBase : public IInspectorDrawer
{
public:
	int						GetId() { return id; }
	const std::string		GetName() const { return name; }
	bool					IsLoaded() { return isLoaded; }
	const boost::filesystem::path& GetPath() const { return path; }
	const std::string		GetType() const { return type; }

	virtual void			OnInspector() = 0;

protected:
	int						id;
	std::string				name;
	std::string				type;
	bool					isLoaded;
	boost::filesystem::path	path;
	bool					isDefault = false;
};

class ResourceManager;

template<class M>
struct GameResource : public GameResourceBase
{
public:
	M* GetResource() { return resource.get(); }

	// Inherited via GameResourceBase
	virtual void OnInspector() = 0;

protected:
	friend class ResourceManager;
	std::unique_ptr<M>		resource;

};
