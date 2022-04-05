#pragma once
#include "pch.h"
#include "IInspectorDrawer.h"

namespace Xerxes
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Effects
			{
				interface IXEffect : public IInspectorDrawer
				{
				public:
					virtual void OnInspector() = 0;
				};
			}
		}
	}
}
