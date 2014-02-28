#ifndef _BULLETFUNCS
#define _BULLETFUNCS

#include "HGEGlobals.h"

void AngleIncrement(bs::UserTypeBase* userType, float frameTime, const float* args)
{
	bullet* b = static_cast<bullet*>(userType);
	b->angle += args[-1]*M_PI_180;
}

void OrbitAroundCenter(bs::UserTypeBase* userType, float frameTime, const float* args)
{
	bullet* b = static_cast<bullet*>(userType);
	b->x = args[-2] + cosf(b->angle + b->framesAlive*M_PI_180*args[-1])*b->framesAlive;
	b->y = args[-3] + sinf(b->angle + b->framesAlive*M_PI_180*args[-1])*b->framesAlive;
}

void Homing(bs::UserTypeBase* userType, float frameTime, const float* args)
{
	projectile* b = static_cast<bullet*>(userType);
	b->angle = atan2(player->GetY() - b->y, player->GetX() - b->x);
}

void BounceOffWalls(bs::UserTypeBase* userType, float frameTime, const float* args)
{
	projectile* b = static_cast<projectile*>(userType);
	
	if(b->anim < 0)
		return;
	bool bounced = false;
	float newA;

	if(b->x > clipX2 || b->x < clipX1)
	{
		newA = -(b->angle + M_PI);
		bounced = true;
	}
	else if(b->y < clipY1 || b->y > clipY2)
	{
		newA = -b->angle;
		bounced = true;
	}

	if(bounced)
	{
		BIRTH_ANIM* g = &CreateSpawnEffect(clipX2, b->y, -(b->angle+M_PI), b->anim);
		switch(b->type)
		{
		case 0://bullet
			{
			bullet* newB = &CreateBullet(clipX2, b->y, b->speed, newA, b->anim, ((bullet*)b)->isFriendly, ((bullet*)b)->radius);
			g->bul = newB;
			//killBullet(userType,0);
			break;
			}
		case 1://laser
			//CreateLaser1(float x, float y, float speed, float angle, float radius, unsigned cFrames, int anim); //uniform linear
			{
			laser* newB = &CreateLaser1(clipX2, b->y, b->speed, newA, ((laser*)b)->radius, ((laser*)b)->creationFrames, b->anim);
			g->bul = newB;
			//killBullet(userType,0);
			break;
			}
		case 2:
			b->angle = newA;
			break;
		}
	}
}

void PlayerHoming(bs::UserTypeBase* userType, float frameTime, const float* args)
{
	bullet* b = static_cast<bullet*>(userType);
	Enemy* en = 0;
	if(b->target && b->target->IsExpired())
		b->target = 0;
	if(!b->target)
	{
		for(size_t i = 0; i < gMain->currentLevel->enemies.size(); i++)
		{
			en = gMain->currentLevel->enemies[i];
			if(en->GetActiveTime() >= 0 && !en->IsExpired())
				b->target = en;
		}
	}

	if(b->target)
	{			
		float temp = atan2(b->target->GetY() - b->y, b->target->GetX() - b->x);
		
			float change = 0;
		for(int i = 0; i < 12; i++)
		{
		if(fabs(b->angle - temp) >= M_PI_180 * 2)
		{
			if(b->angle - temp < 0)
				change = 1;
			else
				change = -1;

			if(fabs(b->angle - temp) > M_PI)
				change = 0 - change;

			b->angle += change*M_PI_180;
		}
		else
			break;
		}

	}
}

void Spin(bs::UserTypeBase* userType, float frameTime, const float* args)
{
	bullet* b = static_cast<bullet*>(userType);
	
	b->spriteangle += args[-1]*M_PI_180;
}

//sets speed to  to args[1] after args[2] frames
void SetSpeedAfterFrames(bs::UserTypeBase* userType, float frameTime, const float* args)
{
	bullet* b = static_cast<bullet*>(userType);

	if(b->framesAlive == args[-2])
	{
		b->speed = args[-1];
		//b->angle += hgeCreate(HGE_VERSION)->Random_Float(-M_PI_180*5,M_PI_180*5);
	}
}
#endif