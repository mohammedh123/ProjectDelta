#ifndef __BOSS1_H__
#define __BOSS1_H__

#include "Boss.h"

class Boss1 : public Boss
{
	int timeInState;
public:
	Boss1();

	void Update();
	void Render();
	void ChangeState(int hp);
};

#endif