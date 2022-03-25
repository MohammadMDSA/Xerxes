#pragma once
#include "EditorWindow.h"

namespace Xerxes
{
	namespace Editor
	{
		namespace Panels
		{
			class InspectorWindow : public EditorWindow
			{
			public:
				InspectorWindow(int id);



				// Inherited via EditorWindow
				virtual void OnGUI() override;
				virtual int GetCustomWindowFlags() override;


				// Inherited via EditorWindow
				virtual void Update(float deltaTime) override;

			};

		}
	}
}