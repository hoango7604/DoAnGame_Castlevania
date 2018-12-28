#include "Sprites.h"
#include "Game.h"
#include "debug.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

CSprites * CSprites::__instance = NULL;

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprite::Draw(float x, float y, int alpha)
{
	CGame * game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha);
}

void CSprites::Add(LPCSTR fileSource, LPDIRECT3DTEXTURE9 tex)
{
	fstream pFile;
	pFile.open(fileSource, fstream::in);
	string lineString;
	int *posSpace = NULL;
	string subString;
	int id, left, top, bottom, right;

	while (pFile.good())
	{
		getline(pFile, lineString);
		stringstream ss;
		ss.str(lineString);
		ss >> id >> left >> top >> right >> bottom;
		LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
		sprites[id] = s;

	}
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void CAnimation::ResetFrame()
{
	currentFrame = frames.size() - 1;
	lastFrameTime = GetTickCount();
}

CAnimations * CAnimations::__instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(LPCSTR fileSource)
{

	fstream pFile;
	pFile.open(fileSource, fstream::in);
	string linestring;
	int number_of_sprite;
	int id;
	DWORD time;
	while (pFile.good())
	{

		getline(pFile, linestring);
		if (linestring == "")
			return;
		stringstream ss;
		ss.str(linestring);
		ss >> number_of_sprite >> time >> id;
		int *a = new int[number_of_sprite];
		LPANIMATION ani;
		ani = new CAnimation(time);
		for (int j = 0; j < number_of_sprite; j++)
		{			
			/*for (int i = 0; i < 4 + j; i++)
			{*/
				ss >> a[j];
			//}
			
			ani->Add(a[j]); 
		}
		animations[id] = ani;
		
		
		//delete a;
	}
}

LPANIMATION CAnimations::Get(int id)
{
	return animations[id];
}