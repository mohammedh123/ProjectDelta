#ifndef _MENUSTATE
#define _MENUSTATE

#include "HGEGlobals.h"
#include <vector>
#include "StateManager.h"
#include "GameState.h"

enum MenuItemType
{
	NEW_GAME,
	GRIMOIRE,
	EXIT
};

struct MenuItem
{
	MenuItem(const char* text, MenuItemType id) : text(text), id(id) {}

	const char* text;
	MenuItemType id;
};

class MenuState : public GameState
{
public:
	MenuState();
	~MenuState();

	void Initialize();
	void Shutdown();

	bool Update(float delta);
	void Render();
private:
	int keyDownCounter;
	float selectLERP, selectY;
	int selectIdx;
	std::vector<MenuItem> menuOptions;
	int deltaAlpha;
	float totaltime;
};

#endif