#include "GrimoireState.h"
#include "HGEGlobals.h"
#include "MainState.h"
#include "guiMenuItem.h"

GrimoireState::GrimoireState()
{}

GrimoireState::~GrimoireState()
{
}

void GrimoireState::Initialize()
{
	selectedIdx = 0;
	selected = grimoire.begin();
	end = grimoire.end();
	end--;
	keyDownCounter = 0;

	totaltime = 0;
}

void GrimoireState::Shutdown()
{
	delete this;
   // release whatever resources I loaded in initialize
}

bool GrimoireState::Update(float delta)
{
	totaltime += delta;
	hgeInputEvent evt;

	while(hgeCreate(HGE_VERSION)->Input_GetEvent(&evt))
	{
		if(evt.type == INPUT_KEYDOWN)
		{
			if(evt.key == HGEK_ESCAPE)
				StateManager::getInstance()->popState();
			if(evt.key == HGEK_ENTER)
				StateManager::getInstance()->pushState(new MainState(true, &selected->second));
			
			if(evt.key == HGEK_UP)
			{
				if(keyDownCounter % 3 == 0)
				{
					if(selected != grimoire.begin())
					{
						selected--;
						selectedIdx--;
					}
					else
					{
						selected = end;
						selectedIdx = grimoire.size() - 1;
					}
				}
				if(evt.flags & HGEINP_REPEAT)
				{
					keyDownCounter++;
				}
			}
			else if(evt.key == HGEK_DOWN)
			{
				if(keyDownCounter % 3 == 0)
				{
					if(selected != end)
					{
						selected++;
						selectedIdx++;
					}
					else
					{
						selected = grimoire.begin();
						selectedIdx = 0;
					}
				}
				if(evt.flags & HGEINP_REPEAT)
				{
					keyDownCounter++;
				}
			}
		}
		else if(evt.type == INPUT_KEYUP)
			if(evt.key == HGEK_UP || evt.key == HGEK_DOWN)
				keyDownCounter = 0;
	}

	return true; 
}

void GrimoireState::Render()
{
	HGE* hge = hgeCreate(HGE_VERSION);
	
	static hgeSprite* ds = res->GetSprite("menu_bg");
	ds->Render(0,0);
	
	float heightOfBox = 200;
	float currentHeight = 0;
	oss.str("");

	for(std::map<GrimoireKey, GrimoireData>::const_iterator i = grimoire.begin(); i != grimoire.end(); i++)
	{
		currentHeight += res->GetFont("font_small")->GetHeight();
		oss << i->first.title << "\n";
	}
	
	if(currentHeight > heightOfBox)
	{
		res->GetFont("font_small")->SetScale(heightOfBox / currentHeight);
		res->GetFont("font_small")->SetProportion(currentHeight / heightOfBox);
	}
	emptySprite->SetColor(0x33FFFFFF);
	emptySprite->RenderStretch(95, SCREEN_HEIGHT*0.5f - min(heightOfBox, currentHeight)*0.5f
		+ selectedIdx*res->GetFont("font_small")->GetScale()*res->GetFont("font_small")->GetHeight(), 
		105 + res->GetFont("font_small")->GetStringWidth(oss.str().c_str()), 
		SCREEN_HEIGHT*0.5f - min(heightOfBox, currentHeight)*0.5f
		+ (selectedIdx+1)*res->GetFont("font_small")->GetScale()*res->GetFont("font_small")->GetHeight());
	res->GetFont("font_small")->Render(100, SCREEN_HEIGHT*0.5f - min(heightOfBox, currentHeight)*0.5f, HGETEXT_LEFT, oss.str().c_str());
	
	res->GetFont("font_small")->SetScale(1);
	res->GetFont("font_small")->SetProportion(1);

	res->GetFont("menu_item")->SetColor(0xFFFFFFFF);
	res->GetFont("menu_item")->printf(SCREEN_WIDTH*0.5+50, 200, HGETEXT_CENTER, "%s\n%s", selected->first.title, selected->second.desc.c_str());
	// render our background, etc.
} 