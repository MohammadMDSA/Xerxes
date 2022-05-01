#pragma once

#include "XPreprocessors.h"

#include <rttr/registration_friend.h>
#include <rttr/rttr_enable.h>

namespace Xerxes
{
	namespace Engine
	{
		namespace Core
		{

			class XObject
			{
				XClassTop(XObject)

			protected:
				XObject();
			};
		}
	}
}

