#include "pch.h"

#include "GameObjectComponent.h"
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_GUID(GameObjectComponent, "GameObjectComponent")

RTTR_REGISTRATION
{
	rttr::registration::class_<GameObjectComponent>(XNameOf(GameObjectComponent))
		/*.property<>("gameObject", &GameObjectComponent::gameObject);*/
		;
}

GameObjectComponent::GameObjectComponent()
{
}

GameObjectComponent::GameObjectComponent(const GameObjectComponent& other)
{
	this->gameObject = other.gameObject;
}
