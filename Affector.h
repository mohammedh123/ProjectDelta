#ifndef _AFFECTOR
#define _AFFECTOR

#include "HGEGlobals.h"
 
struct Affector
{
	virtual void affect(bullet& d)
	{
	}
};

struct GravityAffector : public Affector
{
	void affect(bullet& d)
	{
		d.maxSpeed = 10;
		d.acceleration = 0.012f;
		d.accelerationDirection = M_PI_2;
	}
};

struct WeirdAffector : public Affector
{
	void affect(bullet& d)
	{
		//d.accelX = 0.1f*cosf(10*(atan2(d.velY, d.velX) + d.framesAlive * M_PI_12));
		//d.accelY = 0.1f*sinf(10*(atan2(d.velY, d.velX) + d.framesAlive * M_PI_12));
	}
};



#endif