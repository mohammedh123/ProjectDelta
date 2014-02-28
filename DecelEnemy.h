#ifndef __DECELENEMY_H__
#define __DECELENEMY_H__

#include "Enemy.h"

class DecelEnemy : public Enemy
{
	int timeBeforeDecel;
	float deceleration;
public:
	DecelEnemy(float x, float y, float speed, float angle, float hp, int activeTime, int enemyType, const char * contrFilename, unsigned timeBeforeDecel, float deceleration, unsigned nsp = 0, unsigned nlp = 0);

	void Update();
};

#endif