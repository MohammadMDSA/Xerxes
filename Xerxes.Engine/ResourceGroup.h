#pragma once
#include "GameResource.h"

#include <boost/uuid/uuid.hpp>
#include <boost/functional/hash/hash.hpp>
#include "Types.h"

using namespace Xerxes::Engine;

template<class T>
class ResourceGroup
{
public:
	inline T* GetById(GameResourceId id)
	{
		if (!group.contains(id))
			return nullptr;
		return group[id];
	}

	inline std::vector<T*> GetAll(bool userOnly = true)
	{
		std::vector<T*> result;
		for (auto& it : group) {
			if (userOnly && it.second->IsSystemResource())
				continue;
			result.push_back(it.second);
		}
		return result;
	}

	inline void CleanUpDefaults()
	{
		group.erase(std::remove_if(
			group.begin(), group.end(),
			[](const T*& x) {
				return !x->isDefault; // put your condition here
			}), group.end());
	}

	inline T* GetByName(std::string name)
	{
		for (auto res : group)
		{
			auto resource = dynamic_cast<GameResourceBase*>(res.second);
			if (resource->GetName() == name)
				return res.second;
		}

		return nullptr;
	}

private:
	friend class ResourceManager;
	std::unordered_map<GameResourceId, T*, boost::hash<GameResourceId>> group;
};
