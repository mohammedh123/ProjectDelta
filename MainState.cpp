#include "MainState.h"
#include "HGEGlobals.h"
#include "Game.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

MainState::MainState(bool isGrimoireTesting, GrimoireData* grimData) : isTesting(isGrimoireTesting)
{
	totaltime = 0;

	if(grimData)
	{
		gMain->level = 1;
		gMain->levels.push_back(new LEVEL());
		gMain->currentLevel = gMain->levels[0];

		player = new BalancedPlayer(CENTERX_OF_AREA, SCREEN_HEIGHT - 100);

		gMain->currentLevel->enemies.push_back(new Enemy(CENTERX_OF_AREA, 128, 0, 0, 10000000000, 0, 0, grimData->controllerName.c_str(), 0, 0, 0, grimData->func));
	}
	else
		gMain->initializeGame();
}

MainState::~MainState()
{
	
}

void MainState::Initialize()
{
	totaltime = 0;
//	if(!gMain->getControlledEntity())
//		gMain->initializeGame();
//	else
//	{
//	}
}

void MainState::Shutdown()
{
	for(int i = 0; i < gMain->levels.size(); i++)
		delete gMain->levels[i];
	gMain->levels.clear();
	delete player;
	delete this;
}

bool MainState::Update(float delta)
{
	if(hgeCreate(HGE_VERSION)->Input_KeyDown(HGEK_ESCAPE))
	{	
		delete StateManager::getInstance()->popState();
		return true;
	}
	//partSys->Update(delta);
	totaltime += delta;

	machine->setGlobalVariableValue("Player_X", player->GetX());
	machine->setGlobalVariableValue("Player_Y", player->GetY());

	UpdateCamera();
			
	hgeDistortionMesh* ds = res->GetDistortionMesh("testbg");

	for(int i = 0; i < ds->GetRows(); i++)
		for(int j = 0; j < ds->GetCols(); j++)
			ds->SetDisplacement(j, i, ((ds->GetRows()*0.5-1)-j)*(512/ds->GetRows())+(CENTERX_OF_AREA-player->GetX())/20*sinf(512/ds->GetRows() * (i*ds->GetRows()+j)), ((ds->GetRows()*0.5-1)-i)*(512/ds->GetRows())+(SCREEN_HEIGHT*0.5-player->GetY())/30*sinf(512/ds->GetRows() * (i*ds->GetRows()+j)), HGEDISP_CENTER);

	ScoreLERP();
	gMain->addNewEntities();
	gMain->UpdateAnimations(delta);
	gMain->checkCollisions();
	gMain->removeDeadEntities();
	gMain->executeGameLoop();

	return true; 
}

void MainState::Render()
{
	HGE* hge = hgeCreate(HGE_VERSION);	
	static hgeFont* fnt = res->GetFont("font_interface");

	//res->GetSprite("deltabgscroll")->Render(0,0);
	res->GetDistortionMesh("testbg")->Render(clipX1 -20, -5);
	gMain->drawEntities();
	res->GetSprite("deltabg")->Render(0,0);
	if(player->shifting)
		res->GetSprite("playerhitbox")->Render(player->GetX(), player->GetY());
	fnt->Render(clipX2 + 16, clipY1, HGETEXT_LEFT, "Score: ");
	fnt->printf(SCREEN_WIDTH - 16, clipY1, HGETEXT_RIGHT, "%d", int(ceil(gMain->curScore)));
	fnt->Render(clipX2 + 16, clipY1+24, HGETEXT_LEFT, "Graze: ");
	fnt->printf(SCREEN_WIDTH - 16, clipY1+24, HGETEXT_RIGHT, "%d", gMain->graze);
	fnt->Render(clipX2 + 16, clipY1+48, HGETEXT_LEFT, "Power: ");
	fnt->printf(SCREEN_WIDTH - 16, clipY1+48, HGETEXT_RIGHT, "%d", player->power);
} 