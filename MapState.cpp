#include "MapState.h"
#include "MainState.h"
#include "HGEGlobals.h"
#include "Game.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

MapState::MapState()
{
}

MapState::~MapState()
{
	
}

void MapState::Initialize()
{
	stages.push_back(StageAvatar(100, SCREEN_HEIGHT*0.5f, STAGE_ONE));
	stages.push_back(StageAvatar(250, SCREEN_HEIGHT*0.5f, STAGE_TWO));
	stages.push_back(StageAvatar(450, SCREEN_HEIGHT*0.5f-100, STAGE_TWO));
	stages.push_back(StageAvatar(650, SCREEN_HEIGHT*0.5f+100, STAGE_TWO));

	stages[0].connect(&stages[1], HGEK_RIGHT);
	stages[0].connect(&stages[3], HGEK_DOWN);

	stages[1].connect(&stages[2], HGEK_RIGHT);

	selectedStage = &stages[0];
	playerX = selectedStage->x;
	playerY = selectedStage->y;
	playerLERP = 0;

	cameraX = cameraY = bgCameraX = bgCameraY = 0;
	starCreationCounter = 0;
	totaltime = 0;

	bg.v[0].x = 0;				bg.v[0].y = 0;
	bg.v[1].x = SCREEN_WIDTH;	bg.v[1].y = 0;
	bg.v[2].x = SCREEN_WIDTH;	bg.v[2].y = SCREEN_HEIGHT;
	bg.v[3].x = 0;				bg.v[3].y = SCREEN_HEIGHT;

	bg.v[0].tx = 0;				bg.v[0].ty = 0;
	bg.v[1].tx = 3;				bg.v[1].ty = 0;
	bg.v[2].tx = 3;				bg.v[2].ty = 3;
	bg.v[3].tx = 0;				bg.v[3].ty = 3;

	bg.blend = BLEND_DEFAULT;

	for(int i=0;i<4;i++)
	{
		// Set up z-coordinate of vertices
		bg.v[i].z=0.5f;
		// Set up color. The format of DWORD col is 0xAARRGGBB
		bg.v[i].col=0xFFFFFFFF;
	}

	bg.tex = res->GetTexture("map_bg.png");
}

void MapState::Shutdown()
{
	delete this;
}

bool MapState::Update(float delta)
{
	HGE* hge = hgeCreate(HGE_VERSION);

	playerLERP = min(1, playerLERP += FRAME_TIME);
	FloatLERP(playerX, selectedStage->x, playerLERP*0.25);
	FloatLERP(playerY, selectedStage->y, playerLERP*0.25);

	starCreationCounter++;

	bgCameraX = starCreationCounter*0.001f;

	if(playerX > SCREEN_WIDTH*0.5f)
	{
		cameraX = playerX - SCREEN_WIDTH*0.5;
	}

	for(std::set<MapState::StageAvatar::KeyBindStage>::iterator i = selectedStage->connectedStages.begin(); i != selectedStage->connectedStages.end(); i++)
		if(hge->Input_KeyDown(i->key))
		{
			selectedStage = i->stage;
			playerLERP = 0;
			break;
		}

	if(starCreationCounter % 20 == 0)
	{
		stars.push_back(Star(bgCameraX*150+hge->Random_Float(0, SCREEN_WIDTH), bgCameraY*150+hge->Random_Float(0, SCREEN_HEIGHT), ARGB(120, 
			hge->Random_Int(0, 255),
			hge->Random_Int(0, 255),
			hge->Random_Int(0, 255)), hge->Random_Float(0, 0.75)));
	}

	for(int i = 0; i < stars.size(); i++)
	{
		stars[i].scale = sinf(stars[i].lifetime*M_PI*0.00625f)*stars[i].dscale;
		stars[i].lifetime++;

		if(stars[i].scale < 0)
		{
			stars.erase(stars.begin() + i);
			i--;
		}
		
	}

	if(hge->Input_KeyDown(HGEK_ENTER))
		StateManager::getInstance()->pushState(new MainState());

	bg.v[0].tx = bgCameraX+0;						bg.v[0].ty = bgCameraY+0;
	bg.v[1].tx = bgCameraX+3;						bg.v[1].ty = bgCameraY+0;
	bg.v[2].tx = bgCameraX+3;						bg.v[2].ty = bgCameraY+3;
	bg.v[3].tx = bgCameraX+0;						bg.v[3].ty = bgCameraY+3;

	return true; 
}

void MapState::Render()
{
	HGE* hge = hgeCreate(HGE_VERSION);	
	
	hge->Gfx_RenderQuad(&bg);
	hge->Gfx_Clear(0);
	for(int i = 0; i < stars.size(); i++)
	{
		res->GetSprite("map_star")->SetColor(stars[i].color);
		res->GetSprite("map_star")->RenderEx(stars[i].x-bgCameraX*150, stars[i].y-bgCameraY*150, 0, stars[i].scale);
	}

	for(int i = 0; i < stages.size(); i++)
		for(std::set<MapState::StageAvatar::KeyBindStage>::iterator j = stages[i].connectedStages.begin(); j != stages[i].connectedStages.end(); j++)
			hge->Gfx_RenderLine(stages[i].x-cameraX, stages[i].y-cameraY, j->stage->x-cameraX, j->stage->y-cameraY, 0x3300FF00); 

	for(int i = 0; i < stages.size(); i++)
	{
		stages[i].sprite->Render(stages[i].x-cameraX, stages[i].y-cameraY);
		if(selectedStage == &stages[i])
		{	
			res->GetSprite("map_stageselect")->Render(stages[i].x-cameraX, stages[i].y-cameraY);
			res->GetAnimation("player")->RenderEx(playerX-cameraX, playerY-cameraY, M_PI_2, 1);
		}	
	}
}

bool operator <(const MapState::StageAvatar::KeyBindStage& lhs, const MapState::StageAvatar::KeyBindStage& rhs)
{
	return lhs.key < rhs.key && lhs.stage < rhs.stage;
}