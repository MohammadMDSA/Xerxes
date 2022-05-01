#pragma once
#include "XPreprocessors.h"
#include "XObject.h"
#include "XReference.h"

using namespace boost::uuids;
using namespace Xerxes::Engine::Core;

namespace Xerxes
{
	namespace Engine
	{
		namespace Core
		{
			namespace Reference
			{
				template<typename T>
				class Ref
				{
					XClassTop(Ref<T>)
				public:
					Ref(uuid id, XObject* referencer);
					~Ref();

					bool Fetch();
				private:
					T* Get();

					bool dirty;
					T* resource;

					XProperty_R(uuid, id);
					XProperty_R(XObject*, referencer)
				};

			}
		}
	}
}