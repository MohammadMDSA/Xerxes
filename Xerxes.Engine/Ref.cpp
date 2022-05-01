#include "pch.h"
#include "Ref.h"

using namespace boost::uuids;
using namespace Xerxes::Engine::Core;

RTTR_REGISTRATION
{
	XRegisterClass(Xerxes::Engine::Core::Reference::Ref<Xerxes::Engine::Core::Reference::XReference>)
	.constructor<boost::uuids::uuid, Xerxes::Engine::Core::XObject*>();
}

template<typename T>
Xerxes::Engine::Core::Reference::Ref<T>::Ref(uuid _id, XObject* referencer) :
	dirty(true),
	id(_id)
{
	this->referencer = referencer;
}

template<typename T>
bool Xerxes::Engine::Core::Reference::Ref<T>::Fetch()
{
	if (!dirty)
		return resource == nullptr;
	resource = T::Fetch(id)
}
