#include "Game.h"
#include "BulletSpriteTypes.h"
#include "Enemies.h"

void Game::initializeGame()
{
	level = 1;
	levels.push_back(new LEVEL());
	currentLevel = levels[0];

	player = new BalancedPlayer(CENTERX_OF_AREA, SCREEN_HEIGHT - 100);
	
	char* temp = "SurroundOnce";
	currentLevel->enemies.push_back(new WalkerEnemy(clipX1-32, 128, 1.5, 0, 10, 60*1, EST_EN1WHITE, temp, 4));
	currentLevel->enemies.push_back(new WalkerEnemy(clipX2+32, 128+32, 1.5, M_PI, 10, 60*1.25, EST_EN1WHITE, temp, 4));

	currentLevel->enemies.push_back(new WalkerEnemy(clipX1-32, 128, 1.5, 0, 10, 60*4, EST_EN1WHITE, temp, 4));
	currentLevel->enemies.push_back(new WalkerEnemy(clipX2+32, 128+32, 1.5, M_PI, 10, 60*4.25, EST_EN1WHITE, temp, 4));
	currentLevel->enemies.push_back(new WalkerEnemy(clipX1-32, 128+64, 1.5, 0, 10, 60*4.5, EST_EN1WHITE, temp, 4));
	currentLevel->enemies.push_back(new WalkerEnemy(clipX2+32, 128+96, 1.5, M_PI, 10, 60*4.75, EST_EN1WHITE, temp, 4));

	for(int i = 0; i < 10; i++)
	{	
		currentLevel->enemies.push_back(new WalkerEnemy(i % 2 == 0 ? clipX1-32 : clipX2+32, 128+12*i, 2, i % 2 == 0 ? 0 : M_PI, 10, 60*(8+i*0.25f), EST_EN1BLACK, temp, 3));
	}

	currentLevel->enemies.push_back(new DecelEnemy(clipX1-32, 128, 3, 0, 60, 60*14, EST_EN1RED, "TriHoming", 45, 0.075, 4, 1));
	currentLevel->enemies.push_back(new DecelEnemy(clipX2+32, 128, 3, M_PI, 60, 60*14, EST_EN1RED, "TriHoming", 45, 0.075, 4, 1));

	currentLevel->enemies.push_back(new WalkerEnemy(clipX1-32, 128, 1.5, 0, 10, 60*24, EST_EN1WHITE, "BombExplode", 2));
	currentLevel->enemies.push_back(new WalkerEnemy(clipX2+32, 128, 1.5, M_PI, 10, 60*26, EST_EN1WHITE, "BombExplode", 2));	

	currentLevel->enemies.push_back(new WalkerEnemy(clipX1-32, 128, 1.5, 0, 10, 60*30, EST_EN1WHITE, "BombExplode", 2));
	currentLevel->enemies.push_back(new WalkerEnemy(clipX2+32, 128, 1.5, M_PI, 10, 60*32, EST_EN1WHITE, "BombExplode", 2));	

	currentLevel->enemies.push_back(new WalkerEnemy(clipX1-32, 128, 1.5, 0, 10, 60*31, EST_EN1WHITE, temp, 1));
	currentLevel->enemies.push_back(new WalkerEnemy(clipX2+32, 128+32, 1.5, M_PI, 10, 60*31.25, EST_EN1WHITE, temp, 1));
	currentLevel->enemies.push_back(new WalkerEnemy(clipX1-32, 128+64, 1.5, 0, 10, 60*31.5, EST_EN1WHITE, temp, 1));
	currentLevel->enemies.push_back(new WalkerEnemy(clipX2+32, 128+96, 1.5, M_PI, 10, 60*31.75, EST_EN1WHITE, temp, 1));

	currentLevel->enemies.push_back(new DecelEnemy(CENTERX_OF_AREA, 0, 3, M_PI_2, 120, 60*36, EST_EN1RED, "Spiraller", 30, 0.075));
	
	temp = "HomingShot1";

	for(int i = 0; i < 20; i++)
	{	
		currentLevel->enemies.push_back(new WalkerEnemy(clipX1-32, 128+12, 1.5, 0, 0.001f, 60*(44 + i*0.25f), EST_EN1RED, temp, 1));
		currentLevel->enemies.push_back(new WalkerEnemy(clipX2+32, 128+12, 1.5, M_PI, 0.001f, 60*(44.5 + i*0.25f), EST_EN1BLUE, temp, 1));
	}

	currentLevel->enemies.push_back(new DecelEnemy(CENTERX_OF_AREA, 0, 3, M_PI_2, 120, 60*52, EST_EN1MAGENTA, "AreaShot1", 30, 0.075));
	
	currentLevel->enemies.push_back(new DecelEnemy(CENTERX_OF_AREA-64, 0, 3, M_PI_2, 120, 60*60, EST_EN1MAGENTA, "AreaShot1", 30, 0.075));
	currentLevel->enemies.push_back(new DecelEnemy(CENTERX_OF_AREA+64, 0, 3, M_PI_2, 120, 60*60, EST_EN1MAGENTA, "AreaShot1", 30, 0.075));

	currentLevel->enemies.push_back(new Boss1());
}
