#include "HGEGlobals.h"
#include "Game.h"
#include "BulletFunctions.h"
#include "BulletSpriteTypes.h"
#include "Enemy.h"

Game* gMain = 0;
hgeResourceManager* res = 0;
hgeSprite* emptySprite = 0;
float CAMERA_X = 0, CAMERA_Y = 0;
float clipX1 = 140, clipX2 = 500, clipY1=0, clipY2=SCREEN_HEIGHT;
std::ostringstream oss(std::ostringstream::out);
bs::Machine* machine = 0;
Player* player = 0;
std::map<GrimoireKey, GrimoireData> grimoire;

void initializeVariables()
{
	playerShotSprites[STANDARD_GREEN] = res->GetAnimation("playershot");
	playerShotSprites[BALANCED_OTHER] = res->GetAnimation("balancedplayerother");
	playerShotSprites[BALANCED_BIG] = res->GetAnimation("balancedbig");
	//enemy sprites
	{		
		enemySprites[EST_EN1RED]		= res->GetAnimation("en1red");
		enemySprites[EST_EN1GREEN]		= res->GetAnimation("en1green");
		enemySprites[EST_EN1BLUE]		= res->GetAnimation("en1blue");
		enemySprites[EST_EN1YELLOW]		= res->GetAnimation("en1yellow");
		enemySprites[EST_EN1CYAN]		= res->GetAnimation("en1cyan");
		enemySprites[EST_EN1MAGENTA]	= res->GetAnimation("en1magenta");
		enemySprites[EST_EN1WHITE]		= res->GetAnimation("en1white");
		enemySprites[EST_EN1BLACK]		= res->GetAnimation("en1black");

		enemySprites[EST_BOSS1]		= res->GetAnimation("boss1");
	}

	//proj sprites
	{
		for(int i = 0; i < 8; i++)
		{
			beamSprites[i] = new hgeSprite(res->GetTexture("beams"), 0, 32*i, 128, 32);
			beamSprites[i]->SetHotSpot(0, 16);
		}
		
		for(int i = 0; i < NUM_BEAM_SPRITES; i++)
			beamRadiiInv[i] = 2/beamSprites[i]->GetHeight();


		for(int i = S01*NUM_COLORS; i < S01*NUM_COLORS + NUM_COLORS; i++)
			bulletSprites[i] = new hgeAnimation(res->GetTexture("bullets"), 1, 1, 32*(i%NUM_COLORS), 0, 32, 16);
		for(int i = S02*NUM_COLORS; i < S02*NUM_COLORS + NUM_COLORS; i++)
			bulletSprites[i] = new hgeAnimation(res->GetTexture("bullets"), 1, 1, 64*(i%NUM_COLORS), 16, 64, 64);
		for(int i = S03*NUM_COLORS; i < S03*NUM_COLORS + NUM_COLORS; i++)
			bulletSprites[i] = new hgeAnimation(res->GetTexture("bullets"), 1, 1, 32*(i%NUM_COLORS), 80, 32, 32);
		for(int i = S04*NUM_COLORS; i < S04*NUM_COLORS + NUM_COLORS; i++)
		{
			int color = i % NUM_COLORS;
			bulletSprites[i] = new hgeAnimation(res->GetTexture("bullets"), 2, 10, 48*2*(color / 2), 112+48*(color%2), 48, 48);		
		}
		for(int i = S05*NUM_COLORS; i < S05*NUM_COLORS + NUM_COLORS; i++)
			bulletSprites[i] = new hgeAnimation(res->GetTexture("bullets"), 1, 1, 32*(i%NUM_COLORS), 208, 32, 32);
		for(int i = S06*NUM_COLORS; i < S06*NUM_COLORS + NUM_COLORS; i++)
			bulletSprites[i] = new hgeAnimation(res->GetTexture("bullets"), 1, 1, 32*(i%NUM_COLORS), 240, 32, 32);
		for(int i = S07*NUM_COLORS; i < S07*NUM_COLORS + NUM_COLORS; i++)
		{
			if(i % NUM_COLORS == 0)
				bulletSprites[i] = new hgeAnimation(res->GetTexture("bullets"), 1, 1, 0, 272, 128, 128);
			else
				bulletSprites[i] = 0;
		}
		for(int i = S08*NUM_COLORS; i < S08*NUM_COLORS + NUM_COLORS; i++)
			bulletSprites[i] = new hgeAnimation(res->GetTexture("bullets"), 1, 1, 64*(i%NUM_COLORS), 400, 64, 32);
		
		for(int i = 0; i < NUM_BULLET_SPRITES*NUM_COLORS; i++)
		{
			if(bulletSprites[i])
			{
				bulletSprites[i]->SetHotSpot(bulletSprites[i]->GetWidth()*0.5, bulletSprites[i]->GetHeight()*0.5);
				bulletSprites[i]->Play();
			}
		}
	}

	//bullet radii
	{
		spriteHitboxRadii[S01] = 4;
		spriteHitboxRadii[S02] = 8;
		spriteHitboxRadii[S03] = 8;
		spriteHitboxRadii[S04] = 10.5;
		spriteHitboxRadii[S05] = 8;
		spriteHitboxRadii[S06] = 12;
		spriteHitboxRadii[S07] = 48;
		spriteHitboxRadii[S08] = 8;

		for(int i = 0; i < NUM_BULLET_SPRITES; i++)
			spriteRadiiInv[i] = 1.0f/spriteHitboxRadii[i];


		spriteDefaultScales[S01] = 0.5;
		spriteDefaultScales[S02] = 1;
		spriteDefaultScales[S03] = 1;
		spriteDefaultScales[S04] = 2.0f/3.0f;
		spriteDefaultScales[S05] = 0.5;
		spriteDefaultScales[S06] = 1;
		spriteDefaultScales[S07] = 1;
		spriteDefaultScales[S08] = 2.0f/3.0f;

		spriteBlendModes[S01] = BLEND_DEFAULT;
		spriteBlendModes[S02] = BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE;
		spriteBlendModes[S03] = BLEND_DEFAULT;
		spriteBlendModes[S04] = BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE;
		spriteBlendModes[S05] = BLEND_DEFAULT;
		spriteBlendModes[S06] = BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE;
		spriteBlendModes[S07] = BLEND_DEFAULT;
		spriteBlendModes[S08] = BLEND_DEFAULT;


	}

	//birth effect sprites
	{
		spawnEffectSprites[S01][0]				= res->GetAnimation("birth03_1");
		spawnEffectSprites[S01][1]				= res->GetAnimation("birth03_2");

		spawnEffectSprites[S02][0]				= res->GetAnimation("birth03_1");
		spawnEffectSprites[S02][1]				= res->GetAnimation("birth03_2");

		spawnEffectSprites[S03][0]				= res->GetAnimation("birth03_1");
		spawnEffectSprites[S03][1]				= res->GetAnimation("birth03_2");

		spawnEffectSprites[S04][0]				= res->GetAnimation("birth01_1");
		spawnEffectSprites[S04][1]				= res->GetAnimation("birth01_2");

		spawnEffectSprites[S05][0]				= res->GetAnimation("birth03_1");
		spawnEffectSprites[S05][1]				= res->GetAnimation("birth03_2");

		spawnEffectSprites[S06][0]				= res->GetAnimation("birth03_1");
		spawnEffectSprites[S06][1]				= res->GetAnimation("birth03_2");

		spawnEffectSprites[S07][0]				= res->GetAnimation("birth01_1");
		spawnEffectSprites[S07][1]				= res->GetAnimation("birth01_2");

		spawnEffectSprites[S08][0]				= res->GetAnimation("birth03_1");
		spawnEffectSprites[S08][1]				= res->GetAnimation("birth03_2");	
	}

	gMain = new Game();
	machine = new bs::Machine;
	machine->createType("bullet");
	machine->createType("laser");
	machine->createType("curvedlaser");
	machine->createType("beam");
	//x y speed angle damage radius sprite
	machine->registerEmitFunction("bullet", "emitp", 7, emitBulletP);
	//x y speed angle radius sprite intensity
	machine->registerEmitFunction("bullet", "emit", 7, emitBullet);
	//x y speed tx ty radius sprite intensity
	machine->registerEmitFunction("bullet", "emitt", 8, emitBulletT);

	//x y speed angle rad cFrames spr
	machine->registerEmitFunction("laser", "emitl", 7, emitLaser);
	//x y speed tx ty rad cFrames spr
	machine->registerEmitFunction("laser", "emitlt", 8, emitLaserT);

	//x y speed angle rad spr
	machine->registerEmitFunction("curvedlaser", "emitcl", 6, emitCurvedLaser);
	//x y speed tx ty rad spr
	machine->registerEmitFunction("curvedlaser", "emitclt", 7, emitCurvedLaserT);

	//x y speed angle radius maxlength chargetime lifetime spr
	machine->registerEmitFunction("beam", "emitb", 9, emitBeam);

	machine->setDieFunction("bullet", killBullet);	
	machine->setDieFunction("laser", killBullet);	
	machine->setDieFunction("curvedlaser", killBullet);	
	machine->setDieFunction("beam", killBullet);	

	machine->setAnchorX("bullet", setX, getX);
	machine->setAnchorY("bullet", setY, getY);
	machine->setAnchorAngle("bullet", setAngle, getAngle);
	machine->registerProperty("bullet", "speed", setSpeed, getSpeed);
	machine->registerProperty("bullet", "alpha", setAlpha, getAlpha);

	machine->setAnchorX("laser", setX, getX);
	machine->setAnchorY("laser", setY, getY);
	machine->setAnchorAngle("laser", setAngle, getAngle);
	machine->registerProperty("laser", "speed", setSpeed, getSpeed);
	machine->registerProperty("laser", "alpha", setAlpha, getAlpha);

	machine->setAnchorX("curvedlaser", setX, getX);
	machine->setAnchorY("curvedlaser", setY, getY);
	machine->setAnchorAngle("curvedlaser", setAngle, getAngle);
	machine->registerProperty("curvedlaser", "speed", setSpeed, getSpeed);
	machine->registerProperty("curvedlaser", "alpha", setAlpha, getAlpha);

	machine->setAnchorX("beam", setX, getX);
	machine->setAnchorY("beam", setY, getY);
	machine->setAnchorAngle("beam", setAngle, getAngle);
	machine->registerProperty("beam", "speed", setSpeed, getSpeed);
	machine->registerProperty("beam", "alpha", setAlpha, getAlpha);

	machine->registerProperty("bullet", "sprite", setSprite, getSprite);
	machine->registerProperty("laser", "sprite", setSprite, getSprite);
	machine->registerProperty("curvedlaser", "sprite", setSprite, getSprite);
	machine->registerProperty("beam", "sprite", setSprite, getSprite);

	machine->registerProperty("bullet", "radius", setRadius, getRadius);
	machine->registerProperty("laser", "radius", setRadius, getRadius);
	machine->registerProperty("curvedlaser", "radius", setRadius, getRadius);
	machine->registerProperty("beam", "radius", setRadius, getRadius);

	machine->registerAffector("bullet", "AngleIncrement", AngleIncrement);
	machine->registerAffector("bullet", "Homing", Homing);
	machine->registerAffector("laser", "Homing", Homing);
	machine->registerAffector("curvedlaser", "Homing", Homing);
	machine->registerAffector("bullet", "Spin", Spin);
	machine->registerAffector("bullet", "BounceOffWalls", BounceOffWalls);
	machine->registerAffector("laser", "BounceOffWalls", BounceOffWalls);
	machine->registerAffector("curvedlaser", "BounceOffWalls", BounceOffWalls);
	machine->registerAffector("bullet", "PlayerHoming", PlayerHoming);
	machine->registerAffector("bullet", "SetSpeedAfterFrames", SetSpeedAfterFrames);
	machine->registerAffector("bullet", "OrbitAroundCenter", OrbitAroundCenter);
	//native functions
	machine->registerNativeFunction("GetAngleTowardsPlayer", true, 2, GetAngleTowardsPlayer);
	machine->registerNativeFunction("Cos", true, 1, Cos);
	machine->registerNativeFunction("Sin", true, 1, Sin);
	machine->registerNativeFunction("Random", true, 2, Random);
	machine->registerNativeFunction("RandomInt", true, 2, RandomInt);

	//globals
	machine->registerGlobalVariable("SCREEN_WIDTH", true, SCREEN_WIDTH);
	machine->registerGlobalVariable("SCREEN_HEIGHT", true, SCREEN_HEIGHT);
	machine->registerGlobalVariable("PI", true, M_PI);
	machine->registerGlobalVariable("TRUE", true, 1);
	machine->registerGlobalVariable("FALSE", true, -1);

	//bullet sprite types

	int coun = 0;
	for(size_t i = 1; i <= NUM_BULLET_SPRITES; i++)
	{
		for(int j = 0; j < NUM_COLORS; j++)
		{
			oss.str("");
			switch(j)
			{
			case 0:	oss << "RED" << i;break;
			case 1:	oss << "GREEN" << i;break;
			case 2: oss << "BLUE" << i;break;
			case 3:	oss << "YELLOW" << i;break;
			case 4: oss << "CYAN" << i;break;
			case 5:	oss << "MAGENTA" << i;break;
			case 6: oss << "ORANGE" << i;break;
			case 7:	oss << "BLACK" << i;break;
			}

			machine->registerGlobalVariable(oss.str(), true, coun);

			coun++;
		}
	}

	machine->registerGlobalVariable("STANDARD_GREEN", true, STANDARD_GREEN);
	machine->registerGlobalVariable("BALANCED_OTHER", true, BALANCED_OTHER);
	machine->registerGlobalVariable("BALANCED_BIG", true, BALANCED_BIG);

	machine->registerGlobalVariable("Player_X", true, 0);
	machine->registerGlobalVariable("Player_Y", true, 0);

	FILE* fp = fopen("enemies.pd", "rb");
	fseek(fp, 0, SEEK_END);
	size_t fileLength = ftell(fp);
	unsigned char* buffer = new unsigned char[fileLength];

	fseek(fp, 0, SEEK_SET);
	fread(buffer, fileLength, 1, fp);
	fclose(fp);

	if (machine->compileScript(buffer, fileLength) != 0)
	{
		std::cout << "Could not compile " << fp->_file << std::endl;
		const bs::Log& _log = machine->getLog();

		std::string msg = _log.getFirstEntry();
		while (msg != bs::Log::END)
		{
			std::cout << msg << std::endl;
			msg = _log.getNextEntry();
		}
		delete[] buffer;
		return;
	}
	delete[] buffer;
}

