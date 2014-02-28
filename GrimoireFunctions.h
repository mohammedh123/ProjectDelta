#include "Enemy.h"
#include "HGEGlobals.h"

void SpiralTest1(Enemy* e)
{
	e->IncrementActiveTime();
	if(e->GetActiveTime() <= 0)
		return;

	if(e->lerpTimer >= 1)
	{
		e->lerpTimer = 0;
		e->tx = hgeCreate(HGE_VERSION)->Random_Float(75, CENTERX_OF_AREA*2 - 75);
		e->ty = hgeCreate(HGE_VERSION)->Random_Float(50, 150);
	}

	FloatLERP(e->x, e->tx, e->lerpTimer);
	FloatLERP(e->y, e->ty, e->lerpTimer);
	e->lerpTimer = min(e->lerpTimer += (FRAME_TIME*0.25f), 1);

	e->UpdateCtrlBndryChck();

	if(!e->GetController()->isEnabled())
		e->GetController()->enable(true);
}

void SpecialTest1(Enemy* e)
{
	e->IncrementActiveTime();
	if(e->GetActiveTime() <= 0)
		return;

	if(e->lerpTimer >= 1)
	{
		e->lerpTimer = 0;
		e->tx = hgeCreate(HGE_VERSION)->Random_Float(75, CENTERX_OF_AREA*2 - 75);
		e->ty = hgeCreate(HGE_VERSION)->Random_Float(50, 150);
	}

	FloatLERP(e->x, e->tx, e->lerpTimer);
	FloatLERP(e->y, e->ty, e->lerpTimer);
	e->lerpTimer = min(e->lerpTimer += (FRAME_TIME*0.25f), 1);

	e->UpdateCtrlBndryChck();

	if(!e->GetController()->isEnabled())
		e->GetController()->enable(true);
}

void SpecialTest2(Enemy* e)
{
	e->IncrementActiveTime();
	if(e->GetActiveTime() <= 0)
		return;

	//if(e->lerpTimer >= 1)
	//{
	//	e->lerpTimer = 0;
	//	e->tx = hgeCreate(HGE_VERSION)->Random_Float(75, CENTERX_OF_AREA*2 - 75);
	//	e->ty = hgeCreate(HGE_VERSION)->Random_Float(50, 150);
	//}

	//FloatLERP(e->x, e->tx, e->lerpTimer);
	//FloatLERP(e->y, e->ty, e->lerpTimer);
	//e->lerpTimer = min(e->lerpTimer += (FRAME_TIME*0.25f), 1);

	e->UpdateCtrlBndryChck();

	if(!e->GetController()->isEnabled())
		e->GetController()->enable(true);
}

void SpecialTest3(Enemy* e)
{
	e->IncrementActiveTime();
	if(e->GetActiveTime() <= 0)
		return;

	if(e->lerpTimer >= 1)
	{
		e->lerpTimer = 0;
		e->tx = hgeCreate(HGE_VERSION)->Random_Float(75, CENTERX_OF_AREA*2 - 75);
		e->ty = hgeCreate(HGE_VERSION)->Random_Float(50, 150);
	}

	FloatLERP(e->x, e->tx, e->lerpTimer);
	FloatLERP(e->y, e->ty, e->lerpTimer);
	e->lerpTimer = min(e->lerpTimer += (FRAME_TIME*0.1f), 1);

	e->UpdateCtrlBndryChck();

	if(!e->GetController()->isEnabled())
		e->GetController()->enable(true);
}