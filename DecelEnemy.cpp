#include "DecelEnemy.h"
#include "math.h"

DecelEnemy::DecelEnemy(float x, float y, float speed, float angle, float hp, int activeTime, int enemyType, const char * contrFilename, unsigned timeBeforeDecel, float deceleration, unsigned nsp, unsigned nlp) : Enemy(x, y, speed, angle, hp, activeTime, enemyType, contrFilename, 0, nsp, nlp), timeBeforeDecel(timeBeforeDecel), deceleration(deceleration)
{}

void DecelEnemy::Update()
{
	Enemy::Update();
	if(activeTime <= 0)
		return;

	if(activeTime >= timeBeforeDecel)
		speed -= deceleration;
	if(activeTime >= 600)
	{	
		speed = 3;
		angle = -M_PI_2;
	}
	speed = max(0, speed);

	x += cosf(angle)*speed;
	y += sinf(angle)*speed;

	UpdateCtrlBndryChck();
}