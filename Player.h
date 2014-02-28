#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "bsBulletScript.h"

class Player
{
protected:
	float x,y, baseSpeed, slowSpeed, speed;
	bool shooting;
	bs::Controller* controller;
public:
	unsigned power;
	bool shifting; 
	Player(float x, float y, char* ctrlname = "PlayerController");
	~Player();

	float GetX() const;
	float GetY() const;
	float GetSpeed() const;

	void SetPosition(float x, float y);
	void SetController(const char* name);
	void SetSpeed(float spd);

	virtual void Update();
	virtual void Render();
};

#endif