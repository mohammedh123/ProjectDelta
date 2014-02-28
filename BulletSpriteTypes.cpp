#include "BulletSpriteTypes.h"


hgeAnimation* enemySprites[NUM_ENEMY_SPRITES];

hgeAnimation* playerShotSprites[3];
hgeSprite* beamSprites[NUM_BEAM_SPRITES];
hgeAnimation* bulletSprites[NUM_BULLET_SPRITES*NUM_COLORS];
hgeAnimation* spawnEffectSprites[NUM_BULLET_SPRITES][2];
float spriteHitboxRadii[NUM_BULLET_SPRITES];
float spriteRadiiInv[NUM_BULLET_SPRITES];
float beamRadiiInv[NUM_BEAM_SPRITES];
float spriteDefaultScales[NUM_BULLET_SPRITES];
int spriteBlendModes[NUM_BULLET_SPRITES];