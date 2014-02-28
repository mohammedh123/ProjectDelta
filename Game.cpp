#include "HGEGlobals.h"
#include "Game.h"
#include "StateManager.h"
#include "Enemy.h"
#include <fstream>
#include <stack>
#include "BulletSpriteTypes.h"

Game::Game() :	controlledEntity(NULL), level(1), graze(0)
{
	paused = false;
	score = 0;
	curScore = 0;
}

Game::~Game()
{
	delete controlledEntity;
	while(!levels.empty())
	{
		delete levels[levels.size() - 1];
		levels.pop_back();
	}
}

Entity* Game::getEntity(size_t idx) const
{
	return NULL;
}

unsigned Game::getSize() const
{	return 0;	}

Entity* Game::getControlledEntity() const
{	return controlledEntity;	}

void Game::addEntity(Entity* n)
{
	//currentLevel->entities.push_back(n);
}

void Game::setControlledEntity(Entity* ent)
{	controlledEntity = ent;	}

void Game::addNewEntities()
{

}

void Game::executeGameLoop() const
{
	UpdateEntities();
}

void Game::UpdateEntities() const
{	
	res->GetParticleSystem("playershot")->Update(FRAME_TIME);
	static hgeVector pPos;
	if(paused)
		return;

	machine->preUpdate(FRAME_TIME);

	player->Update();
	if(player->GetY() <= 128)
	{
		for(size_t i = 0; i < currentLevel->powers.size(); i++)
		{
			if(currentLevel->powers[i].framesInRadius < 0)
				currentLevel->powers[i].framesInRadius = 0;
		}
	}
	pPos.x = player->GetX();
	pPos.y = player->GetY();

	float speedSqr = 0, speedX=0, speedY=0;
	for(int i = 0; i < MAX_BULLETS; i++)
	{
		if(currentLevel->bullets[i].anim >= 0)
		{	
			while(currentLevel->bullets[i].angle > M_PI)
				currentLevel->bullets[i].angle -= 2*M_PI;
			while(currentLevel->bullets[i].angle < -M_PI)
				currentLevel->bullets[i].angle += 2*M_PI;

			speedX = cosf(currentLevel->bullets[i].angle)*currentLevel->bullets[i].speed;
			speedY = sinf(currentLevel->bullets[i].angle)*currentLevel->bullets[i].speed;

			currentLevel->bullets[i].x += speedX;
			currentLevel->bullets[i].y += speedY;

			if(currentLevel->bullets[i].func)
				(currentLevel->bullets[i].func)(&currentLevel->bullets[i], FRAME_TIME, 0);
			machine->updateType(&currentLevel->bullets[i], currentLevel->bullets[i].x, currentLevel->bullets[i].y, currentLevel->bullets[i].angle, FRAME_TIME);

			static unsigned diff = 50;
			static hgeRect t;
			static hgeRect viewWindow = hgeRect(clipX1-20, clipY1-20, clipX2+20, clipY2);
			t.SetRadius(currentLevel->bullets[i].x, currentLevel->bullets[i].y, currentLevel->bullets[i].radius);

			if(!RectRectIntersect(t, viewWindow))
			{
				killBullet(&currentLevel->bullets[i], 0);
			}
			currentLevel->bullets[i].framesAlive++;
		}
	}
	float laserEndX, laserEndY; 
	for(int i = 0; i < MAX_BULLETS; i++)
	{
		if(currentLevel->lasers[i].anim >= 0)
		{	
			while(currentLevel->lasers[i].angle > M_PI)
				currentLevel->lasers[i].angle -= 2*M_PI;
			while(currentLevel->lasers[i].angle < -M_PI)
				currentLevel->lasers[i].angle += 2*M_PI;
			

			speedX = cosf(currentLevel->lasers[i].angle)*currentLevel->lasers[i].speed;
			speedY = sinf(currentLevel->lasers[i].angle)*currentLevel->lasers[i].speed;
			
			currentLevel->lasers[i].x += speedX;
			currentLevel->lasers[i].y += speedY;
			machine->updateType(&currentLevel->lasers[i], currentLevel->lasers[i].x, currentLevel->lasers[i].y, currentLevel->lasers[i].angle, FRAME_TIME);

			static unsigned diff = 50;
			laserEndX = currentLevel->lasers[i].x + cosf(currentLevel->lasers[i].angle+M_PI)*currentLevel->lasers[i].length;
			laserEndY = currentLevel->lasers[i].y + sinf(currentLevel->lasers[i].angle+M_PI)*currentLevel->lasers[i].length;

			if(laserEndX < clipX1-diff || laserEndX > clipX2+diff || laserEndY < clipY1-diff || laserEndY > clipY2+diff)
			{
				killBullet(&currentLevel->lasers[i], 0);
			}
			currentLevel->lasers[i].framesAlive++;
			//currentLevel->lasers[i].creationFrames--;
			if(currentLevel->lasers[i].creationFrames >= currentLevel->lasers[i].framesAlive) 
				currentLevel->lasers[i].length += currentLevel->lasers[i].speed;
		}
	}

	for(int i = 0; i < MAX_CURVEDLASERS; i++)
	{
		if(currentLevel->curvedlasers[i].anim >= 0)
		{	
			while(currentLevel->curvedlasers[i].angle > M_PI)
				currentLevel->curvedlasers[i].angle -= 2*M_PI;
			while(currentLevel->curvedlasers[i].angle < -M_PI)
				currentLevel->curvedlasers[i].angle += 2*M_PI;

			currentLevel->curvedlasers[i].points.push_back(vec2(currentLevel->curvedlasers[i].x, currentLevel->curvedlasers[i].y));
			
			speedX = cosf(currentLevel->curvedlasers[i].angle)*currentLevel->curvedlasers[i].speed;
			speedY = sinf(currentLevel->curvedlasers[i].angle)*currentLevel->curvedlasers[i].speed;
			
			currentLevel->curvedlasers[i].x += speedX;
			currentLevel->curvedlasers[i].y += speedY;
			machine->updateType(&currentLevel->curvedlasers[i], currentLevel->curvedlasers[i].x, currentLevel->curvedlasers[i].y, currentLevel->curvedlasers[i].angle, FRAME_TIME);

			if(currentLevel->curvedlasers[i].points.size() == 32)
			{
				currentLevel->curvedlasers[i].points.erase(currentLevel->curvedlasers[i].points.begin());	
			}
			
			static unsigned diff = 50;
	
			laserEndX = currentLevel->curvedlasers[i].points.front().x;
			laserEndY = currentLevel->curvedlasers[i].points.front().y;

			if(laserEndX < clipX1-diff || laserEndX > clipX2+diff || laserEndY < clipY1-diff || laserEndY > clipY2+diff)
			{
				laserEndX = currentLevel->curvedlasers[i].points.back().x;
				laserEndY = currentLevel->curvedlasers[i].points.back().y;

				if(laserEndX < clipX1-diff || laserEndX > clipX2+diff || laserEndY < clipY1-diff || laserEndY > clipY2+diff)
				{
					killBullet(&currentLevel->curvedlasers[i], 0);
				}
			}
			currentLevel->curvedlasers[i].framesAlive++;
		}
	}

	int framesIn, framesOff;
	for(int i = 0; i < MAX_BULLETS*0.25; i++)
	{
		if(currentLevel->beams[i].anim >= 0)
		{	
			while(currentLevel->beams[i].angle > M_PI)
				currentLevel->beams[i].angle -= 2*M_PI;
			while(currentLevel->beams[i].angle < -M_PI)
				currentLevel->beams[i].angle += 2*M_PI;
						
			machine->updateType(&currentLevel->beams[i], currentLevel->beams[i].x, currentLevel->beams[i].y, currentLevel->beams[i].angle, FRAME_TIME);

			currentLevel->beams[i].framesAlive++;
			//currentLevel->beams[i].creationFrames--;

			framesIn = currentLevel->beams[i].framesAlive - currentLevel->beams[i].chargetime;
			framesOff = currentLevel->beams[i].lifetime - framesIn;

			if(framesIn > 0 && framesIn <= 5)
			{
				currentLevel->beams[i].radius = currentLevel->beams[i].initRad * 0.2 * framesIn;
			}
			if(framesOff <= 5)
			{
				currentLevel->beams[i].radius = currentLevel->beams[i].initRad * 0.2 * framesOff;
			}

			if(currentLevel->beams[i].framesAlive >= currentLevel->beams[i].chargetime + currentLevel->beams[i].lifetime)
				killBullet(&currentLevel->beams[i], 0);
			currentLevel->beams[i].length = min(currentLevel->beams[i].length += currentLevel->beams[i].speed, currentLevel->beams[i].maxlength);
		}
	}

	machine->postUpdate(FRAME_TIME);

	for(size_t i = 0; i < currentLevel->enemies.size(); i++)
	{
		currentLevel->enemies[i]->Update();
	}

	hgeVector t;
	for(size_t i = 0; i < currentLevel->anims.size(); i++)
	{
		if(currentLevel->anims[i].target)
		{
			t = LERP(hgeVector(currentLevel->anims[i].x, currentLevel->anims[i].y), hgeVector(currentLevel->anims[i].target->GetX(), currentLevel->anims[i].target->GetY()), currentLevel->anims[i].lifetime*FRAME_TIME);
			currentLevel->anims[i].x = t.x;
			currentLevel->anims[i].y = t.y;

			if(t == hgeVector(currentLevel->anims[i].target->GetX(), currentLevel->anims[i].target->GetY()))
				currentLevel->anims[i].scale = 0;
		}
		currentLevel->anims[i].a -= currentLevel->anims[i].adecay;
		currentLevel->anims[i].scale -= currentLevel->anims[i].decay;
		currentLevel->anims[i].lifetime++;
		if(currentLevel->anims[i].bul && currentLevel->anims[i].bul->anim >= 0)
		{
			switch(currentLevel->anims[i].bul->type)
			{
			case 0:
				currentLevel->anims[i].x = currentLevel->anims[i].bul->x;
				currentLevel->anims[i].y = currentLevel->anims[i].bul->y;
				break;
			case 1:
				if(((laser*)(currentLevel->anims[i].bul))->creationFrames >= ((laser*)(currentLevel->anims[i].bul))->framesAlive)
					currentLevel->anims[i].scale = 0.5+0.05f*sinf(((laser*)(currentLevel->anims[i].bul))->framesAlive*M_PI_12);
				break;
			case 2:
				if(((curvedlaser*)currentLevel->anims[i].bul)->points.size() < 31)
					currentLevel->anims[i].scale = 0.5+0.05f*sinf(((curvedlaser*)(currentLevel->anims[i].bul))->framesAlive*M_PI_12);
				break;
			case 3:
				currentLevel->anims[i].x = currentLevel->anims[i].bul->x;
				currentLevel->anims[i].y = currentLevel->anims[i].bul->y;
				if(((beam*)currentLevel->anims[i].bul)->framesAlive < ((beam*)currentLevel->anims[i].bul)->lifetime + ((beam*)currentLevel->anims[i].bul)->chargetime)
					currentLevel->anims[i].scale = 0.2+0.1f*sinf(((beam*)(currentLevel->anims[i].bul))->framesAlive*M_PI_2);
				break;
			}
		}
		if(currentLevel->anims[i].scale <= 0 || currentLevel->anims[i].a <= 0)
		{
			currentLevel->anims.erase(currentLevel->anims.begin() + i);
			i--;
		}
		//else
		//	currentLevel->anims[i].bul && currentLevel->anims[i].bul->type == 0 ? currentLevel->anims[i].bul->alpha = 255 : 0; 
	}


	for(size_t i = 0; i < currentLevel->powers.size(); i++)
	{
		currentLevel->powers[i].update();
		if(currentLevel->powers[i].framesInRadius < 0 && circleIntersect(currentLevel->powers[i].x, currentLevel->powers[i].y, 4, pPos.x, pPos.y, 25))
			currentLevel->powers[i].framesInRadius++;
		else if(currentLevel->powers[i].framesInRadius >= 0)
		{
			t = LERP(hgeVector(currentLevel->powers[i].x,currentLevel->powers[i].y), pPos, currentLevel->powers[i].framesInRadius*FRAME_TIME*0.5f);
			currentLevel->powers[i].x = t.x;
			currentLevel->powers[i].y = t.y;

			currentLevel->powers[i].framesInRadius++;
		}
		
		if(circleIntersect(currentLevel->powers[i].x, currentLevel->powers[i].y, 4, pPos.x, pPos.y, 15))
		{
			//increase players power
			switch(currentLevel->powers[i].type)
			{
			case SMALL:
				player->power += 1;
				break;
			case LARGE:
				player->power += 5;
				break;
			}
			currentLevel->powers.erase(currentLevel->powers.begin() + i);
			i--;
		}
		else if(currentLevel->powers[i].y > SCREEN_HEIGHT + 20)
		{
			currentLevel->powers.erase(currentLevel->powers.begin() + i);
			i--;
		}
	}

	for(size_t i = 0; i < currentLevel->scorepoints.size(); i++)
	{
		t = LERP(currentLevel->scorepoints[i].pos, pPos, currentLevel->scorepoints[i].framesAlive*FRAME_TIME*0.5f);
		currentLevel->scorepoints[i].pos = t;

		currentLevel->scorepoints[i].framesAlive++;
	}
}

