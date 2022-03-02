#pragma once

#include <string>


class EditorWindow
{
public:
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
	inline float			GetHeight() { return height; }
	inline float			GetWidth() { return width; }
	inline float			GetPosX() { return positionX; }
	inline float			GetPosY() { return positionY; }
	bool					HasFocus() { return hasFocus; }
	EditorWindow(int id, std::string);

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

	bool					shouldUpdatePosition;
	bool					shouldUpdateDimansion;

private:
	int						GetForcedFullScreenFlags();
	int						GetFullScreenFlags();

};

