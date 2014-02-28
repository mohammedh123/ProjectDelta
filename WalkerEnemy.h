#ifndef __WALKERENEMY_H__
#define __WALKERENEMY_H__

#include "Enemy.h"

class WalkerEnemy : public Enemy
{
public:
	WalkerEnemy(float x, float y, float speed, float angle, float hp, int activeTime, int enemyType, const char * contrFilename, unsigned nsp = 0, unsigned nlp = 0);

	void Update();
};

#endif