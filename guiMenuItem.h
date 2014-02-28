#ifndef _GUIMENUITEM_H_
#define _GUIMENUITEM_H_

#include "HGEGlobals.h"

class guiMenuItem : public hgeGUIObject
{
public:
	guiMenuItem(int id, float x, float y, hgeFont* fnt, const std::string& title, hgeSprite* bg);

	virtual void	Render();
	virtual void	Update(float dt);

	virtual void	Enter();
	virtual void	Leave();
	virtual bool	IsDone();
	virtual void	Focus(bool bFocused);

	virtual bool	MouseLButton(bool bDown);
	virtual bool	KeyClick(int key, int chr);
private:
	hgeSprite* bg;
	bool focused, fading;
	int alpha;
	float totaltime, zeroTimer, angle;
	float cx, cy;
	std::string title;
	hgeFont* font;
};

#endif