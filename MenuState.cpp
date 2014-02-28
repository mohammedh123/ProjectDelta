#include "MenuState.h"
#include "HGEGlobals.h"
#include "MapState.h"
#include "GrimoireState.h"
#include "guiMenuItem.h"

MenuState::MenuState()
{}

MenuState::~MenuState()
{
}

void MenuState::Initialize()
{
	keyDownCounter = 0;
	selectLERP = 0;
	selectY = 204;
	selectIdx = 0;
	menuOptions.push_back(MenuItem("New Game", NEW_GAME));
	menuOptions.push_back(MenuItem("Grimoire", GRIMOIRE));
	menuOptions.push_back(MenuItem("Exit", EXIT));
	
	deltaAlpha = 0;
	totaltime = 0;
}

void MenuState::Shutdown()
{
	delete this;
   // release whatever resources I loaded in initialize
}

bool MenuState::Update(float delta)
{
	totaltime += delta;
	hgeFont* fnt = res->GetFont("menu_item");
	float mx, my;
	hgeInputEvent evt;
	hgeCreate(HGE_VERSION)->Input_GetMousePos(&mx, &my);

	selectLERP = min(1, selectLERP += delta);
	if(totaltime >= 2)
	{
		IntLERP(deltaAlpha, 255, totaltime-2);
	}

	FloatLERP(selectY, 204 + 1.5*selectIdx*fnt->GetHeight() - fnt->GetHeight()*0.5f, selectLERP);
	while(hgeCreate(HGE_VERSION)->Input_GetEvent(&evt))
	{
		if(evt.type == INPUT_KEYDOWN)
		{
			if(evt.key == HGEK_ENTER)
			{
				switch(selectIdx)
				{
				case NEW_GAME:
					StateManager::getInstance()->pushState(new MapState());
					return true;
				case GRIMOIRE:
					StateManager::getInstance()->pushState(new GrimoireState());
					return true;
				case EXIT:
					return false;
				}
			}

			if(evt.key == HGEK_UP)
			{
				if(keyDownCounter % 5 == 0)
				{
					selectIdx--;
					if(selectIdx < 0)
						selectIdx = menuOptions.size() - 1;
					selectLERP = 0;
				}
				if(evt.flags & HGEINP_REPEAT)
				{
					keyDownCounter++;
				}
			}
			else if(evt.key == HGEK_DOWN)
			{
				if(keyDownCounter % 5 == 0)
				{
					selectIdx++;
					if(selectIdx == menuOptions.size())
						selectIdx = 0;
					selectLERP = 0;
				}
				if(evt.flags & HGEINP_REPEAT)
				{
					keyDownCounter++;
				}
			}
		}
		else if(evt.type == INPUT_KEYUP)
			if(evt.key == HGEK_UP || evt.key == HGEK_DOWN)
				keyDownCounter = 0;
	}
	
	return true; 
}

void MenuState::Render()
{
	HGE* hge = hgeCreate(HGE_VERSION);
	
	hgeSprite* bg = res->GetSprite("menu_bg");
	hgeSprite* delta = res->GetSprite("menu_delta");
	hgeSprite* select = res->GetSprite("menu_select");
	hgeFont* fnt = res->GetFont("menu_item");

	bg->Render(0,0);
	delta->SetColor(ARGB(deltaAlpha, 255, 255, 255));
	delta->Render(462, 320);

	select->Render(0, selectY);
	fnt->SetColor(0xFF00FF26);
	for(int i = 0; i < menuOptions.size(); i++)
	{
		fnt->Render(10, 204 + 1.5*i*fnt->GetHeight() - fnt->GetHeight()*0.5f, HGETEXT_LEFT | HGETEXT_MIDDLE, menuOptions[i].text);
	}

	// render our background, etc.
} 