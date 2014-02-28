#include "HGEGlobals.h"
#include "StateManager.h"
#include "MenuState.h"
#include "Game.h"
#include "BulletSpriteTypes.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "GrimoireFunctions.h"
using namespace std;

bool FrameFunc()
{
	HGE* hge = hgeCreate(HGE_VERSION);
/*	if(hge->Input_KeyDown(HGEK_ESCAPE))
	{
		StateManager::getInstance()->popAll();
		return true;
	}*/	
	if(hge->Input_KeyDown(HGEK_Q))
		gMain->paused = !gMain->paused;
	
	if(hge->Input_KeyDown(HGEK_W))
	{
		for(int i = 0; i < gMain->currentLevel->enemies.size()-1; i++)
		{
			gMain->currentLevel->enemies[i]->SetHP(0);
			gMain->currentLevel->enemies.back()->SetActiveTime(0);
		}
	};//HGEFPS_UNLIMITED);
	if(hge->Input_KeyUp(HGEK_W))
		hge->System_SetState(HGE_FPS, 60);

	if (!StateManager::getInstance()->Update(hge->Timer_GetDelta()))
	{
		return true;
	}
	return false;
}

bool RenderFunc()
{
	HGE* hge = hgeCreate(HGE_VERSION);

	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0xFFDDDDDD);

	StateManager::getInstance()->Render();

	res->GetFont("font_small")->SetColor(0xFFFFFFFF);
	res->GetFont("font_small")->printf(SCREEN_WIDTH, SCREEN_HEIGHT-10, HGETEXT_RIGHT, "FPS: %d", hge->Timer_GetFPS());
	hge->Gfx_EndScene();
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{	
	HGE* hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_FPS, 60);//HGEFPS_UNLIMITED);
	hge->System_SetState(HGE_SHOWSPLASH, false);
	//hge->System_SetState(HGE_TEXTUREFILTER, true);
	hge->System_SetState(HGE_HIDEMOUSE, false);
	//hge->System_SetState(HGE_INIFILE, "ProjectDelta.ini");
	hge->System_SetState(HGE_LOGFILE, "ProjectDelta.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "ProjectDelta");
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate()) 
	{
		res = new hgeResourceManager("resource.res");
		res->Precache();
		initializeVariables();
		hge->Gfx_SetClipping(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		hge->Random_Seed();

		grimoire[GrimoireKey("Spiral 1")] = GrimoireData(SpiralTest1, "spiraltest1", "Triple spiral, random movement.");
		grimoire[GrimoireKey("Special 1")] = GrimoireData(SpecialTest1, "specialtest1", "Set of flowering bullets.");
		grimoire[GrimoireKey("Special 2")] = GrimoireData(SpecialTest2, "specialtest2", "who the hell knows");
		grimoire[GrimoireKey("Special 3")] = GrimoireData(SpecialTest3, "specialtest3", "bootleg amulet");
		grimoire[GrimoireKey("Special 4")] = GrimoireData(SpecialTest3, "specialtest4", "subterranean sun");
		grimoire[GrimoireKey("Special 5")] = GrimoireData(SpecialTest2, "specialtest5", "mishakuji-sama");
		emptySprite = new hgeSprite(NULL, 0,0,32,32);
		emptySprite->SetColor(ARGB(255, 100,0,0));
		//emptySprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
	emptySprite->SetBlendMode(BLEND_DEFAULT);
		//gMain->loadMap(FILE_NAME);
		StateManager::getInstance()->pushState(new MenuState());
		for(int i = 0; i < NUM_BULLET_SPRITES*NUM_COLORS; i++)
			bulletSprites[i] ? bulletSprites[i]->Play() : 0;
		hge->System_Start();

		StateManager::getInstance()->popAll();
		delete StateManager::getInstance();
		delete emptySprite;
	}

	for(int i = 0; i < NUM_BULLET_SPRITES*NUM_COLORS; i++)
		delete bulletSprites[i];
	for(int i = 0; i < NUM_BEAM_SPRITES; i++)
		delete beamSprites[i];

	delete player;
	delete res;
	delete gMain;
	delete machine;
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