inline void UpdateCamera()
{
	float x,y,w,h;
	res->GetDistortionMesh("testbg")->GetTextureRect(&x, &y, &w, &h);
	y += 0.5f;
	y = fmod(y, 512);
	res->GetDistortionMesh("testbg")->SetTextureRect(x, y, w, h);
}

bool circleIntersect(float cx1, float cy1, float radi1, float cx2, float cy2, float radi2)
{
	return (radi1 + radi2) * (radi1 + radi2) > ((cx1 - cx2) * (cx1 - cx2)) + ((cy1 - cy2) *	(cy1 - cy2));
}

bool circleLineIntersect(const hgeVector& c, float r, const hgeVector& p1, const hgeVector& p2)
{
	hgeVector dir = p2 - p1;
	hgeVector diff = c - p1;
	float proj = diff.Dot(&(dir/dir.Length()));
	hgeVector projv = proj * (dir/dir.Length());
	float t = diff.Dot(&dir) / dir.Dot(&dir);
	if (t < 0.0f)
		t = 0.0f;
	if (t > 1.0f)
		t = 1.0f;
	hgeVector closest = p1 + t * dir;
	hgeVector realclos = dir + projv;
	hgeVector d = c - closest;
	hgeVector reald = c - realclos;

	float distsqr = d.Length() * d.Length();
	return distsqr <= r * r;
	//return reald.Length() < r;
}

