#pragma once

#include <string>

class EditorWindow
{
public:
	~EditorWindow();

	void					MakeFullscreen();
	void					DrawWindow();

	std::string				GetTitle() const { return title; }
	void					SetTitle(std::string title) { this->title = title; }
	int						GetId() { return id; }
	bool					GetAlwaysFullscreen() { return alwaysFullscreen; }
	void					SetAlwaysFullScreen(bool afs) { this->alwaysFullscreen = afs; }
	EditorWindow(int id, std::string);
protected:
	friend class EditorWindowManager;

	virtual void			OnGUI();
	void					BeginWindow();
	void					EndWindow();




	int						id;
	std::string				title;
	bool					alwaysFullscreen;
	bool					fulllscreen;
	float					backgroundAlpha;

};

