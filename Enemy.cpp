#include "Enemy.h"
#include "Game.h"
#include "HGEGlobals.h"
#include "BulletSpriteTypes.h"

Enemy::Enemy(float x, float y, float speed, float angle, float hp, int activeTime, int enemyType, const char * contrFilename, float spriteangleinc, unsigned nsp, unsigned nlp, EnemyUpdater func) : 
x(x), y(y), controller(0), hp(hp), maxhp(hp), activeTime(-activeTime), remove(false), angle(angle), speed(speed), spriteangle(0), spriteangleinc(spriteangleinc), enemySpriteType(enemyType), score(hp * 120), numSmallPower(nsp), numLargePower(nlp), func(func), tx(0), ty(0), lerpTimer(1)
{
	anim = new hgeAnimation(*enemySprites[enemyType]);
	if(enemyType >= EST_EN1RED && enemyType <= EST_EN1BLACK)
	{
		Enemy::spriteangleinc = M_PI_6+M_PI_180;
		radius = 16;
	}
	SetController(contrFilename);
}

Enemy::~Enemy()
{
	if(controller)
		machine->destroyController(controller);
	delete anim;
}

float Enemy::GetX() const
{	return x;	}

float Enemy::GetY() const
{	return y;	}

float Enemy::GetCurrentHP() const
{	return hp;	}

float Enemy::GetMaxHP() const
{	return maxhp;	}

float Enemy::GetAngle() const
{	return angle;	}

float Enemy::GetSpeed() const
{	return speed;	}

int Enemy::GetActiveTime() const
{	return activeTime;	}

float Enemy::GetRadius() const
{	return radius;	}

bs::Controller* Enemy::GetController() const
{	return controller;	}

hgeAnimation* Enemy::GetAnimation() const
{	return anim;	}

bool Enemy::IsExpired() const
{	return remove;	}

void Enemy::SetPosition(float x1, float y1)
{	x = x1; y = y1;	}

void Enemy::SetHP(float nhp)
{	hp = nhp;	}

void Enemy::SetAngle(float newa)
{	angle = newa;	}

void Enemy::SetSpeed(float news)
{	speed = news;	}

void Enemy::SetExpired(bool expired)
{	remove = expired;	}

void Enemy::SetController(const char* name)
{
	controller = machine->createController(name, x, y, angle, 0);
	controller->enable(false);
}

void Enemy::Update()
{
	if(func)
	{
		(func)(this);
		return;
	}

	activeTime++;
	if(activeTime <= 0)
		return;

	if(hp <= 0)
	{		
		BIRTH_ANIM r;
		r.anim[0] = NULL;
		r.anim[0] = new hgeAnimation(*res->GetAnimation("enemydeath"));
		r.anim[0]->Play();
		r.anim[1] = NULL;
		r.scale = 1.5f;
		r.decay = 0.1f;
		r.angle = hgeCreate(HGE_VERSION)->Random_Float(0, 2*M_PI);
		r.x = GetX();
		r.y = GetY();
		r.r = 255;
		r.g = 255;
		r.b = 255;
		r.adecay = 15;
		gMain->currentLevel->anims.push_back(r);
		gMain->score += score;

		for(unsigned i = 0; i < numSmallPower; i++)
		{
			gMain->currentLevel->powers.push_back(power(GetX() + hgeCreate(HGE_VERSION)->Random_Float(-64, 64),
				GetY() + hgeCreate(HGE_VERSION)->Random_Float(-32, 32), SMALL));
		}

		for(unsigned i = 0; i < numLargePower; i++)
		{
			gMain->currentLevel->powers.push_back(power(GetX() + hgeCreate(HGE_VERSION)->Random_Float(-64, 64),
				GetY() + hgeCreate(HGE_VERSION)->Random_Float(-32, 32), LARGE));
		}

		remove = true;
	}
	if(remove)
		return;

	spriteangle += spriteangleinc;

	if(!controller->isEnabled())
		controller->enable(true);
}

void Enemy::Render()
{
	if(activeTime <= 0)
		return;
	anim->RenderEx(x, y, angle+spriteangle);
}
	
void Enemy::TakeDamage(float dmg)
{
	hp -= dmg;
}
	
void Enemy::IncrementActiveTime()
{	activeTime++;	}

void Enemy::UpdateCtrlBndryChck()
{	
	controller->setPosition(x, y);
	controller->setAngle(angle, 0);

	if(x < clipX1-50 || x > clipX2 + 50 || y < clipY1-50 || y > clipY2+50)
		remove = true;
}