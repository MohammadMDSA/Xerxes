#pragma once
#include "pch.h"

using namespace DirectX::SimpleMath;

namespace Xerxes
{
	namespace Engine
	{
		namespace Graphics
		{

			struct Light
			{
				Vector4			ambientColor;
				Vector4			diffuseColor;
				Vector4			specularColor;
				float			specularPower;
			};

		}
	}
}