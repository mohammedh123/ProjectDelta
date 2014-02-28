#include "guiMenuItem.h"
#include "HGEGlobals.h"

guiMenuItem::guiMenuItem(int _id, float _x, float _y, hgeFont* fnt, const std::string& title, hgeSprite* bg) : title(title), font(fnt), cx(_x), cy(_y), totaltime(0), focused(false), bg(bg), zeroTimer(0), angle(0), alpha(255), fading(false)
{
	float w;
	
	id=_id;

	bStatic=false;
	bVisible=true;
	bEnabled=true;

	w=font->GetStringWidth(title.c_str());

	rect.Set(cx-w/2, cy-fnt->GetHeight()/2, cx+w/2, cy+font->GetHeight()/2);
}

void guiMenuItem::Render()
{
	bg->SetColor(ARGB(alpha, 255,255,255));
	if(focused)
		bg->RenderEx(cx+angle, cy, 0, 2, 1);
	else
		bg->RenderEx(cx+angle, cy, 0, 2, 1);

	if(focused)
		font->SetColor(ARGB(alpha, 255, 255, 255));
	else
		font->SetColor(ARGB(min(0x88,alpha), 255, 255, 255));
	font->Render(cx, cy-font->GetHeight()*0.5f, HGETEXT_CENTER, title.c_str());
}

void guiMenuItem::Update(float dt)
{
	totaltime += dt;

	if(fading) 
		alpha -= 4;
	if(!focused)
		zeroTimer += dt;
	else
		zeroTimer = 0;

	if(focused)
		angle = 10*sinf(2*totaltime);
	else
		FloatLERP(angle, 0, zeroTimer);
}

void guiMenuItem::Enter()
{
}

void guiMenuItem::Leave()
{
	fading = true;
}

bool guiMenuItem::IsDone()
{
	return alpha <= 0;
}

void guiMenuItem::Focus(bool bFocused)
{
	if(bFocused)
		totaltime = 0;
	else
		zeroTimer = 0;

	focused = bFocused;
}

bool guiMenuItem::MouseLButton(bool bDown)
{
	return false;
}

bool guiMenuItem::KeyClick(int key, int chr)
{
	if(key == HGEK_ENTER)
		return true;

	return false;
}