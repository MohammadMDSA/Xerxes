#include "pch.h"

#include "GameObjectComponent.h"

GameObjectComponent::GameObjectComponent(std::string nm) :
	name(nm)
{
}

GameObjectComponent::GameObjectComponent(const GameObjectComponent& other)
{
	this->gameObject = other.gameObject;
	this->name = other.name;
}