void Game::UpdateAnimations(float delta)
{	
	float ddx,ddy,ddw,ddh;	
	for(int i = 0; i < NUM_BEAM_SPRITES; i++)
	{
		beamSprites[i]->GetTextureRect(&ddx, &ddy, &ddw, &ddh);	
		beamSprites[i]->SetTextureRect(ddx+6, ddy, ddw, ddh, false);
		if(ddx > beamSprites[i]->GetWidth())
			ddx -= beamSprites[i]->GetWidth();	
	}
	
	for(int i = 0; i < NUM_BULLET_SPRITES*NUM_COLORS; i++)
		bulletSprites[i] ? bulletSprites[i]->Update(delta) : 0;
	for(size_t i = 0; i < currentLevel->enemies.size(); i++)
	{
		currentLevel->enemies[i]->GetAnimation()->Update(delta);
	}
}

void Game::removeDeadEntities()
{	
	
	for(size_t i = 0; i < currentLevel->enemies.size(); i++)
	{
		if(currentLevel->enemies[i]->IsExpired())
		{
			delete currentLevel->enemies[i];
			currentLevel->enemies.erase(currentLevel->enemies.begin() + i);
			i--;
		}
	}
	for(size_t i = 0; i < currentLevel->scorepoints.size(); i++)
	{
		if(circleIntersect(currentLevel->scorepoints[i].pos.x, currentLevel->scorepoints[i].pos.y, 4,
			player->GetX(), player->GetY(), 5))
		{
			gMain->score += 20;
			currentLevel->scorepoints.erase(currentLevel->scorepoints.begin() + i);
			i--;
		}
	}
}

