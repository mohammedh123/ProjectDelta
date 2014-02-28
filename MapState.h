#ifndef _MAPSTATE
#define _MAPSTATE

#include "HGEGlobals.h"
#include "StateManager.h"
#include "GameState.h"

#include <set>

class MapState : public GameState
{
public:
	enum StageID
	{
		STAGE_ONE,
		STAGE_TWO
	};

	struct Star
	{
		Star(float x, float y, DWORD color, float scale) : dscale(scale), x(x), y(y), color(color), scale(0), lifetime(0) {}
		float x,y,dscale,scale;
		int lifetime;
		DWORD color;
	};

	struct StageAvatar
	{
		struct KeyBindStage
		{
			KeyBindStage(StageAvatar* stage, int key) : stage(stage), key(key) {}

			StageAvatar* stage;
			int key;
		};

		StageAvatar(float x, float y, StageID id, hgeSprite* sprite=res->GetSprite("map_stage")) : x(x), y(y), id(id), sprite(sprite) {}

		void connect(StageAvatar* rhs, int key)
		{
			connectedStages.insert(KeyBindStage(rhs, key));
			
			int opKey;
			if(key == HGEK_LEFT)
				opKey = HGEK_RIGHT;
			else if(key == HGEK_RIGHT)
				opKey = HGEK_LEFT;
			else if(key == HGEK_UP)
				opKey = HGEK_DOWN;
			else if(key == HGEK_DOWN)
				opKey = HGEK_UP;
			
			rhs->connectedStages.insert(KeyBindStage(this, opKey));
		}

		float x, y;
		StageID id;
		std::set<KeyBindStage> connectedStages;
		hgeSprite* sprite;
	};

public:
	MapState();
	~MapState();

	void Initialize();
	void Shutdown();

	bool Update(float delta);
	void Render();
private:
	StageAvatar* selectedStage;
	float bgCameraX, bgCameraY, cameraX, cameraY, playerX, playerY, playerLERP;
	hgeQuad bg;
	int starCreationCounter;
	std::vector<StageAvatar> stages;
	std::vector<Star> stars;
};

bool operator <(const MapState::StageAvatar::KeyBindStage& lhs, const MapState::StageAvatar::KeyBindStage& rhs);

#endif