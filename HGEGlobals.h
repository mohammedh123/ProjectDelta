#include "hge.h"
#include "hgeguictrls.h"
#include "hgegui.h"
#include "hgeanim.h"
#include "hgefont.h"
#include "hgestrings.h"
#include "hgeparticle.h"
#include "hgeresource.h"
#include "bsBulletScript.h"
#include "BalancedPlayer.h"
#include "Player.h"
#include "Boss.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

#ifndef _GLOBALS
#define _GLOBALS
#define clamp(a,b,c) ((a) < (b) ? b : a > c ? c : a)
#define SQRT_2_2 0.70710678118654752440084436210485f
#define M_180_PI	57.295779513082320876798154814105f
#define M_PI_3	1.047197551196597746153f
#define M_PI_6	0.5235987755982988730765f
#define M_PI_180	0.017453292519943295769236906004f	// 1°
#define M_PI_90	0.034906585039886591538473815369f	// 2°
#define M_PI_60	0.052359877559829887307710723052f	// 3°
#define M_PI_45	0.069813170079773183076947630736f	// 4°
#define M_PI_18	0.17453292519943295769236906004f	// 10°
#define M_PI_12	0.26179938779914943653855361526f	// 15°
#define FRAME_TIME 0.016666666666666666666666666666667f
#define CENTERX_OF_AREA (SCREEN_WIDTH*0.5)

#define MAX_BULLETS 2000
#define MAX_CURVEDLASERS 80

#define LASER_EXPAND 0.1f

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct vec2
{
	vec2(float z, float f) : x(z), y(f) {}
	float x,y;
};

typedef void(*bulletFunc)(struct bs::UserTypeBase*,float,const float*);
typedef void(*EnemyUpdater)(class Enemy*);

class Game;
class Entity;
class Enemy;

struct scorepoint
{
	scorepoint(float x, float y) : pos(x, y), framesAlive(0) {}
	hgeVector pos;
	unsigned framesAlive;
};

struct GrimoireData
{
	GrimoireData() : func(0), controllerName(""), desc("") {}
	GrimoireData(EnemyUpdater func, const std::string& controllerName, const std::string& desc) : func(func), controllerName(controllerName), desc(desc) {}
	
	EnemyUpdater func;
	std::string controllerName, desc;
};

struct GrimoireKey
{
	GrimoireKey() : title("") {}
	GrimoireKey(char* title) : title(title) {}

	char* title;
};

	enum powertype
	{
		SMALL,
		LARGE
	};

struct power
{
	power(float x, float y, powertype type) : x(x), y(y), type(type), angle(0), vely(-3), framesInRadius(-1) 
	{
		x = max(x, 0);
		x = min(407, x);
	}

	void update()
	{
		angle = min(angle + M_PI_18, 2*M_PI);
		vely = min(vely + 0.15f, 1.5f);
		y += vely;
	}
	float x,y,angle,vely;
	powertype type;
	int framesInRadius;
};

struct projectile : public bs::UserTypeBase
{
	unsigned type;
	float x,y;
	float alpha,speed,angle,radius,anim;
	int blend;
};

struct bullet: public projectile
{
	bullet() {type = 0; target = 0; spriteangle = 0; func=0;}
	float spriteangle,
		damage;
	unsigned framesAlive;
	bool isFriendly, grazed;
	bulletFunc func;
	Enemy* target;
};

struct laser : public projectile
{
	laser() {type = 1; grazeCount = 0;}
	float length;
	unsigned framesAlive, grazeCount;
	int creationFrames;
};

struct curvedlaser : public projectile
{
	curvedlaser() {type=2;}

	std::list<vec2> points;
	unsigned framesAlive;
};

struct beam : public projectile
{
	beam() {type = 3;}

	float initRad;
	unsigned grazeCount, framesAlive;
	int chargetime, lifetime;
	float length, maxlength;
};

struct BIRTH_ANIM
{
	BIRTH_ANIM() : a(255), adecay(0), bul(NULL), target(0), lifetime(0) {}
	hgeAnimation* anim[2];
	float scale, decay;
	float x,y, angle;
	Enemy* target;
	unsigned a,r,g,b,adecay,lifetime;
	projectile* bul;
};


struct LEVEL
{	
	beam beams[MAX_BULLETS/4];
	bullet bullets[MAX_BULLETS];
	laser lasers[MAX_BULLETS];
	curvedlaser curvedlasers[MAX_CURVEDLASERS];
	std::vector<BIRTH_ANIM> anims;
	std::vector<Enemy*> enemies;
	std::vector<power> powers;
	std::vector<scorepoint> scorepoints;
	unsigned cBullet;
	unsigned cLaser;
	unsigned cCurvedLaser;
	unsigned cBeam;

	LEVEL() : cBullet(0), cLaser(0), cCurvedLaser(0), cBeam(0)
	{
		for(size_t i = 0; i < MAX_BULLETS/4; i++)
		{
			beams[i].anim = -1;
			beams[i].framesAlive = 0;
			beams[i].length = 0;
		}
		for(size_t i = 0; i < MAX_BULLETS; i++)
		{
			bullets[i].anim = -1;
			bullets[i].framesAlive = 0;
			bullets[i].alpha = 255;
		}

		for(size_t i = 0; i < MAX_BULLETS; i++)
		{
			lasers[i].anim = -1;
			lasers[i].framesAlive = 0;
			lasers[i].length = 0;
		}

		for(size_t i = 0; i < MAX_CURVEDLASERS; i++)
		{
			curvedlasers[i].anim = -1;
			curvedlasers[i].framesAlive = 0;
		}
	}
	~LEVEL();
};