bool circleRectIntersect(const hgeVector& c, float r, const hgeVector& tl, const hgeVector& tr, const hgeVector& br, const hgeVector& bl)
{
	return pointInRect(c.x, c.y, tl.x, tl.y, tr.x, tr.y, br.x, br.y, bl.x, bl.y) 
		|| circleLineIntersect(c, r, tl, tr) 
		|| circleLineIntersect(c, r, tr, br)
		|| circleLineIntersect(c, r, br, bl)
		|| circleLineIntersect(c, r, bl, tl);

	//float closestX = clamp(c.x, r1.x1, r1.x2);
	//float closestY = clamp(c.y, r1.x1, r1.x2);

	//// Calculate the distance between the circle's center and this closest point
	//float distanceX = c.x - closestX;
	//float distanceY = c.y - closestY;

	//// If the distance is less than the circle's radius, an intersection occurs
	//float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
	//return distanceSquared < (r * r);

}

bool lineLineIntersect(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4)
{
	float denom = (y4 - y3)*(x2-x1) - (x4-x3)*(y2-y1);
	float ua = ((x4-x3)*(y1-y3)-(y4-y3)*(x1-x3))/denom;
	float ub = ((x2-x1)*(y1-y3)-(y2-y1)*(x1-x3))/denom;

	return ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1;
}

