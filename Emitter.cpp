#include "Emitter.h"
#include "BulletFunctions.h"

Emitter::Emitter() : fCounter(0), delay(0), x(0), y(0), active(true), framesAlive(0)
{}

Emitter::Emitter(float x, float y, unsigned delayBetweenShots, bulletInfo bInfo) : fCounter(0), delay(delayBetweenShots), x(x), y(y), active(true), bInfo(bInfo), framesAlive(0)
{
}

Emitter::~Emitter()
{	
	for(size_t i = 0; i < affectors.size(); i++)
		delete affectors[i];
}

void Emitter::Update()
{
	if(active)
	{
		framesAlive++;
		fCounter++;
	}
}
	
Emitter* Emitter::AddAffector(Affector* d)
{
	affectors.push_back(d);
	return this;
}