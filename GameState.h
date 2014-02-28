//----------------------------------------------------------------------------
// File:         GameState.h
// Description:  Interface for the GameState abstract class.
//----------------------------------------------------------------------------
#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include "HGEGlobals.h"

class GameState
{
public:
	virtual ~GameState();
	// initialize the state
	virtual void Initialize() = 0;
	// shutdown the state
	virtual void Shutdown() = 0;

	// handle frame Update
	virtual bool Update(float delta) = 0;
	// handle frame rendering
	virtual void Render() = 0;
	float totaltime;
protected:
	hgeGUI* gui;
};

#endif 