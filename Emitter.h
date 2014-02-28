#ifndef _EMITTER
#define _EMITTER

#include "Affector.h"
#include "HGEGlobals.h"
#include <vector>
 
class Emitter
{
	unsigned fCounter, delay, framesAlive;
	float x, y;
	bulletInfo bInfo;
	bool active;

	std::vector<Affector *> affectors;
public:
	Emitter();
	~Emitter();
	Emitter(float x, float y, unsigned delayBetweenShots = 0, bulletInfo bInfo = bulletInfo());

	Emitter* AddAffector(Affector* d);
	void Update();
};

#endif
//60 frames = 1 second