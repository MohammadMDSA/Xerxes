#pragma once

#include "GameResource.h"

template<class T>
class ResourceGroup
{
public:
	inline T* GetById(int id)
	{
		if (!group.contains(id))
			return nullptr;
		return group[id];
	}

	inline std::vector<T*> GetAll()
	{
		std::vector<T*> result;
		for (auto& it : group) {
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
	}

private:
	friend class ResourceManager;
	std::unordered_map<int, T*> group;
};
