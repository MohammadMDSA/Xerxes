#include "pch.h"
#include "XReference.h"

RTTR_REGISTRATION
{
	XRegisterClass(Xerxes::Engine::Core::Reference::XReference)
	XRegisterProperty_RVal(id, Xerxes::Engine::Core::Reference::XReference)
	XRegisterMethod(Reference, Xerxes::Engine::Core::Reference::XReference)
	XRegisterMethod(Unreference, Xerxes::Engine::Core::Reference::XReference);
}

Xerxes::Engine::Core::Reference::XReference::XReference() :
	id(boost::uuids::random_generator()())
{
}

bool Xerxes::Engine::Core::Reference::XReference::Reference(XObject* obj)
{
	if (referencers.contains(obj))
		return false;
	referencers.insert(obj);
	return true;
}

bool Xerxes::Engine::Core::Reference::XReference::Unreference(XObject* obj)
{
	if (!referencers.contains(obj))
		return false;
	referencers.erase(obj);
	return true;
}
