#pragma once
#include "XObject.h"

using namespace Xerxes::Engine::Core;

namespace Xerxes
{
	namespace Engine
	{
		namespace Core
		{
			namespace Reference
			{
				class XReference : public XObject
				{
					XClass(XReference, XObject);

				public:
					XReference();

					bool Reference(XObject*);
					bool Unreference(XObject*);
					inline std::size_t CountRefs() const { return referencers.size(); }
				private:
					std::unordered_set<XObject*> referencers;

					XProperty_R(boost::uuids::uuid, id)
				};

			}
		}
	}
}