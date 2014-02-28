#ifndef __BULLETSPRITETYPES_H___
#define __BULLETSPRITETYPES_H___

#include "hge.h"
#include "hgeanim.h"

#include <map>
#define NUM_BEAM_SPRITES 8
#define NUM_BULLET_SPRITES 8
#define NUM_ENEMY_SPRITES 9
#define NUM_COLORS 8

//	en1 = glowing edge ball enemy

//	01 = small pellet (na)
//	02 = giant ball (a)
//	03 = large ball (black edge, na)
//	04 = glow ball (a)
//	05 = white ball (colored deep edges, na)
//	06 = large ball (very small color edge, a)
//	07 = extremely large sun (all colors)
//	08 = large pellet (na)

//	RED, BLUE, GREEN
//	YELLOW, CYAN, MAGENTA
//	ORANGE, BLACK

enum PlayerShotSprites
{
	STANDARD_GREEN,
	BALANCED_OTHER,
	BALANCED_BIG
};

enum BulletSprites
{
	S01,
	S02,
	S03,
	S04,
	S05,
	S06,
	S07,
	S08
};

enum BeamSprites
{
	B_RED
};

enum EnemySpriteTypes
{
	EST_EN1RED,
	EST_EN1GREEN,	
	EST_EN1BLUE,	
	EST_EN1YELLOW,	
	EST_EN1CYAN,		
	EST_EN1MAGENTA,	
	EST_EN1WHITE,
	EST_EN1BLACK,
	
	EST_BOSS1
};

extern hgeAnimation* enemySprites[NUM_ENEMY_SPRITES];

extern hgeAnimation* playerShotSprites[3];
extern hgeSprite* beamSprites[NUM_BEAM_SPRITES];
extern hgeAnimation* bulletSprites[NUM_BULLET_SPRITES*NUM_COLORS];
extern hgeAnimation* spawnEffectSprites[NUM_BULLET_SPRITES][2];
extern float spriteHitboxRadii[NUM_BULLET_SPRITES];
extern float spriteRadiiInv[NUM_BULLET_SPRITES];
extern float beamRadiiInv[NUM_BEAM_SPRITES];
extern float spriteDefaultScales[NUM_BULLET_SPRITES];
extern int spriteBlendModes[NUM_BULLET_SPRITES];
#endif