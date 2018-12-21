#include "UI.h"

UI::UI()
{
	simonHP = NULL;
	simonHP = NULL;
}


UI::~UI()
{
	
}

bool UI::Initialize(LPDIRECT3DDEVICE9 d3ddv, Simon * simon)
{
	CTextures * textures = CTextures::GetInstance();	
	heart = new CSprite(1, 0, 33, 16, 50, textures->Get(ID_TEX_UI));
	pinkrect = new CSprite(2, 65, 37, 131, 82, textures->Get(ID_TEX_UI));
	axe = new CSprite(3, 32, 0, 64, 32, textures->Get(ID_TEX_UI));
	holyWater = new CSprite(4, 96, 0, 128, 32, textures->Get(ID_TEX_UI));
	cross = new CSprite(5, 0, 0, 32, 32, textures->Get(ID_TEX_UI));
	knife = new CSprite(6, 64, 6, 96, 25, textures->Get(ID_TEX_UI));
	stopWatch = new CSprite(7, 128, 0, 160, 32, textures->Get(ID_TEX_UI));
	noHP = new CSprite(10,51,37,58,50,textures->Get(ID_TEX_UI));

	for (int i = 0; i < 16; i++)
	{
		CSprite* sprite = new CSprite(8, 23, 37, 30, 50, textures->Get(ID_TEX_UI));

		simonHPList.push_back(sprite);
	}

	for (int i = 0; i < 16; i++)
	{
		CSprite* sprite = new CSprite(9, 37, 37, 44, 50, textures->Get(ID_TEX_UI));
		enemyHPList.push_back(sprite);
	}

	font = NULL;
	AddFontResourceEx(GAME_FONT, FR_PRIVATE, NULL);
	HRESULT result = D3DXCreateFont(
		d3ddv, 20, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &font);

	if (!SUCCEEDED(result))
		return false;
	SetRect(&rect, 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT);
	information = "SCORE_000000 TIME 0000 STAGE 00\n";
	information += "PLAYER                =62\n";
	information += "ENEMY                 P=3\n";
	return false;
}

//void UI::Update(int bossHP, int time, int life, int stage)
void UI::Update( int time, int stage,Simon *simon)
{
	this->time = time;
	this->stage = stage;


	std::string scoreString = std::to_string(simon->GetScore());
	while (scoreString.length() < 6)
		scoreString = "0" + scoreString;


	std::string timeString = std::to_string(this->time);
	while (timeString.length() < 4)
		timeString = "0" + timeString;


	std::string stageString = std::to_string(this->stage);
	while (stageString.length() < 2)
		stageString = "0" + stageString;

	information = "SCORE_" + scoreString +  " TIME " + timeString + " STAGE " + stageString + "\n";
	information += "PLAYER                =" + std::to_string(simon->GetHeart()) + "\n";
	information += "ENEMY                P=" + std::to_string(simon->GetLife()) + "\n";
}

void UI::Render(float x,float y,Simon *simon,int bossheath,int lv)
{	
	if (font)
		font->DrawTextA(NULL, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	for (int i = 0; i < simon->preHP; i++)
	{
		simonHPList[i]->Draw(x + 130 + 11 * i, 42 , 255);
	}
	for (int i = 0; i <16 - simon->preHP; i++)
	{
		noHP->Draw(x + 295 - 11 * i, 42, 255);
	}
	if (lv == 99 || lv == 31)
	{
		for (int i = 0; i < bossheath; i++)
		{
			enemyHPList[i]->Draw(x + 130 + 11 * i, 62, 255);
		}
	
		for (int i = 0; i < 16 - bossheath; i++)
		{
			noHP->Draw(x + 295 - 11 * i, 62, 255);
		}
	}
	else
		for (int i = 0; i < 16; i++)
		{
			enemyHPList[i]->Draw(x + 130 + 11 * i, 62, 255);
		}
	heart->Draw(x + SCREEN_WIDTH / 2 + 105 ,40, 255);
	pinkrect->Draw(x + SCREEN_WIDTH / 2 , 40, 255);
	if (simon->currentWeapon != NULL)
	{
		int weapon = simon->currentWeapon;
		{
			switch (weapon)
			{
			case ITEM_KNIFE:
				knife->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;
			case ITEM_AXE:
				axe->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;
			case ITEM_HOLYWATER:
				holyWater->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;
			case ITEM_CROSS:
				cross->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;
			case ITEM_CLOCK:
				stopWatch->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;
			}
		}
	}
}


