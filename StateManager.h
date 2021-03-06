
//----------------------------------------------------------------------------
// File:         StateManager.h
// Description:  Interface for the StateManager class.
//----------------------------------------------------------------------------
#ifndef _STATEMANAGER_H
#define _STATEMANAGER_H

#include <vector>

// forward declaration for GameState class
class GameState;

//----------------------------------------------------------------------------
// Class:          StateManager
// Description:  Manages game states for the engine.
//----------------------------------------------------------------------------
class StateManager
{
public:
	// Constructor
	StateManager();
	// Destructor
	~StateManager();

	// Push a new state onto the stack
	void pushState(GameState* state);

	// Pop the current state off the stack
	GameState* popState();

	// Remove all states from the stack
	void popAll();

	// Call the current state's Update method
	bool Update(float delta);

	// Call the current state's render method
	void Render();

	// Return the singleton StateManager instance
	static StateManager* getInstance();

private:
	std::vector<GameState*> _states;
	static StateManager* _instance;
};
#endif 