void Game::drawEntities()
{	
	for(size_t i = 0; i < currentLevel->powers.size(); i++)
	{
		switch(currentLevel->powers[i].type)
		{
		case SMALL:
			res->GetSprite("powersmall")->RenderEx(currentLevel->powers[i].x, currentLevel->powers[i].y, currentLevel->powers[i].angle);
			break;
		case LARGE:
			res->GetSprite("powerlarge")->RenderEx(currentLevel->powers[i].x, currentLevel->powers[i].y, currentLevel->powers[i].angle);
			break;
		}
	}

	for(size_t i = 0; i < currentLevel->enemies.size(); i++)
	{
		currentLevel->enemies[i]->Render();
	}
res->GetParticleSystem("playershot")->Render();
	
	for(size_t i = 0; i < currentLevel->scorepoints.size(); i++)
	{
		res->GetSprite("scorepoint")->Render(currentLevel->scorepoints[i].pos.x, currentLevel->scorepoints[i].pos.y);
	}
	player->Render();

	for(int i = 0; i < MAX_BULLETS; i++)
	{
		if(currentLevel->bullets[i].anim >= 0)
		{
			if(currentLevel->bullets[i].isFriendly)
			{
				playerShotSprites[(int)currentLevel->bullets[i].anim]->SetColor(ARGB(currentLevel->bullets[i].alpha,255,255,255));;
				playerShotSprites[(int)currentLevel->bullets[i].anim]->RenderEx(currentLevel->bullets[i].x, currentLevel->bullets[i].y, currentLevel->bullets[i].angle + currentLevel->bullets[i].spriteangle);
			}
			else
			{
				bulletSprites[(int)currentLevel->bullets[i].anim]->SetBlendMode(currentLevel->bullets[i].blend);
				bulletSprites[(int)currentLevel->bullets[i].anim]->SetColor(ARGB(currentLevel->bullets[i].alpha,255,255,255));;
				bulletSprites[(int)currentLevel->bullets[i].anim]->RenderEx(currentLevel->bullets[i].x, currentLevel->bullets[i].y, currentLevel->bullets[i].angle + currentLevel->bullets[i].spriteangle, currentLevel->bullets[i].radius * spriteRadiiInv[int(currentLevel->bullets[i].anim)/NUM_COLORS]);
			}
		}
	}

	laser* t = 0;
	for(int i = 0; i < MAX_BULLETS; i++)
	{
		t = &currentLevel->lasers[i];
		if(t->anim >= 0)
		{
			bulletSprites[(int)t->anim]->SetBlendMode(t->blend);

			//for(float d = 0; d <= t->length; d += t->radius)
			//{
			bulletSprites[(int)t->anim]->Render4V(t->x + cosf(t->angle-M_PI_2)*t->radius, 
				t->y + sinf(t->angle-M_PI_2)*t->radius,
				t->x + cosf(t->angle+M_PI_2)*t->radius,
				t->y + sinf(t->angle+M_PI_2)*t->radius,
				t->x + cosf(t->angle+M_PI)*t->length + cosf(t->angle+M_PI-M_PI_2)*t->radius,
				t->y + sinf(t->angle+M_PI)*t->length + sinf(t->angle+M_PI-M_PI_2)*t->radius,
				t->x + cosf(t->angle+M_PI)*t->length + cosf(t->angle+M_PI+M_PI_2)*t->radius, 
				t->y + sinf(t->angle+M_PI)*t->length + sinf(t->angle+M_PI+M_PI_2)*t->radius);
			//	t->anim->RenderEx(t->x+cosf(t->angle + M_PI)*d, t->y+sinf(t->angle + M_PI)*d, t->angle, 1, 2*t->radius/t->anim->GetHeight());
			//}	
			//t->anim->RenderEx(t->x+cosf(t->angle + M_PI)*t->length, t->y+sinf(t->angle + M_PI)*t->length, t->angle, 1, 2*t->radius/t->anim->GetHeight());
		}
	}

	curvedlaser* c = 0;
	std::list<vec2>::iterator k,start,end;
	float diffA=0,oldX=0,oldY=0;
	for(int i = 0; i < MAX_CURVEDLASERS; i++)
	{
		c = &currentLevel->curvedlasers[i];
		diffA = c->angle;
		if(c->anim >= 0 && c->points.size() >= 2)
		{
			bulletSprites[(int)c->anim]->SetBlendMode(c->blend);
			start = c->points.begin();
//			start--;
			end = c->points.end();
			end--;

			for(k = end;; )
			{
				if(k == end)
					diffA = c->angle;
				else
					diffA = atan2(oldY - k->y, oldX - k->x);
				
				bulletSprites[(int)c->anim]->RenderEx(k->x, k->y, diffA, 1, 2*c->radius/bulletSprites[(int)c->anim]->GetHeight());
				if(k == start)
					break;
				oldX = k->x;
				oldY = k->y;
				k--;
			}
		}
	}

	beam* b = 0;
	for(int i = 0; i < MAX_BULLETS*0.25; i++)
	{
		b = &currentLevel->beams[i];
		if(b->anim >= 0)
		{
			beamSprites[(int)b->anim]->SetBlendMode(b->blend);

			//for(float d = 0; d <= t->length; d += t->radius)
			//{
			//beamSprites[(int)b->anim]->SetColor(ARGB(b->alpha, 255, 255, 255));
			if(b->framesAlive <= b->chargetime)
				hgeCreate(HGE_VERSION)->Gfx_RenderLine(b->x, b->y, b->x + cosf(b->angle)*b->length, b->y + sinf(b->angle)*b->length);
			else
				beamSprites[(int)b->anim]->RenderEx(b->x, b->y, b->angle, b->length / beamSprites[(int)b->anim]->GetWidth(), b->radius *beamRadiiInv[(int)b->anim]);
				/*beamSprites[(int)b->anim]->Render4V(b->x + cosf(b->angle+M_PI_2)*b->radius, 
				b->y + sinf(b->angle+M_PI_2)*b->radius,
				b->x + cosf(b->angle-M_PI_2)*b->radius,
				b->y + sinf(b->angle-M_PI_2)*b->radius,
				b->x + cosf(b->angle)*b->length + cosf(b->angle-M_PI_2)*b->radius,
				b->y + sinf(b->angle)*b->length + sinf(b->angle-M_PI_2)*b->radius,
				b->x + cosf(b->angle)*b->length + cosf(b->angle+M_PI_2)*b->radius, 
				b->y + sinf(b->angle)*b->length + sinf(b->angle+M_PI_2)*b->radius);*/
			//	b->anim->RenderEx(b->x+cosf(b->angle + M_PI)*d, b->y+sinf(b->angle + M_PI)*d, b->angle, 1, 2*b->radius/b->anim->GetHeight());
			//}	
			//t->anim->RenderEx(t->x+cosf(t->angle + M_PI)*t->length, t->y+sinf(t->angle + M_PI)*t->length, t->angle, 1, 2*t->radius/t->anim->GetHeight());
		}
	}

	for(size_t i = 0; i < currentLevel->anims.size(); i++)
		for(size_t j = 0; j < 2; j++)
		{
			if(!currentLevel->anims[i].anim[j])
				continue;

			if(j == 0)
				currentLevel->anims[i].anim[j]->SetColor(ARGB(currentLevel->anims[i].a, 255, 255, 255));
			else
				currentLevel->anims[i].anim[j]->SetColor(ARGB(currentLevel->anims[i].a, currentLevel->anims[i].r,  currentLevel->anims[i].g,  currentLevel->anims[i].b));
			
			currentLevel->anims[i].anim[j]->RenderEx(currentLevel->anims[i].x, currentLevel->anims[i].y, currentLevel->anims[i].angle, currentLevel->anims[i].scale);	
		}
}

