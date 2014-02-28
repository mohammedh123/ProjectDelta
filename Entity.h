#ifndef _ENTITY
#define _ENTITY

#include "HGEGlobals.h"
#include <string>
#include <vector>

class Entity
{
public:
	Entity();
	Entity(const Entity& rhs);
	Entity(float x, float y, const std::string& fname = "", float speed = 50.0f, float hp = 100.0f);
	virtual ~Entity();

	float getX() const;
	float getY() const;
	virtual float getDx(bool useSelfAngle = true, float _angle = 0, float dt = hgeCreate(HGE_VERSION)->Timer_GetDelta()) const;
	virtual float getDy(bool useSelfAngle = true, float _angle = 0, float dt = hgeCreate(HGE_VERSION)->Timer_GetDelta()) const;
	float getWidth() const;
	float getHeight() const;
	float getSpeed() const;
	hgeAnimation* getSprite() const;

	void setPosition(const hgeVector& c);
	void setX(float nx);
	void setY(float ny);
	void setSpeed(float ndx);
	void setSprite(std::string& spritename);

	virtual void Update() = 0;
	virtual void draw();
	virtual void move(float dx, float dy);
	void hpCheck();
	
	const Entity& operator=(const Entity& rhs);

	bool remove;
	float x,y, angle, spriteangle;
	float realSpeed, baseSpeed;
	int alpha;
	float hp, maxhp;
	float cx, cy;
	hgeAnimation* sprite;
private:
};

#endif