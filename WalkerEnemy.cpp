#include "WalkerEnemy.h"
#include "math.h"

WalkerEnemy::WalkerEnemy(float x, float y, float speed, float angle, float hp, int activeTime, int enemyType, const char * contrFilename, unsigned nsp, unsigned nlp) : Enemy(x, y, speed, angle, hp, activeTime, enemyType, contrFilename, 0, nsp, nlp)
{}

void WalkerEnemy::Update()
{
	Enemy::Update();
	if(activeTime <= 0)
		return;

	x += cosf(angle)*speed;
	y += sinf(angle)*speed;

	UpdateCtrlBndryChck();
}