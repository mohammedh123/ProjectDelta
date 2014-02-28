#include "Player.h"

#include "HGEGlobals.h"
#include <stack>
#include <vector>

Player::Player(float x, float y, char* ctrlname) : x(x), y(y), baseSpeed(240), speed(baseSpeed), slowSpeed(120), controller(0), shooting(false), shifting(false), power(0)
{
	SetController(ctrlname);
}

Player::~Player()
{
	if(controller)
		machine->destroyController(controller);
}

float Player::GetX() const
{	return x;	}

float Player::GetY() const
{	return y;	}

float Player::GetSpeed() const
{	return speed;	}

void Player::SetPosition(float x1, float y1)
{	x = x1; y = y1;	}

void Player::SetController(const char* name)
{
	controller = machine->createController(name, x, y, 0, this);
	controller->enable(false);
}

void Player::SetSpeed(float spd)
{	speed = spd;	}

void Player::Update()
{
	HGE* hge = hgeCreate(HGE_VERSION);
	static std::stack<int,std::vector<int>> hDir(std::vector<int>(1, 0)), vDir(std::vector<int>(1, 0));
	
	if(hge->Input_KeyDown(HGEK_SHIFT))
	{
		shifting = true;
		controller->raiseEvent("shift", 0);
	}
	if(hge->Input_KeyUp(HGEK_SHIFT))
	{
		shifting = false;
		controller->raiseEvent("regular", 0);
	}

	SetSpeed(shifting ? slowSpeed : baseSpeed);
	if(hge->Input_KeyDown(HGEK_LEFT))
	{	hDir.push(-1);						}
	else if(hge->Input_KeyUp(HGEK_LEFT))
	{	if(hDir.top() == -1){ hDir.pop();}	}

	if(hge->Input_KeyDown(HGEK_RIGHT))
	{	hDir.push(1);						}
	else if(hge->Input_KeyUp(HGEK_RIGHT))
	{	if(hDir.top() == 1){ hDir.pop();}	}

	if(hge->Input_KeyDown(HGEK_UP))
	{	vDir.push(-1);						}
	else if(hge->Input_KeyUp(HGEK_UP))
	{	if(vDir.top() == -1){ vDir.pop();}	}

	if(hge->Input_KeyDown(HGEK_DOWN))
	{	vDir.push(1);						}
	else if(hge->Input_KeyUp(HGEK_DOWN))
	{	if(vDir.top() == 1){ vDir.pop();}	}

	if(!(hge->Input_GetKeyState(HGEK_LEFT) || hge->Input_GetKeyState(HGEK_RIGHT)))
	{	while(hDir.size() != 1)	hDir.pop();	}
	if(!(hge->Input_GetKeyState(HGEK_UP) || hge->Input_GetKeyState(HGEK_DOWN)))
	{	while(vDir.size() != 1)	vDir.pop();	}

	SetPosition(x+speed*FRAME_TIME*hDir.top(),y+speed*FRAME_TIME*vDir.top());
	if(x-8 < clipX1)
		x = clipX1+8;
	if(x+8 > clipX2)
		x = clipX2-8;
	if(y-12 < clipY1)
		y = clipY1+12;
	if(y+16 > clipY2)
		y = clipY2-16;

	controller->setPosition(x, y);

	if(hge->Input_KeyDown(HGEK_Z))
		controller->enable(true);
	if(hge->Input_KeyUp(HGEK_Z))
		controller->enable(false);


}

void Player::Render()
{
	res->GetAnimation("player")->Render(x, y);
}