bool pointInRect(float x, float y, float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy)
{
	float ex,ey,fx,fy;

	ex=bx-ax; ey=by-ay;
	fx=dx-ax; fy=dy-ay;

	if ((x-ax)*ex+(y-ay)*ey<0.0) return false;
	if ((x-bx)*ex+(y-by)*ey>0.0) return false;
	if ((x-ax)*fx+(y-ay)*fy<0.0) return false;
	if ((x-dx)*fx+(y-dy)*fy>0.0) return false;

	return true;
}

hgeVector BezierCurve(const hgeVector& a, const hgeVector& b, const hgeVector& c, float t, bool useSpeed, float speed)
{
	return LERP(LERP(a,b,t, useSpeed,speed), LERP(b,c,t, useSpeed,speed),  t, useSpeed,speed);
}

hgeVector LERP(const hgeVector& cp1, const hgeVector& cp2, float t, bool useSpeed, float speed)
{ 
	hgeVector dir = cp2-cp1;
	//if(useSpeed)
	//	t = t/(speed/(cp2-cp1).Length());
	return cp1 + t * dir;
}

bullet& CreateBullet(float x, float y, float speed, float angle, float radius, int anim, bool isFriendly)
{
	GetAvailableSlotBullet();
	bullet &t = gMain->currentLevel->bullets[gMain->currentLevel->cBullet];

	t.func = 0;
	t.spriteangle = 0;
	t.alpha = 255;
	t.grazed = false;
	t.framesAlive = 0;
	t.x = x;
	t.y = y;
	t.angle = angle;
	t.speed = speed;
	t.isFriendly = isFriendly;
	t.anim = anim;
	t.radius = radius;

	return t;
}

bullet& CreateBulletT(float x, float y, float speed, float tx, float ty, float radius, int anim, bool isFriendly)
{
	GetAvailableSlotBullet();
	bullet &t = gMain->currentLevel->bullets[gMain->currentLevel->cBullet];

	t.spriteangle = 0;
	t.alpha = 255;
	t.grazed = false;
	t.framesAlive = 0;
	t.x = x;
	t.y = y;
	t.angle = atan2(ty-y,tx-x);
	t.speed = speed;
	t.isFriendly = isFriendly;
	t.anim = anim;
	t.radius = radius;

	return t;
}