void Game::checkCollisions()
{
	for(int i = 0; i < MAX_BULLETS; i++)
	{
		if(currentLevel->bullets[i].anim >= 0)
		{
			if(currentLevel->bullets[i].isFriendly)
			{				
				//check against other enemies
				for(size_t j = 0; j < currentLevel->enemies.size(); j++)
				{
					if(currentLevel->enemies[j]->GetActiveTime() < 0)
						continue;

					if(circleIntersect(currentLevel->bullets[i].x, currentLevel->bullets[i].y, spriteHitboxRadii[int(currentLevel->bullets[i].anim) / NUM_COLORS] * currentLevel->bullets[i].radius*spriteRadiiInv[int(currentLevel->bullets[i].anim)/NUM_COLORS], currentLevel->enemies[j]->GetX(), currentLevel->enemies[j]->GetY(), currentLevel->enemies[j]->GetRadius()))
					{
						killBullet(&currentLevel->bullets[i], 0);
						res->GetParticleSystem("playershot")->FireAt(currentLevel->bullets[i].x, currentLevel->bullets[i].y);
						currentLevel->enemies[j]->TakeDamage(currentLevel->bullets[i].damage);
					}
				}
			}
			else
			{
				if(circleIntersect(currentLevel->bullets[i].x, currentLevel->bullets[i].y, currentLevel->bullets[i].radius, player->GetX(), player->GetY(), 3))
				{
					killBullet(&currentLevel->bullets[i], 0);
					CreateSpawnEffect(player->GetX(), player->GetY(), 0, S04*NUM_COLORS);
				}
				if(!currentLevel->bullets[i].grazed && circleIntersect(currentLevel->bullets[i].x, currentLevel->bullets[i].y, currentLevel->bullets[i].radius, player->GetX(), player->GetY(), 10))
				{
					gMain->score += 20;
					graze++;
					currentLevel->bullets[i].grazed = true;
				}
			}
		}
	}	
	for(int i = 0; i < MAX_BULLETS; i++)
	{
		if(currentLevel->lasers[i].anim >= 0)
		{
			for(float d = 0; d <= currentLevel->lasers[i].length; d += currentLevel->lasers[i].radius)
			{
				if(circleIntersect(player->GetX(), player->GetY(), 15, currentLevel->lasers[i].x+cosf(currentLevel->lasers[i].angle + M_PI)*d, currentLevel->lasers[i].y+sinf(currentLevel->lasers[i].angle + M_PI)*d,  currentLevel->lasers[i].radius))
				{
					currentLevel->lasers[i].grazeCount++;

					if(currentLevel->lasers[i].grazeCount % 20 == 0)
					{
						gMain->score += 20;
						graze++;
					}
				}
			}
		}
	}
}