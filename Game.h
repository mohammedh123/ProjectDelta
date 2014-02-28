#ifndef _GAME
#define _GAME

#include "Entity.h"
#include <deque>
#include <string>

class Game
{
public:
	Game();
	~Game();

	Entity* getEntity(size_t idx) const;
	unsigned getSize() const;
	Entity* getControlledEntity() const;
	
	void addEntity(Entity* n);
	void setControlledEntity(Entity* ent);

	void addNewEntities();
	void executeGameLoop() const;
	void UpdateEntities() const;
	void UpdateAnimations(float delta);
	void removeDeadEntities();
	void drawEntities();
	void checkCollisions();
	void initializeGame();
	
	int level;
	std::deque<LEVEL*> levels;
	LEVEL sandbox;
	LEVEL* currentLevel;
	unsigned graze;
	bool paused;
	unsigned score;
	float curScore;
private:
	Entity* controlledEntity;
};
#endif