laser& CreateLaser1(float x, float y, float speed, float angle, float radius, unsigned cFrames, int anim) //uniform linear
{
	GetAvailableSlotLaser();
	laser &t = gMain->currentLevel->lasers[gMain->currentLevel->cLaser];

	t.grazeCount = 0;
	t.length = 0;
	t.radius = radius;;
	t.creationFrames = cFrames;
	t.framesAlive = 0;
	t.x = x;
	t.y = y;
	t.angle = angle;
	t.speed = speed;
	t.anim = anim;

	return t;
}

laser& CreateLaserT(float x, float y, float speed, float tx, float ty, float radius, unsigned cFrames, int anim)
{
	GetAvailableSlotLaser();
	laser &t = gMain->currentLevel->lasers[gMain->currentLevel->cLaser];

	t.length = 0;
	t.radius = radius;;
	t.creationFrames = cFrames;
	t.framesAlive = 0;
	t.x = x;
	t.grazeCount = 0;
	t.y = y;
	t.angle = atan2(ty-y,tx-x);
	t.speed = speed;
	t.anim = anim;

	return t;
}

curvedlaser& CreateCurvedLaser(float x, float y, float speed, float angle, float radius, int anim) //uniform linear
{
	GetAvailableSlotCurvedLaser();
	curvedlaser &t = gMain->currentLevel->curvedlasers[gMain->currentLevel->cCurvedLaser];

	t.points.clear();
	t.radius = radius;;
	t.framesAlive = 0;
	t.x = x;
	t.y = y;
	t.angle = angle;
	t.speed = speed;
	t.anim = anim;

	return t;
}

curvedlaser& CreateCurvedLaserT(float x, float y, float speed, float tx, float ty, float radius, int anim)
{
	GetAvailableSlotCurvedLaser();
	curvedlaser &t = gMain->currentLevel->curvedlasers[gMain->currentLevel->cCurvedLaser];

	t.points.clear();
	t.radius = radius;;
	t.framesAlive = 0;
	t.x = x;
	t.y = y;
	t.angle = atan2(ty-y,tx-x);
	t.speed = speed;
	t.anim = anim;

	return t;
}


//x y speed angle radius maxlength chargetime lifetime spr
beam& CreateBeam(float x, float y, float speed, float angle, float radius, float maxlength, unsigned chargetime, unsigned lifetime, int anim)
{
	GetAvailableSlotBeam();
	beam &t = gMain->currentLevel->beams[gMain->currentLevel->cBeam];

	t.radius = 0;
	t.framesAlive = 0;
	t.x = x;
	t.y = y;
	t.initRad = radius;
	t.chargetime = chargetime;
	t.grazeCount = 0;
	t.length = 0;
	t.maxlength = maxlength;
	t.lifetime = lifetime;
	t.angle = angle;
	t.speed = speed;
	t.anim = anim;

	return t;
}

LEVEL::~LEVEL()
{
	for(size_t i = 0; i < enemies.size(); i++)
		delete enemies[i];
	enemies.clear();
}

void GetAvailableSlotBullet()
{
	int tCounter = 0;
	while(gMain->currentLevel->bullets[gMain->currentLevel->cBullet].anim >= 0 && tCounter < MAX_BULLETS)
	{	
		gMain->currentLevel->cBullet++; tCounter++;	
		if(gMain->currentLevel->cBullet >= MAX_BULLETS)
			gMain->currentLevel->cBullet -= MAX_BULLETS;
	}
	if(tCounter >= MAX_BULLETS)
		gMain->currentLevel->cBullet++;
	if(gMain->currentLevel->cBullet >= MAX_BULLETS)
		gMain->currentLevel->cBullet -= MAX_BULLETS;
}

void GetAvailableSlotLaser()
{
	int tCounter = 0;
	while(gMain->currentLevel->lasers[gMain->currentLevel->cLaser].anim >= 0 && tCounter < MAX_BULLETS)
	{	
		gMain->currentLevel->cLaser++; tCounter++;	
		if(gMain->currentLevel->cLaser >= MAX_BULLETS)
			gMain->currentLevel->cLaser -= MAX_BULLETS;
	}
	if(tCounter >= MAX_BULLETS)
		gMain->currentLevel->cLaser++;
	if(gMain->currentLevel->cLaser >= MAX_BULLETS)
		gMain->currentLevel->cLaser -= MAX_BULLETS;
}

void GetAvailableSlotCurvedLaser()
{
	int tCounter = 0;
	while(gMain->currentLevel->curvedlasers[gMain->currentLevel->cCurvedLaser].anim >= 0 && tCounter < MAX_CURVEDLASERS)
	{	
		gMain->currentLevel->cCurvedLaser++; tCounter++;	
		if(gMain->currentLevel->cCurvedLaser >= MAX_CURVEDLASERS)
			gMain->currentLevel->cCurvedLaser -= MAX_CURVEDLASERS;
	}
	if(tCounter >= MAX_CURVEDLASERS)
		gMain->currentLevel->cCurvedLaser++;
	if(gMain->currentLevel->cCurvedLaser >= MAX_CURVEDLASERS)
		gMain->currentLevel->cCurvedLaser -= MAX_CURVEDLASERS;
}

