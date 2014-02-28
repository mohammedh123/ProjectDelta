#ifndef __BOSS_H__
#define __BOSS_H__

#include "Enemy.h"
#include "bsBulletScript.h"
#include "hge.h"
#include "hgeanim.h"

#include <stack>

class Boss : public Enemy
{
protected:
	int suspendTime, chargeTime, timeInSA;
	float currentHPDiff;
public:
	std::stack<int> hpStack;
	int state, prevState;

	Boss(float x, float y, float hp, int activeTime, int enemyType, const char * contrFilename);
	virtual ~Boss();
	
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void ChangeState(int hp) = 0;
};

#endif