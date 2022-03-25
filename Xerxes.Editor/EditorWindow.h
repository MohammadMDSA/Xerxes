#pragma once

#include <string>

namespace Xerxes
{
	namespace Editor
	{
		namespace Panels
		{
			class EditorWindow
			{
			public:
				EditorWindow(int id, std::string);
				~EditorWindow();

				void					BeginWindow();
				void					EndWindow();
				virtual void			Update(float deltaTime) = 0;
				virtual void			OnGUI() = 0;

				void					SetFullscreen(bool fullScreen);

				std::string				GetTitle() const { return title; }
				void					SetTitle(std::string title) { this->title = title; }
				void					SetPosition(float x, float y);
				void					SetDimansion(float width, float height);

				int						GetId() { return id; }
				bool					GetAlwaysFullscreen() { return alwaysFullscreen; }
				void					SetAlwaysFullScreen(bool afs) { this->alwaysFullscreen = afs; }
				inline float			GetHeight() { return height > 0 ? height : 0; }
				inline float			GetWidth() { return width > 0 ? width : 0; }
				inline float			GetPosX() { return positionX >= 0 ? positionX : 0; }
				inline float			GetPosY() { return positionY >= 0 ? positionY : 0; }
				bool					HasFocus() { return hasFocus; }

			protected:

				virtual void			OnGUIInit() { }
				virtual int				GetCustomWindowFlags() = 0;

				bool					firstFrame;


				int						id;
				std::string				title;
				bool					alwaysFullscreen;
				bool					fullscreen;
				float					backgroundAlpha;
				float					width;
				float					height;
				float					positionX;
				float					positionY;
				bool					isOpen;
				bool					hasFocus;
				bool					isHovered;

				bool					shouldUpdatePosition;
				bool					shouldUpdateDimansion;

			private:
				int						GetForcedFullScreenFlags();
				int						GetFullScreenFlags();

			};

		}
	}
}