void GetAvailableSlotBeam()
{
	int tCounter = 0;
	while(gMain->currentLevel->beams[gMain->currentLevel->cBeam].anim >= 0 && tCounter < MAX_BULLETS*0.25)
	{	
		gMain->currentLevel->cBeam++; tCounter++;	
		if(gMain->currentLevel->cBeam >= MAX_BULLETS*0.25)
			gMain->currentLevel->cBeam -= MAX_BULLETS*0.25;
	}
	if(tCounter >= MAX_BULLETS*0.25)
		gMain->currentLevel->cBeam++;
	if(gMain->currentLevel->cBeam >= MAX_BULLETS*0.25)
		gMain->currentLevel->cBeam -= MAX_BULLETS*0.25;
}

float FastSqrt(float f)
{
	float result;
	DWORD pre = GetTickCount();
	_asm
	{
		mov eax, f
			sub eax, 0x3f800000
			sar eax, 1
			add eax, 0x3f800000
			mov result, eax
	}
	pre = GetTickCount();
	return result;
}

bs::UserTypeBase* emitBullet(float x, float y, float angle, const float* args, void* user)
{
	bullet* t;
	BIRTH_ANIM* g;
	//x, y, speed, angle, radius, sprite, intens
	g = &CreateSpawnEffect(args[-1], args[-2], args[-4], int(args[-6]));
	g->scale *= args[-7];
	g->decay *= args[-7];
	int typeBul = int(args[-6]) / NUM_COLORS;
	t = &CreateBullet(args[-1], args[-2], args[-3], args[-4], spriteDefaultScales[typeBul]*(args[-5] >= 0 ? args[-5] : spriteHitboxRadii[typeBul]), int(args[-6]), false);
	g->bul = t;
	t->blend = spriteBlendModes[typeBul];
	return t;
}

bs::UserTypeBase* emitBulletP(float x, float y, float angle, const float* args, void* user)
{
	//x y speed angle damage radius sprite 
	bullet* t = &CreateBullet(args[-1], args[-2], args[-3], args[-4], args[-6], int(args[-7]), true);
	t->damage = args[-5];
	t->alpha = 120;
	return t;
}

bs::UserTypeBase* emitBulletT(float x, float y, float angle, const float* args, void* user)
{
	BIRTH_ANIM* g;
	//x, y, speed, tx, ty, radius, sprite intens
	int typeBul = int(args[-7]) / NUM_COLORS;
	bullet *t = &CreateBulletT(args[-1], args[-2], args[-3], args[-4], args[-5], spriteDefaultScales[typeBul] * (args[-6] >= 0 ? args[-6] : spriteHitboxRadii[typeBul]), int(args[-7]), false);
	g = &CreateSpawnEffect(args[-1], args[-2], t->angle, int(args[-7]));
	g->scale *= args[-8];
	g->decay *= args[-8];

	g->bul = t;

	t->blend = spriteBlendModes[typeBul];
	return t;
}

bs::UserTypeBase* emitLaser(float x, float y, float angle, const float* args, void* user)
{
	BIRTH_ANIM* g;

	//x y speed angle radius cFrames sprite
	laser *t = &CreateLaser1(args[-1], args[-2], args[-3], args[-4], args[-5], unsigned(args[-6]), int(args[-7]));
	g = &CreateSpawnEffect(args[-1], args[-2], t->angle, int(args[-7]));
	g->bul = t;

	t->blend = spriteBlendModes[int(args[-7]) / NUM_COLORS];
	
	return t;
}

bs::UserTypeBase* emitLaserT(float x, float y, float angle, const float* args, void* user)
{
	BIRTH_ANIM* g;
	//x y speed tx ty cFrames sprite
	laser *t = &CreateLaserT(args[-1], args[-2], args[-3], args[-4], args[-5], args[-6], unsigned(args[-7]), int(args[-8]));
	g = &CreateSpawnEffect(args[-1], args[-2], t->angle, int(args[-8]));
	g->bul = t;
	t->blend = spriteBlendModes[int(args[-7])/NUM_COLORS];
	
	return t;
}

bs::UserTypeBase* emitCurvedLaser(float x, float y, float angle, const float* args, void* user)
{
	BIRTH_ANIM* g;

	//x y speed angle radius sprite
	curvedlaser *t = &CreateCurvedLaser(args[-1], args[-2], args[-3], args[-4], args[-5], args[-6]);
	g = &CreateSpawnEffect(args[-1], args[-2], t->angle, int(args[-6]));
	g->bul = t;
	t->blend = BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE;
	return t;
}

bs::UserTypeBase* emitCurvedLaserT(float x, float y, float angle, const float* args, void* user)
{
	BIRTH_ANIM* g;
	//x y speed tx ty radius sprite
	curvedlaser *t = &CreateCurvedLaserT(args[-1], args[-2], args[-3], args[-4], args[-5], args[-6], args[-7]);
	g = &CreateSpawnEffect(args[-1], args[-2], t->angle, int(args[-7]));
	g->bul = t;
	t->blend = BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE;
	return t;
}

