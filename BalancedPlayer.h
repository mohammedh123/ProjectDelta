#ifndef __BALANCEDPLAYER_H__
#define __BALANCEDPLAYER_H__

#include "Player.h"

class BalancedPlayer : public Player
{
public:
	BalancedPlayer(float x, float y);

	void Update();
};

#endif