#ifndef _GRIMSTATE
#define _GRIMSTATE

#include "HGEGlobals.h"
#include <vector>
#include "StateManager.h"
#include "GameState.h"

class GrimoireState : public GameState
{
public:
	GrimoireState();
	~GrimoireState();

	void Initialize();
	void Shutdown();

	bool Update(float delta);
	void Render();
private:
	int keyDownCounter;
	int selectedIdx;
	std::map<GrimoireKey, GrimoireData>::iterator selected, end;
	float totaltime;
};

#endif