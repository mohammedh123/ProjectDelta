#ifndef _MAINSTATE
#define _MAINSTATE

#include "HGEGlobals.h"
#include "StateManager.h"
#include "GameState.h"

class MainState : public GameState
{
public:
	MainState(bool isGrimoireTesting = false, GrimoireData* grimData = 0);
	~MainState();

	void Initialize();
	void Shutdown();

	bool Update(float delta);
	void Render();
private:
	bool isTesting;
};

#endif