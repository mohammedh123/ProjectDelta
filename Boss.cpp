#include "Boss.h"
#include "HGEGlobals.h"

Boss::Boss(float x, float y, float hp, int activeTime, int enemyType, const char * contrFilename) : Enemy(x, y, 0, 0, hp, activeTime, enemyType, contrFilename)
{
	currentHPDiff = hp;
	timeInSA = 0;
	chargeTime = 0;
	state = -1;
	prevState = -1;
	suspendTime = 0;
}

Boss::~Boss()
{
}

void Boss::Update()
{	
	IncrementActiveTime();
	if(GetActiveTime() < 0)
		return;

	if(!hpStack.empty() && GetCurrentHP() <= hpStack.top())
	{
		ConvertAllToPoints();
		int oldHP = hpStack.top();

		oss.str("");
		oss << "hp" << oldHP;
		GetController()->raiseEvent(oss.str(), 0);
		ChangeState(oldHP);
		hpStack.pop();
		currentHPDiff = oldHP - (hpStack.empty() ? 0 : hpStack.top());
	}

	if(GetCurrentHP() <= 0)
	{
		ConvertAllToPoints();
		SetExpired(true);
	}

	if(IsExpired())
		return;

	UpdateCtrlBndryChck();
}