extern Game* gMain;
extern hgeResourceManager* res;	
extern hgeSprite* emptySprite;
extern std::ostringstream oss;
extern float clipX1, clipX2, clipY1, clipY2;
extern bs::Machine* machine;
extern Player* player;
extern std::map<GrimoireKey, GrimoireData> grimoire;

extern void initializeVariables();
extern void UpdateCamera();

extern bool circleIntersect(float cx1, float cy1, float radi1, float cx2, float cy2, float radi2);
extern bool circleLineIntersect(const hgeVector& c, float r, const hgeVector& p1, const hgeVector& p2);
extern bool circleRectIntersect(const hgeVector& c, float r, const hgeVector& tl, const hgeVector& tr, const hgeVector& br, const hgeVector& bl);
extern bool lineLineIntersect(const float x0, const float y0, const float x1, const float y1, const float x2, const float y2, const float x3, const float y3);
extern bool pointInRect(float x, float y, float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy);

extern hgeVector BezierCurve(const hgeVector& a, const hgeVector& b, const hgeVector& c, float t, bool useSpeed=false, float speed=0);
extern hgeVector LERP(const hgeVector& cp1, const hgeVector& cp2, float t, bool useSpeed=false, float speed=0);
extern void ScoreLERP();

extern bullet& CreateBullet(float x, float y, float speed, float angle, float radius, int anim, bool isFriendly); //uniform linear
extern bullet& CreateBullet2(float x, float y, float speed, float angle, float acceleration, float accelerationAngle, float velocityminmax, int anim, float radius); //uniform accelerated
extern bullet& CreateBulletT(float x, float y, float speed, float tx, float ty, float radius, int anim, bool isFriendly);

extern laser& CreateLaser1(float x, float y, float speed, float angle, float radius, unsigned cFrames, int anim); //uniform linear
extern laser& CreateLaser2(float x, float y, float speed, float angle, float radius, unsigned cFrames, float acceleration, float velocityminmax, int anim); //uniform accelerated
extern laser& CreateLaserT(float x, float y, float speed, float tx, float ty, float radius, unsigned cFrames, int anim);

extern curvedlaser& CreateCurvedLaser(float x, float y, float speed, float angle, float radius, int anim); //uniform linear
extern curvedlaser& CreateCurvedLaserT(float x, float y, float speed, float tx, float ty, float radius, int anim); //uniform linear

extern beam& CreateBeam(float x, float y, float speed, float angle, float radius, float maxlength, unsigned chargetime, unsigned lifetime, int anim);

extern BIRTH_ANIM& CreateSpawnEffect(float x, float y, float angle, int shotType);
extern inline bool RectRectIntersect(const hgeRect& rect1, const hgeRect& rect2);

extern void GetAvailableSlotBullet();
extern void GetAvailableSlotLaser();
extern void GetAvailableSlotCurvedLaser();
extern void GetAvailableSlotBeam();
extern float FastSqrt(float f);

extern int GetAngleTowardsPlayer(bs::ScriptState& state);
extern int Cos(bs::ScriptState& state);
extern int Sin(bs::ScriptState& state);
extern int Random(bs::ScriptState& state);
extern int RandomInt(bs::ScriptState& state);

extern bs::UserTypeBase* emitBullet(float x, float y, float angle, const float* args, void* user);
extern bs::UserTypeBase* emitBulletP(float x, float y, float angle, const float* args, void* user);
extern bs::UserTypeBase* emitBulletT(float x, float y, float angle, const float* args, void* user);

extern bs::UserTypeBase* emitLaser(float x, float y, float angle, const float* args, void* user);
extern bs::UserTypeBase* emitLaserT(float x, float y, float angle, const float* args, void* user);

extern bs::UserTypeBase* emitCurvedLaser(float x, float y, float angle, const float* args, void* user);
extern bs::UserTypeBase* emitCurvedLaserT(float x, float y, float angle, const float* args, void* user);

extern bs::UserTypeBase* emitBeam(float x, float y, float angle, const float* args, void* user);

extern void killBullet(bs::UserTypeBase *userType, void* user);

extern void setX(bs::UserTypeBase* userType, float value);
extern void setY(bs::UserTypeBase* userType, float value);
extern void setAngle(bs::UserTypeBase* userType, float value);
extern void setSpeed(bs::UserTypeBase* userType, float value);
extern void setAlpha(bs::UserTypeBase* userType, float value);
extern void setSprite(bs::UserTypeBase* userType, float value);
extern void setRadius(bs::UserTypeBase* userType, float value);
extern float getX(bs::UserTypeBase* userType);
extern float getY(bs::UserTypeBase* userType);
extern float getAngle(bs::UserTypeBase* userType);
extern float getSpeed(bs::UserTypeBase* userType);
extern float getAlpha(bs::UserTypeBase* userType);
extern float getSprite(bs::UserTypeBase* userType);
extern float getRadius(bs::UserTypeBase* userType);


extern void IntLERP(int& start, const int& end, float t);
extern void FloatLERP(float& start, const float& end, float t);
extern float FloatLERPEx(float start, const float& end, float t);

extern void ConvertAllToPoints();

extern bool operator <(const GrimoireKey& lhs, const GrimoireKey& rhs);
extern bool operator ==(const GrimoireKey& lhs, const GrimoireKey& rhs);
#endif