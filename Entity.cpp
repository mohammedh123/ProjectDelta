#include "Entity.h"
#include "Game.h"
#include <fstream>
#include <sstream>

Entity::Entity() : x(0), y(0), realSpeed(0), sprite(NULL), remove(false), angle(0), spriteangle(0), alpha(255)
{
}

Entity::Entity(const Entity& rhs)
{
	x = rhs.x;
	y = rhs.y;
	sprite = rhs.sprite;
}

Entity::Entity(float x, float y, const std::string& fname, float speed, float hp) : x(x), y(y), remove(false), angle(0), spriteangle(0), realSpeed(speed), baseSpeed(speed), alpha(255), hp(hp), maxhp(hp), cx(x), cy(y)
{
	sprite = NULL;
	if(fname != "")
	{
		sprite = new hgeAnimation(*(res->GetAnimation(fname.c_str())));
		sprite->Play();
		//Update();
	}
}

Entity::~Entity()
{
	delete sprite;
	sprite = NULL;
}

const Entity& Entity::operator=(const Entity& rhs)
{
	if(&rhs != this || &rhs != NULL)
	{
		x = rhs.x;
		y = rhs.y;
		sprite = rhs.sprite;
	}

	return *this;
}

float Entity::getX() const
{	return x;	}

float Entity::getY() const
{	return y;	}

float Entity::getDx(bool useSelfAngle, float _angle, float dt) const
{	return useSelfAngle ? cosf(angle) * realSpeed * (1.0f/60.0f) : cosf(_angle) * realSpeed * (1.0f/60.0f);	}

float Entity::getDy(bool useSelfAngle, float _angle, float dt) const
{	return useSelfAngle ? sinf(angle) * realSpeed * (1.0f/60.0f) : sinf(_angle) * realSpeed * (1.0f/60.0f);	}

float Entity::getSpeed() const
{	return realSpeed;	}

float Entity::getWidth() const
{	return sprite->GetWidth();	}

float Entity::getHeight() const
{	return sprite->GetHeight();	}

hgeAnimation* Entity::getSprite() const
{	return sprite;	}

void Entity::setPosition(const hgeVector& c)
{	x = c.x; y = c.y;	}

void Entity::setX(float nx)
{	x = nx;	}

void Entity::setY(float ny)
{	y = ny;	}

void Entity::setSpeed(float ndx)
{	realSpeed = ndx;	}

void Entity::setSprite(std::string& spritename)
{
	if(spritename == "")
		sprite = NULL;
	else
	{
		delete sprite;
		//int t = sprite->GetFrame();
		sprite = new hgeAnimation(*(res->GetAnimation(spritename.c_str())));
		//sprite->SetFrame(t);
		sprite->Play();
	}
}

void Entity::draw()
{
	//Update();
	if(sprite)
		if(getX() >= 0 || getX() < SCREEN_WIDTH)
			sprite->RenderEx(getX(), getY(), spriteangle+angle);
}

void Entity::move(float dx, float dy)
{
	setX(getX() + dx);
	setY(getY() + dy);
}

void Entity::Update()
{
}

void Entity::hpCheck()
{	
}