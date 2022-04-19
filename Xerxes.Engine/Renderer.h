#pragma once
#include "IManager.h"

namespace Xerxes
{
	namespace Engine
	{
		namespace SubsystemManager
		{

			class Renderer : public IManager
			{
			public:
				virtual void OnInit() override;
				virtual void OnShutdown() override;

			private:

			};
		}
	}
}