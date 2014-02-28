#include "BulletManufacturer.h"

BulletManufacturer* BulletManufacturer::_instance = 0;

BulletManufacturer::BulletManufacturer()
{}

BulletManufacturer::~BulletManufacturer()
{}

BulletManufacturer* BulletManufacturer::GetInstance()
{
	if(!_instance)
	{
		_instance = new BulletManufacturer();
	}

	return _instance;
} 
	
bulletInfo& BulletManufacturer::GetBulletInfo(const std::string& name)
{
	return bInfos[name];
}
	
void BulletManufacturer::ParseBulletScript(const char* filename)
{

}

void BulletManufacturer::AddPrototype(const std::string& name, hgeAnimation* sprite, float velX, float velY, float accelX, float accelY, float maxSpeedX, float maxSpeedY)
{
	bulletInfo t;
	t.accelX = accelX;
	t.accelY = accelY;
	t.maxSpeedX = maxSpeedX;
	t.maxSpeedY = maxSpeedY;
	t.sprite = sprite;
	t.velX = velX;
	t.velY = velY;
	bInfos[name] = t;
}

void BulletManufacturer::AddPrototype(const std::string& name, hgeAnimation* sprite, float speed, float angle)
{
	bulletInfo t;
	t.accelX = 0;
	t.accelY = 0;
	t.maxSpeedX = -1;
	t.maxSpeedY = -1;
	t.sprite = sprite;
	t.velX = cosf(angle)*speed;
	t.velY = sinf(angle)*speed;
	bInfos[name] = t;
}
