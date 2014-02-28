#include "Boss1.h"
#include "BulletSpriteTypes.h"
#include "HGEGlobals.h"

enum Boss1States
{
	CHARGING_NEXT_ATTACK,
	MOVING_TO_CENTER,
	STATIONARY,
	SA1_SEEK,
	SA1_CHARGE,
	SA2_SEEK,
	SA2_CHARGE
};
//100 250 100 250
Boss1::Boss1() : Boss(224.5, 0, 1100, 60*75, EST_BOSS1, "Boss1"), timeInState(0)
{
	radius = 64;
	state = MOVING_TO_CENTER;
	hpStack.push(400);
	hpStack.push(600);
	hpStack.push(900);
	hpStack.push(1100);
}

void Boss1::Update()
{
	Boss::Update();
	if(GetActiveTime() < 0)
		return;

	if(spriteangleinc <= M_PI_180*10)
	{
		spriteangleinc += M_PI_180*2*FRAME_TIME;
	}
	else
		spriteangleinc = M_PI_180*10;

	SetAngle(GetAngle() + spriteangleinc);
	static hgeVector newpos, playerPos;
	static bool goingLeft = true, act900 = false, act400 = false;
	static BIRTH_ANIM* t;

	if(chargeTime == 0)
	{			
		switch(state)
		{
		case STATIONARY:
			timeInSA = max(----timeInSA, 0);
			break;
		case MOVING_TO_CENTER:
			timeInSA = max(----timeInSA, 0);
			newpos = LERP(hgeVector(GetX(), GetY()), hgeVector(224.5f, SCREEN_HEIGHT*0.5f-64), timeInState*FRAME_TIME*0.5f);
			SetPosition(newpos.x, newpos.y);
			if(GetX() == 224.5f && GetY() == SCREEN_HEIGHT*0.5f-64)
			{
				timeInState = 0;
				//GetController()->smartEnable(true);

				if(prevState == -1)
					GetController()->raiseEvent("hp1100", 0);
				if(prevState == SA1_CHARGE)
					GetController()->raiseEvent("hp600", 0);

				state = STATIONARY;
				prevState = MOVING_TO_CENTER;
			}
			else
				GetController()->enable(false);	
			break;
		case SA1_SEEK:
			timeInSA = min(++timeInSA, 255);
			if(!act900)
			{
				GetController()->raiseEvent("hp900", 0);
				act900 = true;
			}
			if(suspendTime <= 0)
			{
				playerPos.x = player->GetX();
				playerPos.y = player->GetY();
				state = SA1_CHARGE;
				timeInState = 0;
			}
			break;
		case SA1_CHARGE:
			timeInSA = min(++timeInSA, 255);
			newpos = LERP(hgeVector(GetX(), GetY()), playerPos, timeInState*FRAME_TIME*0.25f);
			SetPosition(newpos.x, newpos.y);
			if(newpos == playerPos)
			{
				state = SA1_SEEK;
				suspendTime = 60;
			}
			break;
		case SA2_SEEK:
			timeInSA = min(++timeInSA, 255);
			if(!act400)
			{
				GetController()->raiseEvent("hp400", 0);
				act400 = true;
			}
			if(suspendTime <= 0)
			{
				playerPos.x = player->GetX();
				playerPos.y = 128;
				state = SA2_CHARGE;
				timeInState = 0;
			}
			break;
		case SA2_CHARGE:
			timeInSA = min(++timeInSA, 255);
			newpos = LERP(hgeVector(GetX(), GetY()), playerPos, timeInState*FRAME_TIME*0.5f);
			SetPosition(newpos.x, newpos.y);
			if(newpos == playerPos)
			{
				state = SA2_SEEK;
				suspendTime = 30;
			}
			break;
		}

			timeInState++;
	}
	else
	{
		GetController()->enable(false);

		for(int i = 0; i < 3; i++)
		{
			t = &CreateSpawnEffect(GetX() + hgeCreate(HGE_VERSION)->Random_Float(-chargeTime*4, chargeTime*4), GetY() + hgeCreate(HGE_VERSION)->Random_Float(-chargeTime*4, chargeTime*4), 0, S04);//S04 = glowball
			t->target = this;
			t->decay = 0.005f;
			t->a = 100;
			t->scale = 0.5f;
		}
	}

	chargeTime = max(--chargeTime, 0);
	suspendTime = max(--suspendTime, 0);
}

void Boss1::Render()
{
	if(activeTime < 0)
		return;

	
	if(timeInSA >= 0)
	{
		res->GetSprite("boss1bg")->SetColor(ARGB(max(0, min(255, timeInSA*2)), 255, 255, 255));
		res->GetSprite("boss1bg")->Render(CENTERX_OF_AREA, SCREEN_HEIGHT*0.5f);
		res->GetSprite("boss1bgspin")->SetColor(ARGB(max(0, min(255, timeInSA*2)), 255, 255, 255));
		res->GetSprite("boss1bgspin")->RenderEx(CENTERX_OF_AREA, SCREEN_HEIGHT*0.5f, activeTime*0.5f*M_PI_180);
	}

	Enemy::Render();

	static float diff = clipX2 - 48 - (clipX1 + 48);

	emptySprite->SetColor(0xAA000000);
	emptySprite->RenderStretch(clipX1 + 48-2, clipY1 + 4-2, clipX1+48-2+diff+4, clipY1+12);
	emptySprite->SetColor(0xDDFF4444);
	emptySprite->RenderStretch(clipX1 + 48, clipY1 + 4, clipX1+48+diff*(GetCurrentHP() / GetMaxHP()), clipY1+4+2);
	emptySprite->SetColor(0xDDFFFFFF);
	emptySprite->RenderStretch(clipX1 + 48, clipY1 + 8, clipX1+48+diff*((GetCurrentHP() - (!hpStack.empty() ? hpStack.top() : 0))/ currentHPDiff), clipY1+8+2);
	
}

void Boss1::ChangeState(int hp)
{
	switch(hp)
	{
		case 900:
			chargeTime = 120;
			state = SA1_SEEK;
			break;
		case 600:
			state = MOVING_TO_CENTER;
			prevState = SA1_CHARGE;
			timeInState = 0;
			break;
		case 400:
			chargeTime = 120;
			state = SA2_SEEK;
			timeInState = 0;;
			prevState = STATIONARY;
			break;
	}	
}