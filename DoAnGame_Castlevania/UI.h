#pragma once
#include "d3dx9.h"
#include "define.h"
#include <string>
#include "Sprites.h"
#include <vector>
#include "Simon.h"

class UI
{
private:
	ID3DXFont *font;
	RECT rect;
	Simon *simon;

	std::string information;

	std::vector<CSprite*> *simonHPList;
	std::vector<CSprite*> *enemyHPList;
	std::vector<CSprite*> *noHPList;

	CSprite* heart;
	CSprite* weapon;
	CSprite* black;
	CSprite* axe;
	CSprite* holyWater;
	CSprite* cross;
	CSprite* knife;
	CSprite* stopWatch;

	CSprite* upgrade1;
	CSprite* upgrade2;

	int simonHP;
	int bossHP;
	int uiHP;

	int score;
	int time;
	int item;
	int energy;
	int life;
	int stage;
	int subWeapon;

public:
	UI();
	~UI();
	bool Initialize();
	void Update(int bossHP, int time, int life, int stage);
	void Render();
};