bs::UserTypeBase* emitBeam(float x, float y, float angle, const float* args, void* user)
{
	//x y speed angle radius maxlength chargetime lifetime spr
	BIRTH_ANIM* g;
	beam* t = &CreateBeam(args[-1], args[-2], args[-3], args[-4], args[-5], args[-6], unsigned(args[-7]), unsigned(args[-8]), int(args[-9]));
	g = &CreateSpawnEffect(args[-1], args[-2], t->angle, S04*NUM_COLORS + int(args[-9]));
	g->bul = t;
	t->blend = BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE;
	return t;
}

BIRTH_ANIM& CreateSpawnEffect(float x, float y, float angle, int shotType)
{
	BIRTH_ANIM r;
	r.anim[0] = spawnEffectSprites[shotType/NUM_COLORS][0];
	r.anim[1] = spawnEffectSprites[shotType/NUM_COLORS][1];

	r.scale = 1.5;
	r.decay = 0.225;
	r.angle = angle;
	r.x = x;
	r.y = y;
	r.r = r.g = r.b = 0;

	int cIdx = shotType % NUM_COLORS;
	switch(cIdx)
	{		//red,yellow,magenta,orange
	case 0:	case 3:			case 5:	case 6:
		r.r = 255;
		break;
	}

	switch(cIdx)
	{		//green,yellow,cyan,orange
	case 1:	case 3:	case 4:
		r.g = 255;
		break;
	case 6:
		r.g = 169;
		break;
	}

	switch(cIdx)
	{		//blue,cyan,magenta
	case 2:			case 4:	case 5:
		r.b = 255;
		break;
	}

	gMain->currentLevel->anims.push_back(r);
	return gMain->currentLevel->anims[gMain->currentLevel->anims.size()-1];
}

void killBullet(bs::UserTypeBase *userType, void* user)
{
	projectile* b = static_cast<projectile*>(userType);

	b->anim = -1;
}

void setX(bs::UserTypeBase* userType, float value)
{
	projectile* b = static_cast<projectile*>(userType);
	b->x = value;
}

void setY(bs::UserTypeBase* userType, float value)
{
	projectile* b = static_cast<projectile*>(userType);
	b->y = value;
}

void setAngle(bs::UserTypeBase* userType, float value)
{
	projectile* b = static_cast<projectile*>(userType);
	b->angle = value;
}

void setSpeed(bs::UserTypeBase* userType, float value)
{
	projectile* b = static_cast<projectile*>(userType);
	b->speed = value;
}

void setAlpha(bs::UserTypeBase* userType, float value)
{
	value = max(0, value);
	projectile* b = static_cast<projectile*>(userType);
	b->alpha = value;
}

void setRadius(bs::UserTypeBase* userType, float value)
{
	value = max(0, value);
	projectile* b = static_cast<projectile*>(userType);
	b->radius = value;
}

float getX(bs::UserTypeBase* userType)
{
	projectile* b = static_cast<projectile*>(userType);
	return b->x;
}

float getY(bs::UserTypeBase* userType)
{
	projectile* b = static_cast<projectile*>(userType);
	return b->y;
}

float getAngle(bs::UserTypeBase* userType)
{
	projectile* b = static_cast<projectile*>(userType);
	return b->angle;
}

float getSpeed(bs::UserTypeBase* userType)
{
	projectile* b = static_cast<projectile*>(userType);
	return b->speed;
}

float getAlpha(bs::UserTypeBase* userType)
{
	projectile* b = static_cast<projectile*>(userType);
	return b->alpha;
}

float getRadius(bs::UserTypeBase* userType)
{
	projectile* b = static_cast<projectile*>(userType);
	return b->radius;
}

///

int GetAngleTowardsPlayer(bs::ScriptState& state)
{
	bs::bstype sX = state.stack[state.stackHead - 1];
	bs::bstype sY = state.stack[state.stackHead - 2];

	bs::bstype result = atan2(player->GetY() - sY, player->GetX() - sX);

	// Push result onto stack.  Normally, when a function ends the arguments must be popped off the stack.  This would be
	// done by subtracting the number of arguments from state.stackHead.  However, in this case, we are returning a value,
	// so this must be pushed onto the stack, taking the argument's place, so we don't need to modify state.stackHead.
	state.stackHead--;
	//state.stack[state.stackHead - 1] = result;
	state.stack[state.stackHead - 1] = result;
	return bs::ScriptOK;
}

int Sin(bs::ScriptState& state)
{
	state.stack[state.stackHead - 1] = sinf(state.stack[state.stackHead - 1]);
	return bs::ScriptOK;
}

int Cos(bs::ScriptState& state)
{
	state.stack[state.stackHead - 1] = cosf(state.stack[state.stackHead - 1]);
	return bs::ScriptOK;
}

