#pragma once
#include "pch.h"

namespace Xerxes
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Effects
			{
				interface IXEffect
				{
				public:
					virtual void OnInspector() = 0;
				};
			}
		}
	}
}
