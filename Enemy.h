#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "bsBulletScript.h"
#include "hge.h"
#include "hgeanim.h"

class Enemy;

typedef void(*EnemyUpdater)(class Enemy*);

class Enemy
{
protected:
	float radius, hp, maxhp, spriteangle, angle, speed;
	unsigned numSmallPower, numLargePower;
public:
	float lerpTimer;
	float x, y, tx, ty;
	EnemyUpdater func;
	float spriteangleinc;
protected:
	unsigned score;
	int enemySpriteType;
	int activeTime;
	bool remove;
	hgeAnimation* anim;		
	bs::Controller* controller;
public:
	Enemy(float x, float y, float speed, float angle, float hp, int activeTime, int enemySprite, const char * contrFilename, float spriteangleinc = 0, unsigned nsp = 0, unsigned nlp = 0, EnemyUpdater func = 0);
	virtual ~Enemy();

	float GetX() const;
	float GetY() const;
	float GetCurrentHP() const;
	float GetMaxHP() const;
	float GetAngle() const;
	float GetSpeed() const;
	float GetRadius() const;

	int GetActiveTime() const;
	bs::Controller* GetController() const;
	hgeAnimation* GetAnimation() const;
	bool IsExpired() const;

	void SetActiveTime(float x) {	activeTime = x;	}
	void SetPosition(float x, float y);
	void SetHP(float nhp);
	void SetAngle(float newa);
	void SetSpeed(float news);
	void SetController(const char* name);
	void SetExpired(bool expired);
	void IncrementActiveTime();
	void UpdateCtrlBndryChck();

	virtual void Update();
	virtual void Render();
	void TakeDamage(float dmg);
};

#endif