int Random(bs::ScriptState& state)
{
	state.stack[state.stackHead - 2] = hgeCreate(HGE_VERSION)->Random_Float(state.stack[state.stackHead - 1], state.stack[state.stackHead - 2]);

	state.stackHead--;
	//state.stack[state.stackHead - 1] = x;
	return bs::ScriptOK;
}

int RandomInt(bs::ScriptState& state)
{
	state.stack[state.stackHead - 2] = bs::bstype(hgeCreate(HGE_VERSION)->Random_Int(int(state.stack[state.stackHead - 1]), int(state.stack[state.stackHead - 2])));

	state.stackHead--;
	//state.stack[state.stackHead - 1] = x;
	return bs::ScriptOK;
}

float getSprite(bs::UserTypeBase* userType)
{
	projectile* b = static_cast<projectile*>(userType);
	return b->anim;
}

void setSprite(bs::UserTypeBase* userType, float value)
{
	static_cast<projectile*>(userType)->anim = value;
}

inline bool RectRectIntersect(const hgeRect& rect1, const hgeRect& rect2)
{
	if(rect1.y2 < rect2.y1 || rect1.y1 > rect2.y2 || rect1.x2 < rect2.x1 || rect1.x1 > rect2.x2)
		return false;

	return true;
}

void ScoreLERP()
{
	float diff = gMain->score - gMain->curScore;
	gMain->curScore += diff*FRAME_TIME*20;
}

void ConvertAllToPoints()
{
	BIRTH_ANIM* r;
	for(int i = 0; i < MAX_BULLETS; i++)
	{
		if(gMain->currentLevel->bullets[i].anim >= 0)
		{
			r = &CreateSpawnEffect(gMain->currentLevel->bullets[i].x, gMain->currentLevel->bullets[i].y, gMain->currentLevel->bullets[i].angle, (int)gMain->currentLevel->bullets[i].anim % NUM_COLORS);
			r->decay *= 0.5;

			gMain->currentLevel->scorepoints.push_back(scorepoint(gMain->currentLevel->bullets[i].x, gMain->currentLevel->bullets[i].y));

			killBullet(&gMain->currentLevel->bullets[i], 0);
		}
	}

	for(int i = 0; i < MAX_BULLETS; i++)
	{
		if(gMain->currentLevel->lasers[i].anim >= 0)
		{
			for(float j = 0; j <= gMain->currentLevel->lasers[i].length; j += gMain->currentLevel->lasers[i].radius)
			{
				r = &CreateSpawnEffect(gMain->currentLevel->lasers[i].x + cosf(gMain->currentLevel->lasers[i].angle + M_PI)*j, 
				gMain->currentLevel->lasers[i].y + sinf(gMain->currentLevel->lasers[i].angle + M_PI)*j, gMain->currentLevel->lasers[i].angle, (int)gMain->currentLevel->lasers[i].anim % NUM_COLORS);
				r->decay *= 0.5;

				gMain->currentLevel->scorepoints.push_back(scorepoint(gMain->currentLevel->lasers[i].x + cosf(gMain->currentLevel->lasers[i].angle + M_PI)*j, gMain->currentLevel->lasers[i].y + sinf(gMain->currentLevel->lasers[i].angle + M_PI)*j));
			}

			killBullet(&gMain->currentLevel->lasers[i], 0);
		}
	}

	std::list<vec2>::iterator it;
	for(int i = 0; i < MAX_CURVEDLASERS; i++)
	{
		if(gMain->currentLevel->curvedlasers[i].anim >= 0)
		{

			for(it = gMain->currentLevel->curvedlasers[i].points.begin(); it != gMain->currentLevel->curvedlasers[i].points.end(); it++)
			{
				//r = &CreateSpawnEffect(it->x, it->y, 0, gMain->currentLevel->curvedlasers[i].anim % NUM_COLORS);
				//r->decay *= 0.5;

				gMain->currentLevel->scorepoints.push_back(scorepoint(it->x, it->y));
			}
			killBullet(&gMain->currentLevel->curvedlasers[i], 0);
		}
	}
}

void IntLERP(int& start, const int& end, float t)
{
	start += (end - start)*t;
}

void FloatLERP(float& start, const float& end, float t)
{
	start += (end - start)*t;
}

float FloatLERPEx(float start, const float& end, float t)
{
	return start + (end - start)*t;
}

bool operator <(const GrimoireKey& lhs, const GrimoireKey& rhs)
{
	//who gives a fuck, put anything here
	int sLHS=0, sRHS=0;
	std::string lhss(lhs.title);
	std::string rhss(rhs.title);

	for(int i = 0; i < lhss.size(); i++)
		sLHS += lhss.at(i);
	for(int i = 0; i < rhss.size(); i++)
		sRHS += rhss.at(i);

	sLHS += lhss.size();
	sRHS += rhss.size();

	return sLHS < sRHS;
}

bool operator ==(const GrimoireKey& lhs, const GrimoireKey& rhs)
{
	return strcmp(lhs.title, rhs.title) == 0;
}