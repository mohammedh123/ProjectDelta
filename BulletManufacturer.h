#ifndef _BMANUF
#define _BMANUF

#include "HGEGlobals.h"
#include <map>

class BulletManufacturer
{
	static BulletManufacturer* _instance;
	std::map<const std::string, bulletInfo> bInfos;
public:	
	BulletManufacturer();
	~BulletManufacturer();

	void ParseBulletScript(const char* filename);
	bulletInfo& GetBulletInfo(const std::string& name);
	void AddPrototype(const std::string& name, hgeAnimation* sprite, float velX, float velY, float accelX, float accelY, float maxSpeedX, float maxSpeedY);
	void AddPrototype(const std::string& name, hgeAnimation* sprite, float speed, float angle);

	static BulletManufacturer* GetInstance();
};

#endif