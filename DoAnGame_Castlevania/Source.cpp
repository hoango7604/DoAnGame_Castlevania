#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "GridObjects.h"

#include "Simon.h"
#include "Ground.h"
#include "define.h"
#include "Zombie.h"
#include "BigFire.h"
#include "Candle.h"
#include "Item.h"
#include "Effect.h"
#include "Map.h"
#include "Stair.h"
#include "RedBat.h"
#include "Panther.h"
#include "MerMan.h"
#include "CheckStair.h"
#include "CheckPoint.h"
#include "UI.h"
#include "BossBat.h"

CGame *game;
Simon * simon;
Item *item;
Effect *whipEffect;
Map *map;
UI * ui;
CSprite *sprite;
ListGrids *listGrids;
vector<GridObjects*> currentGrids;

bool lv1 = true;
bool lv2 = false;
bool lv2_1 = false;
bool lv2_2 = false;
bool boss = false;
// check scene lv2->lv2_1
bool checkScene = false;
bool check1 = false;
bool check = false;
//check lv2->lv2_2
bool temp = false;
//check scene lv2_1 ->boss
bool checkScene1 = false;
bool check3 = false;
bool countLoadResourceLv2 = false;
bool countLoadResourceLv2_1 = false;
bool countLoadResourceLv2_2 = false;
bool countLoadResourceboss = false;
bool isEnableKeyBoard = true;
DWORD timer; // load enemy lv2
DWORD timer2;//load enemy boss
DWORD gameTime = 999000;
CSprites * sprites = CSprites::GetInstance();
CAnimations * animations = CAnimations::GetInstance();
CTextures * textures = CTextures::GetInstance();

class CSampleKeyHander : public CKeyEventHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	if (isEnableKeyBoard && !simon->isHurt)
	{
		// Nhay
		if (KeyCode == DIK_SPACE)
		{
			if (simon->isJump == false && simon->isSit == false && simon->isAttack == false && simon->isOnStair == false)
				simon->SetAction(SIMON_ACTION_JUMP);
		}
		// Danh
		else if (KeyCode == DIK_A)
		{
			if (simon->isAttack == false)
			{
				simon->SetAction(SIMON_ACTION_ATTACK);
				simon->isUseWhip = true;
			}
		}
		// Dùng vũ khí
		else if (KeyCode == DIK_S)
		{
			if (simon->isAttack == false)
			{
				simon->SetAction(SIMON_ACTION_ATTACK);
				simon->isUseWhip = false;
			}
		}
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	if (isEnableKeyBoard && !simon->isHurt)
	{
		// Len xuong cau thang
		if (KeyCode == DIK_UP)
		{
			if (simon->isOnCheckStairUp)
			{
				simon->SetState(SIMON_STATE_IDLE);
			}
			else if (simon->isOnStair)
			{
				simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
			}
		}

		// Ngoi
		if (KeyCode == DIK_DOWN)
		{
			if (simon->isOnCheckStairDown)
			{
				simon->SetState(SIMON_STATE_IDLE);
			}
			else if (simon->isOnStair)
			{
				simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
			}
			else if (simon->isSit)
			{
				if (!simon->isAttack)
				{
					simon->isSit = false;
				}
				else
				{
					simon->isExitSit = true;
				}
			}
		}

		// Di bo
		else if (KeyCode == DIK_RIGHT || KeyCode == DIK_LEFT)
		{
			simon->isMoving = false;
			simon->vx = 0;
		}
	}
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	if (isEnableKeyBoard && !simon->isHurt)
	{
		// Len xuong cau thang
		if (game->IsKeyDown(DIK_UP))
		{
			if (simon->ny == -1 && !simon->isOnStair)
			{
				if (!simon->isAttack && !simon->isSit && !simon->isJump)
				{
					simon->SetState(SIMON_STATE_ONCHECKSTAIR);
				}
			}
			else if (simon->isOnStair && !simon->isAttack)
			{
				simon->ny = -1;
				simon->SetState(SIMON_STATE_ONSTAIR);
				if (simon->isLeftToRight)
					simon->nx = 1;
				else
					simon->nx = -1;
			}
			else if (simon->isOnStair && simon->isAttack)
			{
				simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
			}
		}

		// Ngoi
		if (game->IsKeyDown(DIK_DOWN))
		{
			if (simon->ny == 1 && !simon->isOnStair)
			{
				if (!simon->isAttack && !simon->isSit && !simon->isJump)
				{
					simon->SetState(SIMON_STATE_ONCHECKSTAIR);
				}
			}
			else if (simon->isOnStair && !simon->isAttack)
			{
				simon->ny = 1;
				simon->SetState(SIMON_STATE_ONSTAIR);
				if (simon->isLeftToRight)
					simon->nx = -1;
				else
					simon->nx = 1;
			}
			else if (simon->isOnStair && simon->isAttack)
			{
				simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
			}
			else if (!simon->isOnCheckStairDown && !simon->isOnStair && !simon->isAttack && !simon->isJump)
				simon->SetState(SIMON_STATE_SIT);
		}

		// Di bo
		if (game->IsKeyDown(DIK_RIGHT))
		{
			if (!simon->isSit && !simon->isOnStair && !simon->isAttack)
				simon->SetState(SIMON_STATE_WALK);
			if (!simon->isJump && !simon->isOnStair && !simon->isAttack)
				simon->nx = 1.0f;
		}
		else if (game->IsKeyDown(DIK_LEFT))
		{
			if (!simon->isSit && !simon->isOnStair && !simon->isAttack)
				simon->SetState(SIMON_STATE_WALK);
			if (!simon->isJump && !simon->isOnStair && !simon->isAttack)
				simon->nx = -1.0f;
		}

		// Neu khong co gi xay ra se dung im
	}
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void LoadResourceLv1()
{
	// Đặt vị trí xuất phát cho simon
	simon->SetPosition(1400, 327); // 100 327

	// Khởi tạo listGrids
	listGrids->InitList(MAX_WIDTH_LV1);

#pragma region Ground
	for (int i = 0; i < 50; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(0 + i * 32.0f, 407);
		listGrids->AddObject(ground);
	}
#pragma endregion

#pragma region BigFire
	BigFire *bigfire;

	bigfire = new BigFire();
	bigfire->AddAnimation(700);
	bigfire->SetPosition(335, 340);
	listGrids->AddObject(bigfire);

	bigfire = new BigFire();
	bigfire->AddAnimation(700);
	bigfire->SetPosition(464, 340);
	listGrids->AddObject(bigfire);

	bigfire = new BigFire();
	bigfire->AddAnimation(700);
	bigfire->SetPosition(657, 340);
	listGrids->AddObject(bigfire);

	bigfire = new BigFire();
	bigfire->AddAnimation(700);
	bigfire->SetPosition(851, 340);
	listGrids->AddObject(bigfire);

	bigfire = new BigFire();
	bigfire->AddAnimation(700);
	bigfire->SetPosition(1090, 340);
	listGrids->AddObject(bigfire);

	bigfire = new BigFire();
	bigfire->AddAnimation(700);
	bigfire->SetPosition(1267, 340);
	listGrids->AddObject(bigfire);
#pragma endregion

#pragma region BigFire
	CheckPoint *checkPoint;

	checkPoint = new CheckPoint();
	checkPoint->SetType(CHECKPOINT_LEVELUP);
	checkPoint->SetPosition(1377, 374);
	listGrids->AddObject(checkPoint);

	checkPoint = new CheckPoint();
	checkPoint->SetType(CHECKPOINT_BONUS);
	checkPoint->SetPosition(1410, 374);
	listGrids->AddObject(checkPoint);
#pragma endregion
}

void LoadResourceLv2() {
	for (int i = 0; i < 95; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(0 + i * 32.0f, 440);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1377 + i * 32.0f, 310);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1503 + i * 32.0f, 244);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1854 + i * 32.0f, 310);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(2782 + i * 32.0f, 246);
		listGrids->AddObject(ground);
	}

	for (int i = 0; i < 5; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(65 + i * 255, 376);
		listGrids->AddObject(candle);
	}
	for (int i = 0; i < 4; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(195 + i * 257, 316);
		listGrids->AddObject(candle);
	}

	Candle *candle;
	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1219, 326);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1339, 193);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1445, 380);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1800, 380);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1713, 190);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1986, 198);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(2601, 387);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(2742, 322);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(2868, 190);
	listGrids->AddObject(candle);

	for (int i = 0; i < 4; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(2050 + i * 125, 380);
		listGrids->AddObject(candle);
	}

	for (int i = 0; i < 4; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(801);
		stair->SetPosition(1247 + i * 32, 405 - i * 32);
		listGrids->AddObject(stair);
	}
	for (int i = 0; i < 2; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(801);
		stair->SetPosition(1439 + i * 32.0, 278 - i * 32);
		listGrids->AddObject(stair);
	}

	for (int i = 0; i < 2; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(802);
		stair->SetPosition(1824 + i * 32.0, 246 + i * 32);
		listGrids->AddObject(stair);

	}
	for (int i = 0; i < 6; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(801);
		stair->SetPosition(2590 + i * 32.0, 410 - i * 32);
		listGrids->AddObject(stair);
	}

	CheckStair *checkstair;
	// Bottom right
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(1255, 407);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	// Top left
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1382, 215);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	// Bottom right
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(1448, 278);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	// Top left
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1510, 151);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	// Top right
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1790, 151);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	// Bottom left
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(1850, 278);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	// Bottom right
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(2595, 407);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	// Top left
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(2788, 153);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	Panther *panther;
	panther = new Panther();
	panther->AddAnimation(605);
	panther->AddAnimation(606);
	panther->AddAnimation(607);
	panther->AddAnimation(608);
	panther->SetPosition(1403, 260);
	panther->SetState(PANTHER_STATE_WAIT);
	listGrids->AddObject(panther);

	panther = new Panther();
	panther->AddAnimation(605);
	panther->AddAnimation(606);
	panther->AddAnimation(607);
	panther->AddAnimation(608);
	panther->SetPosition(1700, 200);
	panther->SetState(PANTHER_STATE_WAIT);
	listGrids->AddObject(panther);

	panther = new Panther();
	panther->AddAnimation(605);
	panther->AddAnimation(606);
	panther->AddAnimation(607);
	panther->AddAnimation(608);
	panther->SetPosition(1950, 260);
	panther->SetState(PANTHER_STATE_WAIT);
	listGrids->AddObject(panther);
}

void LoadResourceLv2_1()
{
	for (int i = 0; i < 9; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + i * 32 + 32 * 8, 246);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 16; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 9 + i * 32.0f, 440);
		listGrids->AddObject(ground);
	}
	
	for (int i = 0; i < 12; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 29 + i * 32.0f, 440);
		listGrids->AddObject(ground);
	}
	
	for (int i = 0; i < 3; i++)//
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 17 + i * 32.0f, 246 + 64);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 29 + i * 32.0f, 246 + 32);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 31 + i * 32.0f, 246 + 64);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 35 + 3 + i * 32.0f, 246);
		listGrids->AddObject(ground);
	}

	CheckStair *checkstair = new CheckStair();
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(3139, 360);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(3392, 218);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(3780, 360);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(3520, 406);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(3839, 218);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(3950, 406);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	Candle *candle;
	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3245, 193);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3389, 159);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3137, 386);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3519, 319);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3684, 155);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3776, 385);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3905, 150);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(4035, 192);
	listGrids->AddObject(candle);
}

void LoadResourceLv2_2()
{
	CheckStair * checkstair;
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(154, 182);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(860, 248);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(192 + i * 32.0f, 216);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 14; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(0 + i * 32.0f, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(512 + i * 32.0f, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(640 + i * 32.0f, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(960 + i * 32.0f, 344);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(896 + i * 32.0f, 408);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 16; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 9 + i * 32.0f, 440);
		listGrids->AddObject(ground);
	}
}

void LoadResourceboss()
{
	for (int i = 0; i < 8; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 40 + i * 32.0f, 246);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 48; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 41 + i * 32.0f, 438);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 11; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 49 + i * 32.0f, 310);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 86 + i * 32.0f, 310);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 83 + i * 32.0f, 374);
		listGrids->AddObject(ground);
	}

	Candle *candle;
	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(4285, 194);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(4418, 255);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(4362, 380);
	listGrids->AddObject(candle);

	for (int i = 0; i < 4; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(4866 + i * 127, 380);
		listGrids->AddObject(candle);
	}

	for (int i = 0; i < 2; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(5438 + i * 127, 310);
		listGrids->AddObject(candle);
	}

	BossBat *bossbat = new BossBat();
	bossbat->AddAnimation(609);
	bossbat->AddAnimation(610);
	bossbat->SetPosition(5325, 125);
	bossbat->SetState(BOSSBAT_STATE_WAIT);
	listGrids->AddObject(bossbat);
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	textures->Add(ID_TEX_SIMON, L"Castlevania\\Simon_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIMON_2, L"Castlevania\\Simon_ver_editted.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LV1, L"Castlevania\\lv1.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_LV1_2, L"Castlevania\\maplv2.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_BRICK, L"Castlevania\\2.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_BRICK2, L"Castlevania\\BRICK1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ZOMBIE, L"Castlevania\\ZOMBIE.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ZOMBIE_RIGHT, L"Castlevania\\ZOMBIE_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_PANTHER, L"Castlevania\\PANTHER.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_PANTHER_RIGHT, L"Castlevania\\PANTHER_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_FIRE, L"Castlevania\\123.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"Castlevania\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP, L"Castlevania\\WHIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP_2, L"Castlevania\\WHIP_left.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET, L"Castlevania\\tileset.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET_2, L"Castlevania\\tileset2.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET_3, L"Castlevania\\tileset3.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"Castlevania\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LADDER, L"Castlevania\\3.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LADDER_LEFT, L"Castlevania\\3_.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BBOX, L"Castlevania\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_STAIR_BOT, L"Castlevania\\stair_bottom.png", D3DCOLOR_XRGB(0, 0, 255));//
	textures->Add(ID_TEX_STAIR_TOP, L"Castlevania\\stair_top.png", D3DCOLOR_XRGB(0, 0, 255));
	textures->Add(ID_TEX_MERMAN_LEFT, L"Castlevania\\MERMAN.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MERMAN_RIGHT, L"Castlevania\\MERMAN_right.png", D3DCOLOR_XRGB(255, 0, 255));	
	textures->Add(ID_TEX_BAT, L"Castlevania\\BAT.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BOSS, L"Castlevania\\VAMPIRE_BAT.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_AXE_ACTION, L"Castlevania\\AXE_ACTION.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_HOLY_WATER_ACTION, L"Castlevania\\HOLY_WATER_ACTION.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CROSS_ACTION, L"Castlevania\\CROSS_ACTION.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_KNIFE_ACTION, L"Castlevania\\KNIFE.png", D3DCOLOR_XRGB(255, 0, 255));
	//ui
	textures->Add(ID_TEX_UI, L"Castlevania\\UI.png", D3DCOLOR_XRGB(255, 0, 255));

	//effect and item
	textures->Add(ID_TEX_EFFECT1, L"Castlevania\\0.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_EFFECT2, L"Castlevania\\DEAD.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP_VIP, L"Castlevania\\WHIP_VIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MONEY, L"Castlevania\\money_bag_red.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ROSARY, L"Castlevania\\ROSARY.png", D3DCOLOR_XRGB(255, 0, 255));

	
	
#pragma region Addsprite
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	sprites->Add(10001, 436, 0, 484, 64, texSimon);		// đứng im phải

	sprites->Add(10002, 260, 0, 308, 64, texSimon);		// đi phải
	sprites->Add(10003, 314, 0, 362, 64, texSimon);
	sprites->Add(11002, 378, 0, 426, 64, texSimon);
	sprites->Add(11003, 436, 0, 484, 64, texSimon);

	sprites->Add(10004, 120, 0, 168, 64, texSimon);		// đánh phải			
	sprites->Add(10005, 60, 0, 108, 64, texSimon);
	sprites->Add(10006, 0, 0, 64, 64, texSimon);

	sprites->Add(10028, 0, 67, 49, 130, texSimon);		// ngồi đánh phải			
	sprites->Add(10029, 420, 134, 469, 197, texSimon);
	sprites->Add(10030, 360, 134, 421, 197, texSimon);

	sprites->Add(10034, 120, 134, 168, 197, texSimon);		// trên thang phải đánh phải			
	sprites->Add(10035, 60, 134, 109, 197, texSimon);
	sprites->Add(10036, 0, 134, 59, 197, texSimon);

	sprites->Add(10040, 302, 134, 350, 197, texSimon);		// trên thang trái đánh phải			
	sprites->Add(10041, 242, 134, 290, 197, texSimon);
	sprites->Add(10042, 182, 134, 239, 197, texSimon);

	sprites->Add(10046, 133, 67, 165, 130, texSimon); // lên thang phải
	sprites->Add(10047, 196, 67, 228, 130, texSimon);

	sprites->Add(10052, 133, 67, 165, 130, texSimon); //xuống thang phải
	sprites->Add(10053, 318, 67, 350, 130, texSimon);

	sprites->Add(10018, 196, 0, 230, 64, texSimon); // ngồi phải

	sprites->Add(10020, 196, 0, 230, 64, texSimon); // nhảy phải
	
	sprites->Add(10054, 436, 67, 471, 130, texSimon);//bị đánh từ bên trái

	LPDIRECT3DTEXTURE9 texSimon2 = textures->Get(ID_TEX_SIMON_2);

	sprites->Add(10011, 12, 0, 44, 64, texSimon2);		// đứng im trái

	sprites->Add(10012, 196, 0, 244, 64, texSimon2);		// đi trái
	sprites->Add(10013, 136, 0, 184, 64, texSimon2);
	sprites->Add(11012, 78, 0, 126, 64, texSimon2);
	sprites->Add(11013, 12, 0, 60, 64, texSimon2);

	sprites->Add(10014, 301, 0, 360, 64, texSimon2);		// đánh trái				
	sprites->Add(10015, 361, 0, 404, 64, texSimon2);
	sprites->Add(10016, 421, 0, 464, 64, texSimon2);

	sprites->Add(10031, 420, 67, 477, 130, texSimon2);		// ngồi đánh trái				
	sprites->Add(10032, 0, 134, 44, 197, texSimon2);
	sprites->Add(10033, 60, 134, 104, 197, texSimon2);

	sprites->Add(10037, 120, 134, 177, 197, texSimon2);		// trên thang phải đánh trái			
	sprites->Add(10038, 180, 134, 225, 197, texSimon2);
	sprites->Add(10039, 240, 134, 282, 197, texSimon2);

	sprites->Add(10043, 300, 134, 361, 197, texSimon2);		// trên thang trái đánh trái			
	sprites->Add(10044, 360, 134, 405, 197, texSimon2);
	sprites->Add(10045, 420, 134, 464, 197, texSimon2);

	sprites->Add(10048, 190, 67, 224, 130, texSimon2); // xuống thang trái
	sprites->Add(10049, 129, 67, 163, 130, texSimon2);

	sprites->Add(10050, 190, 67, 224, 130, texSimon2); //lên thang trái
	sprites->Add(10051, 252, 67, 286, 130, texSimon2);

	sprites->Add(10019, 252, 0, 284, 64, texSimon2); // ngồi trái

	sprites->Add(10021, 256, 0, 286, 64, texSimon2);//nhảy trái

	sprites->Add(10055, 12, 70, 41, 130, texSimon2);//bị đánh từ bên phải

	sprites->Add(10099, 180, 237, 240, 264, texSimon);		// chết 

	LPDIRECT3DTEXTURE9 whipR = textures->Get(ID_TEX_WHIP);

	sprites->Add(10022, 570, 200, 554, 267, whipR);			//roi lv2 phải
	sprites->Add(10023, 342, 260, 312, 316, whipR);
	sprites->Add(10024, 342, 328, 458, 364, whipR); // 116

	sprites->Add(10062, 570, 200, 554, 267, whipR);			//roi lv1 phải	
	sprites->Add(10063, 342, 260, 312, 316, whipR);
	sprites->Add(10064, 112, 65, 189, 100, whipR); 
	
	sprites->Add(10056, 570, 0, 553, 67, whipR);			//roi lv0 phải	
	sprites->Add(10057, 346, 0, 312, 49, whipR);
	sprites->Add(10058, 114, 0, 188, 30, whipR);			// 74

	LPDIRECT3DTEXTURE9 whipL = textures->Get(ID_TEX_WHIP_2);

	sprites->Add(10025, 105, 70, 165, 133, whipL);			//roi lv2 trái
	sprites->Add(10026, 340, 65, 406, 117, whipL);
	sprites->Add(10027, 575, 330, 458, 364, whipL);

	sprites->Add(10065, 105, 70, 165, 133, whipL);			//roi lv1 trái	
	sprites->Add(10066, 340, 65, 406, 117, whipL);
	sprites->Add(10067, 572, 65, 530, 100, whipL); 

	sprites->Add(10059, 110, 0, 166, 66, whipL);			//roi lv0 trái
	sprites->Add(10060, 349, 0, 408, 52, whipL);
	sprites->Add(10061, 585, 0, 529, 30, whipL);

	LPDIRECT3DTEXTURE9 axe = textures->Get(ID_TEX_AXE_ACTION);
	sprites->Add(10068, 0, 0, 30, 28, axe);
	sprites->Add(10069, 30, 0, 60, 28, axe);
	sprites->Add(10070, 60, 0, 90, 28, axe);
	sprites->Add(10071, 90, 0, 120, 28, axe);

	LPDIRECT3DTEXTURE9 holywater = textures->Get(ID_TEX_HOLY_WATER_ACTION);
	sprites->Add(10072, 7, 3, 24, 20, holywater);
	sprites->Add(10073, 42, 2, 54, 25, holywater);
	sprites->Add(10074, 63, 0, 96, 26, holywater);

	LPDIRECT3DTEXTURE9 cross = textures->Get(ID_TEX_CROSS_ACTION);
	sprites->Add(10075, 0, 0, 26, 28, cross);
	sprites->Add(10076, 29, 0, 56, 28, cross);
	sprites->Add(10077, 55, 1, 84, 28, cross);

	LPDIRECT3DTEXTURE9 knife = textures->Get(ID_TEX_KNIFE_ACTION);
	sprites->Add(10078, 0, 0, 32, 18, knife);


	LPDIRECT3DTEXTURE9 knife1 = textures->Get(ID_TEX_KNIFE_ACTION);
	sprites->Add(10079, 0, 0, 32, 18, knife1);

	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_BRICK);
	sprites->Add(20001, 0, 0, 32, 32, texMisc);

	LPDIRECT3DTEXTURE9 texMisc2 = textures->Get(ID_TEX_BRICK2);
	sprites->Add(20002, 0, 0, 32, 32, texMisc2);

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ZOMBIE);
	sprites->Add(30001, 0, 0, 32, 64, texEnemy); // đi trái 
	sprites->Add(30002, 36, 0, 68, 64, texEnemy);

	LPDIRECT3DTEXTURE9 texEnemy0 = textures->Get(ID_TEX_ZOMBIE_RIGHT);
	sprites->Add(30003, 0, 0, 32, 64, texEnemy0); // đi Phải 
	sprites->Add(30004, 36, 0, 68, 64, texEnemy0);

	LPDIRECT3DTEXTURE9 texEnemy1 = textures->Get(ID_TEX_PANTHER);
	sprites->Add(30011, 0, 0, 64, 32, texEnemy1); // báo
	sprites->Add(30012, 64, 0, 128, 32, texEnemy1); // báo chạy trái
	sprites->Add(30013, 128, 0, 192, 32, texEnemy1);
	sprites->Add(30014, 192, 0, 256, 32, texEnemy1);

	LPDIRECT3DTEXTURE9 texEnemy10 = textures->Get(ID_TEX_PANTHER_RIGHT);	
	sprites->Add(30015, 128, 0, 192, 32, texEnemy10); //báo  chạy phải
	sprites->Add(30016, 64, 0, 128, 32, texEnemy10);
	sprites->Add(30017, 0, 0, 64, 32, texEnemy10);

	LPDIRECT3DTEXTURE9 texEnemy11 = textures->Get(ID_TEX_BAT);
	sprites->Add(30018, 0, 0, 32, 32, texEnemy11); // dơi // nằm chờ
	sprites->Add(30019, 32, 0, 64, 32, texEnemy11);
	sprites->Add(30020, 64, 0, 96, 32, texEnemy11);
	sprites->Add(30021, 96, 0, 128, 32, texEnemy11);

	LPDIRECT3DTEXTURE9 texEnemy12 = textures->Get(ID_TEX_BOSS);
	sprites->Add(30022, 27, 0, 67, 38, texEnemy12); // boss // nằm chờ
	sprites->Add(30023, 98, 0, 200, 45, texEnemy12);
	sprites->Add(30024, 213, 0, 281, 45, texEnemy12);
	
	LPDIRECT3DTEXTURE9 texEnemy13 = textures->Get(ID_TEX_MERMAN_LEFT);
	sprites->Add(30025, 0, 0, 32, 64, texEnemy13); //người cá trái
	sprites->Add(30026, 32, 0, 64, 64, texEnemy13);
	sprites->Add(30027, 64, 0, 96, 64, texEnemy13);

	LPDIRECT3DTEXTURE9 texEnemy14 = textures->Get(ID_TEX_MERMAN_RIGHT);
	sprites->Add(30028, 64, 0, 96, 64, texEnemy14); // người cá phải
	sprites->Add(30029, 32, 0, 64, 64, texEnemy14);
	sprites->Add(30030, 0, 0, 32, 64, texEnemy14);

	LPDIRECT3DTEXTURE9 texEnemy2 = textures->Get(ID_TEX_FIRE);
	sprites->Add(40011, 0, 0, 32, 64, texEnemy2);
	sprites->Add(40012, 32, 0, 64, 64, texEnemy2);

	LPDIRECT3DTEXTURE9 texEnemy3 = textures->Get(ID_TEX_CANDLE);
	sprites->Add(40013, 0, 0, 16, 32, texEnemy3);
	sprites->Add(40014, 16, 0, 32, 32, texEnemy3);

	LPDIRECT3DTEXTURE9 texMisc3 = textures->Get(ID_TEX_LADDER);
	sprites->Add(40015, 0, 0, 32, 32, texMisc3);

	LPDIRECT3DTEXTURE9 texMisc4 = textures->Get(ID_TEX_LADDER_LEFT);
	sprites->Add(40016, 0, 0, 32, 32, texMisc4);

	LPDIRECT3DTEXTURE9 texMisc5 = textures->Get(ID_TEX_STAIR_BOT);
	sprites->Add(40017, 0, 0, 32, 32, texMisc5);

	LPDIRECT3DTEXTURE9 texMisc6 = textures->Get(ID_TEX_STAIR_TOP);
	sprites->Add(40018, 0, 0, 32, 32, texMisc6);

	LPDIRECT3DTEXTURE9 texMisc7 = textures->Get(ID_TEX_UI);
	sprites->Add(40019, 0, 33, 16, 50, texMisc7);

	LPDIRECT3DTEXTURE9 texMisc8 = textures->Get(ID_TEX_EFFECT1);
	sprites->Add(40020, 0, 0, 16, 20, texMisc8);

	LPDIRECT3DTEXTURE9 texMisc9 = textures->Get(ID_TEX_EFFECT2);
	sprites->Add(40021, 0, 10, 42, 37, texMisc9);
	sprites->Add(41021, 42, 10, 84, 37, texMisc9);
	sprites->Add(42021, 84, 10, 126, 37, texMisc9);

	LPDIRECT3DTEXTURE9 texMisc10 = textures->Get(ID_TEX_WHIP_VIP);
	sprites->Add(40022, 0, 0, 32, 32, texMisc10);

	LPDIRECT3DTEXTURE9 texMisc11 = textures->Get(ID_TEX_KNIFE_ACTION);
	sprites->Add(40023, 0, 0, 32, 18, texMisc11);

	LPDIRECT3DTEXTURE9 texMisc12 = textures->Get(ID_TEX_MONEY);
	sprites->Add(40024, 0, 0, 30, 30, texMisc12);
	#pragma endregion

	LPANIMATION ani;

	#pragma region SimonAnimation

	ani = new CAnimation(100);	//đứng phải
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	//đứng trái
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(180);	//đi phải
	ani->Add(10002);
	ani->Add(10003);
	ani->Add(11002);
	ani->Add(11003);
	animations->Add(500, ani);

	ani = new CAnimation(180);	//đi trái
	ani->Add(10012);
	ani->Add(10013);
	ani->Add(11012);
	ani->Add(11013);
	animations->Add(501, ani);

	ani = new CAnimation(150); //đánh phải
	ani->Add(10004);
	ani->Add(10005);
	ani->Add(10006);

	animations->Add(402, ani);

	ani = new CAnimation(150);//đánh trái
	ani->Add(10014);
	ani->Add(10015);
	ani->Add(10016);

	animations->Add(403, ani);

	ani = new CAnimation(100); //ngồi phải
	ani->Add(10018);
	animations->Add(404, ani);

	ani = new CAnimation(100);//ngồi trái
	ani->Add(10019);
	animations->Add(405, ani);

	ani = new CAnimation(100);//nhảy phải
	ani->Add(10020);
	animations->Add(406, ani);

	ani = new CAnimation(100);//nhảy trái
	ani->Add(10021);
	animations->Add(407, ani);

	ani = new CAnimation(150);//roi phải 
	ani->Add(10022);
	ani->Add(10023);
	ani->Add(10024);
	animations->Add(408, ani);

	ani = new CAnimation(150);//roi trái
	ani->Add(10025);
	ani->Add(10026);
	ani->Add(10027);
	animations->Add(409, ani);

	ani = new CAnimation(150);//ngồi đánh phải
	ani->Add(10028);
	ani->Add(10029);
	ani->Add(10030);
	animations->Add(410, ani);

	ani = new CAnimation(150);//ngồi đánh trái
	ani->Add(10031);
	ani->Add(10032);
	ani->Add(10033);
	animations->Add(411, ani);

	ani = new CAnimation(150);//trên thang phải đánh phải
	ani->Add(10034);
	ani->Add(10035);
	ani->Add(10036);
	animations->Add(412, ani);

	ani = new CAnimation(150);//trên thang phải đánh trái
	ani->Add(10037);
	ani->Add(10038);
	ani->Add(10039);
	animations->Add(413, ani);

	ani = new CAnimation(150);//trên thang trái đánh phải
	ani->Add(10040);
	ani->Add(10041);
	ani->Add(10042);
	animations->Add(414, ani);

	ani = new CAnimation(150);//trên thang trái đánh trái
	ani->Add(10043);
	ani->Add(10044);
	ani->Add(10045);
	animations->Add(415, ani);

	ani = new CAnimation(100);//lên thang phải
	ani->Add(10046);
	ani->Add(10047);
	animations->Add(416, ani);

	ani = new CAnimation(100);//xuống thang phải
	ani->Add(10048);
	ani->Add(10049);
	animations->Add(417, ani);

	ani = new CAnimation(100);//lên thang trái
	ani->Add(10050);
	ani->Add(10051);
	animations->Add(418, ani);

	ani = new CAnimation(100);//xuống thang trái
	ani->Add(10052);
	ani->Add(10053);
	animations->Add(419, ani);

	ani = new CAnimation(100);//đứng im bên phải thang phải
	ani->Add(10047);
	animations->Add(420, ani);

	ani = new CAnimation(100);//đứng im bên phải thang trái
	ani->Add(10049);
	animations->Add(421, ani);

	ani = new CAnimation(100);//đứng im bên trái thang phải
	ani->Add(10051);
	animations->Add(422, ani);

	ani = new CAnimation(100);//đứng im bên trái thang phải
	ani->Add(10053);
	animations->Add(423, ani);

	ani = new CAnimation(100);//bị đánh từ bên trái
	ani->Add(10054);	
	animations->Add(424, ani);

	ani = new CAnimation(100);//bị đánh từ bên phải
	ani->Add(10055);	
	animations->Add(425, ani);

	ani = new CAnimation(150);//roi lv0 phải
	ani->Add(10056);
	ani->Add(10057);
	ani->Add(10058);
	animations->Add(426, ani);

	ani = new CAnimation(150);//roi lv0 trái
	ani->Add(10059);
	ani->Add(10060);
	ani->Add(10061);
	animations->Add(427, ani);

	ani = new CAnimation(150);//roi lv1 phải
	ani->Add(10062);
	ani->Add(10063);
	ani->Add(10064);
	animations->Add(428, ani);

	ani = new CAnimation(150);//roi lv1 trái
	ani->Add(10065);
	ani->Add(10066);
	ani->Add(10067);
	animations->Add(429, ani);

	ani = new CAnimation(150);//axe
	ani->Add(10068);
	ani->Add(10069);
	ani->Add(10070);
	ani->Add(10071);
	animations->Add(430, ani);

	ani = new CAnimation(150);//holywater
	ani->Add(10072);
	ani->Add(10073);
	ani->Add(10074);
	animations->Add(431, ani);

	ani = new CAnimation(150);//cross
	ani->Add(10075);
	ani->Add(10076);
	ani->Add(10077);
	animations->Add(432, ani);

	ani = new CAnimation(150);//KNIFE
	ani->Add(10078);
	ani->Add(10079);
	animations->Add(433, ani);

	ani = new CAnimation(100);	//chết	
	ani->Add(10099);
	animations->Add(599, ani);

#pragma endregion

#pragma region ObjectAnimation

	ani = new CAnimation(100);	//đất1
	ani->Add(20001);
	animations->Add(601, ani);

	ani = new CAnimation(100); //zombie đi trái
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(602, ani);

	ani = new CAnimation(100); //zombie đi phải
	ani->Add(30003);
	ani->Add(30004);
	animations->Add(604, ani);

	ani = new CAnimation(100);	//đất2
	ani->Add(20002);
	animations->Add(603, ani);

	ani = new CAnimation(100); //panther nằm chờ
	ani->Add(30011);
	animations->Add(605, ani);

	ani = new CAnimation(100); //panther chạy trái
	ani->Add(30012);
	ani->Add(30013);
	ani->Add(30014);
	animations->Add(606, ani);

	ani = new CAnimation(100); //panther chạy phải
	ani->Add(30015);
	ani->Add(30016);
	ani->Add(30017);
	animations->Add(607, ani);

	ani = new CAnimation(100); //panther phóng
	ani->Add(30014);
	animations->Add(608, ani);

	ani = new CAnimation(100); //boss dơi nằm chờ
	ani->Add(30022);
	animations->Add(609, ani);

	ani = new CAnimation(100); //boosss dơi bay
	ani->Add(30023);
	ani->Add(30024);
	animations->Add(610, ani);

	ani = new CAnimation(100); //fire
	ani->Add(40011);
	ani->Add(40012);
	animations->Add(700, ani);

	ani = new CAnimation(100); //candle
	ani->Add(40013);
	ani->Add(40014);
	animations->Add(800, ani);

	ani = new CAnimation(0); //STAIR RIHGT
	ani->Add(40015);
	animations->Add(801, ani);

	ani = new CAnimation(0); //STAIR LEFT
	ani->Add(40016);
	animations->Add(802, ani);

	ani = new CAnimation(0); //STAIR BOT
	ani->Add(40017);
	animations->Add(803, ani);

	ani = new CAnimation(0); //STAIR TOP
	ani->Add(40018);
	animations->Add(804, ani);

	ani = new CAnimation(0); //heart
	ani->Add(40019);
	animations->Add(805, ani);

	ani = new CAnimation(150); //hieu ung' dau' sao
	ani->Add(40020);
	animations->Add(806, ani);

	ani = new CAnimation(50); //hieu ung toe' lua
	ani->Add(40021);
	ani->Add(41021);
	ani->Add(42021);
	animations->Add(807, ani);

	ani = new CAnimation(0); //whip item nang cap
	ani->Add(40022);
	animations->Add(808, ani);

	ani = new CAnimation(0); //knife
	ani->Add(40023);
	animations->Add(809, ani);

	ani = new CAnimation(0); //money
	ani->Add(40024);
	animations->Add(810, ani);

	#pragma endregion

	#pragma region simon

	simon = new Simon();
	simon->AddAnimation(400);	// đứng phải	
	simon->AddAnimation(401);	//đứng trái	
	simon->AddAnimation(500);	// đi phải
	simon->AddAnimation(501);	//đi trái
	simon->AddAnimation(402);	//đánh phải 
	simon->AddAnimation(403);	//đánh trái
	simon->AddAnimation(404);	//ngồi phải
	simon->AddAnimation(405);	//ngồi trái
	simon->AddAnimation(406);	//nhảy phải 
	simon->AddAnimation(407);	//nhảy trái
	simon->AddAnimation(410);	//ngồi đánh phải 
	simon->AddAnimation(411);	//ngồi đánh trái
	simon->AddAnimation(412);	//trên thang phải đánh phải
	simon->AddAnimation(413);	//trên thang phải đánh trái
	simon->AddAnimation(414);	//trên thang trái đánh phải
	simon->AddAnimation(415);	//trên thang trái đánh trái
	simon->AddAnimation(416);	//lên thang phải
	simon->AddAnimation(417);	//xuống thang phải
	simon->AddAnimation(418);	//lên thang trái
	simon->AddAnimation(419);	//xuống thang trái
	simon->AddAnimation(420);	//đứng im bên phải thang phải
	simon->AddAnimation(421);	//đứng im bên phải thang trái
	simon->AddAnimation(422);	//đứng im bên trái thang phải
	simon->AddAnimation(423);	//đứng im bên trái thang trái
	simon->AddAnimation(424);	//bị đánh từ bên trái
	simon->AddAnimation(425);	//bị đánh từ bên phải
	simon->AddAnimation(599);	//chết

	simon->whip->AddAnimation(426);//roi lv0 phải
	simon->whip->AddAnimation(427);//roi lv0 trái
	simon->whip->AddAnimation(428);//roi lv1 phải
	simon->whip->AddAnimation(429);//roi lv1 trái
	simon->whip->AddAnimation(408);//roi lv2 phải
	simon->whip->AddAnimation(409);//roi lv2 trái

#pragma endregion

	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	
	ui = new UI();
	ui->Initialize(d3ddv, simon);

	// Load map lv1 ra trước
	LoadResourceLv1();
}

void Update(DWORD dt)
{

#pragma region Resource

	/**
	 * Kiểm tra để load resource tương ứng
	 */
	float x, y;
	simon->GetPosition(x, y);
	if (!simon->isLevelUp)
		gameTime -= dt;

	if (lv1 == true)
	{
		Item *bonus = NULL;
		// Chưa chỉnh lại theo grid
		// Thưởng bonus vòng 1
		/*if (simon->isBonus)
		{
			bonus = new Item();
			bonus->SetPosition(1367, 407);
			bonus->AddAnimation(ITEM_MONEY);
			bonus->SetType(ITEM_MONEY);
			simon->isBonus = false;
		}

		if (bonus != NULL)
		{
			if (bonus->y > 370)
			{
				bonus->y -= SIMON_CLIMBING_SPEED_Y * dt;
				bonus->Render();
			}
			else
			{
				objects.push_back(bonus);
			}
		}*/

		// Lên cấp
		if (simon->isLevelUp) {
			isEnableKeyBoard = false;
			simon->SetState(SIMON_STATE_WALK);
			if (!simon->isJump)
				simon->vx = SIMON_LEVELUP_SPEED;
			DWORD timer = GetTickCount();
			if (timer - simon->onCheckPointTime > LEVELUP_TIME)
			{
				listGrids->ReleaseList();

				lv2 = true;
				lv1 = false;
				simon->isLevelUp = false;
				simon->SetState(SIMON_STATE_IDLE);
				isEnableKeyBoard = true;
			}
		}
	}
	
	if (lv2 == true)
	{
		// Lần đầu load resource lv2
		if (countLoadResourceLv2 == false)
		{
			game->x_cam = 0;
			listGrids->InitList(MAX_WIDTH_LV2);
			LoadResourceLv2();
			countLoadResourceLv2 = true;
			simon->SetPosition(3000, 155);
			simon->GetPosition(x, y);
			timer = GetTickCount();
		}
		else if (countLoadResourceLv2 == true && x < MAX_WIDTH_LV2 - 2 * SIMON_STAND_BBOX_WIDTH)
		{
			// Zombie xuất hiện chưa đúng, cần chỉnh lại (1 lượt 3 con)
			if (GetTickCount() - timer > 5000)
			{
				Zombie *zombie = new Zombie();
				zombie->AddAnimation(602);
				zombie->AddAnimation(604);
				zombie->SetPosition(1200, 376);
				zombie->SetState(ZOMBIE_STATE_WALKING);
				listGrids->AddObject(zombie);
				timer = timer + 5000;
			}
		}
		else  //check point
		{
			listGrids->ReleaseList();

			lv2_1 = true;
			lv2 = false;
		}
	}
	if (lv2_1 == true)
	{
		// Lần đầu load resource lv2_1
		if (countLoadResourceLv2_1 == false)
		{
			listGrids->InitList(MAX_WIDTH_LV2_1);
			LoadResourceLv2_1();
			countLoadResourceLv2_1 = true;
		}
		
		if (x > MAX_WIDTH_LV2_1 - 2 * SIMON_STAND_BBOX_WIDTH)
		{
			listGrids->ReleaseList();

			lv2_1 = false;			
			boss = true;
		}
		if (y > 430)
		{
			lv2_1 = false;
			lv2_2 = true;
			temp = true;
		}
	}
	if (lv2_2 == true)
	{
		if (countLoadResourceLv2_2 == false)
		{
			//game->x_cam = 0;
			listGrids->InitList(MAX_WIDTH_LV2_2);
			LoadResourceLv2_2();
			countLoadResourceLv2_2 = true;			
			simon->SetPosition(150, 120);
			simon->GetPosition(x, y);
				
		}
		if (countLoadResourceLv2_2 == true && temp == true)
		{
			
			if (x < 3600)
			{
				game->x_cam = 0;
				simon->SetPosition(150, 120);
				simon->GetPosition(x, y);

			}
			else 
			{
				game->x_cam = MAX_WIDTH_LV2_2 - SCREEN_WIDTH;
				simon->SetPosition(778, 120);
				simon->GetPosition(x, y);
			}
			/*game->x_cam = 0;
			simon->SetPosition(150, 120);
			simon->GetPosition(x, y);*/
			temp = false;
		}
		if (y < 120)
		{
			
			lv2_1 = true;
			lv2_2 = false;
			if (x < 400)
			{
				simon->SetPosition(3198, 429);
				simon->GetPosition(x, y);
				game->x_cam = MAX_WIDTH_LV2;
			}
			else
			{
				simon->SetPosition(3835, 429);
				simon->GetPosition(x, y);
				game->x_cam = MAX_WIDTH_LV2_1 - SCREEN_WIDTH;
			}
		}
		
	}
	if (boss == true)
	{
		if (countLoadResourceboss == false)
		{
			listGrids->InitList(MAX_WIDTH_BOSS);
			LoadResourceboss();
			countLoadResourceboss = true;
			timer2 = GetTickCount();
		}
		else if (countLoadResourceboss == true)
		{
			if (GetTickCount() - timer2 > 5000)
			{
				Zombie *zombie = new Zombie();
				zombie->AddAnimation(602);
				zombie->AddAnimation(604);
				zombie->SetPosition(4900, 376);
				zombie->SetState(ZOMBIE_STATE_WALKING);
				listGrids->AddObject(zombie);
				timer2 = timer2 + 5000;
			}
		}
	}
#pragma endregion

#pragma region Update Object

	vector<LPGAMEOBJECT> objects;

	if (simon->x < 0)
		currentGrids = listGrids->GetCurrentGrids(0);
	else
		currentGrids = listGrids->GetCurrentGrids(simon->x);

	for (int i = 0; i < currentGrids.size(); i++)
	{
		vector<LPGAMEOBJECT> listObjects = currentGrids[i]->GetListObject();
		int listObjectSize = listObjects.size();
		for (int j = 0; j < listObjectSize; j++)
		{
			objects.push_back(listObjects[j]);
		}
	}

	simon->Update(dt, &objects);

	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<BossBat *>(objects.at(i)))
		{
			BossBat *bossbat = dynamic_cast<BossBat *>(objects.at(i));
			bossbat->Update(x, y, dt, &objects);
		}
		else if (dynamic_cast<Panther *>(objects.at(i)))
		{
			Panther *panther = dynamic_cast<Panther *>(objects.at(i));
			if (panther->isActivate && ((panther->x < (simon->x - 2 * SCREEN_WIDTH / 3)) || (panther->x > (simon->x + 2 * SCREEN_WIDTH / 3))))
			{
				panther->isDie = true;
			}
			else if (!panther->isUse)
			{
				if ((simon->y - panther->y < PANTHER_DISTANCE_SIMON_Y && panther->x - simon->x < PANTHER_DISTANCE_SIMON_X))
				{
					panther->SetState(PANTHER_STATE_RUN);
					panther->isActivate = true;
				}

				if (panther->isActivate &&
				   (panther->x >= PANTHER_JUMP_POINT_1_X - 3 && panther->x <= PANTHER_JUMP_POINT_1_X + 10) ||
				   (panther->x >= PANTHER_JUMP_POINT_2_X - 3 && panther->x <= PANTHER_JUMP_POINT_2_X + 10) ||
				   (panther->x >= PANTHER_JUMP_POINT_3_X - 3 && panther->x <= PANTHER_JUMP_POINT_3_X + 10))
				{
					panther->SetState(PANTHER_STATE_JUMP);
				}

			}

			panther->Update(dt, &objects);
		}
		else
			objects[i]->Update(dt, &objects);

		listGrids->UpdateObjectInGrid(objects[i]);
	}
	
#pragma endregion

#pragma region Remove Object
	/**
	 * Không remove trực tiếp trong mảng khi đang duyệt, thêm vào danh sách các object sẽ bị remove và sau đó mới bắt đầu remove
	 */
	vector<LPGAMEOBJECT> listRemoveObjects;

	// Thêm các object sẽ bị remove vào listRemoveObjects và cập nhật thêm các object mới vào objects và listGrids
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Zombie *>(objects.at(i)))
		{
			Zombie *zombie = dynamic_cast<Zombie *>(objects.at(i));

			if (zombie->GetState() == ZOMBIE_STATE_DIE)
			{
				listRemoveObjects.push_back(zombie);
			}
		}
		else if (dynamic_cast<Panther *>(objects.at(i)))
		{
			Panther *panther = dynamic_cast<Panther *>(objects.at(i));

			if (panther->isDie)
			{
				listRemoveObjects.push_back(panther);
			}
		}
		else if (dynamic_cast<BigFire *>(objects.at(i)))
		{
			BigFire *bigFire = dynamic_cast<BigFire *>(objects.at(i));
			if (bigFire->isHitted)
			{
				float bigfire_x, bigfire_y, bigfire_right, bigfire_bottom;
				bigFire->GetBoundingBox(bigfire_x, bigfire_y, bigfire_right, bigfire_bottom);
				
				item = new Item();
				item->SetPosition(bigfire_x, bigfire_y);
				item->SetSpeed(0, -0.1);
				objects.push_back(item);
				listGrids->AddObject(item);

				// Whip item
				if (simon->whip->level < 2)
				{
					item->AddAnimation(ITEM_WHIPITEM);
					item->SetType(ITEM_WHIPITEM);
				}
				else
				{
					/**
					 * Random ra item: (do hiện tại chỉ có 3 món này)
					 * 95% heart
					 * 4% money
					 * 1% knife
					 */

					srand(time(NULL));
					int random_portion = rand() % 100;

					// Heart
					if (random_portion < 95)
					{
						item->AddAnimation(ITEM_HEART);
						item->SetType(ITEM_HEART);
					}
					// Money
					else if (random_portion >= 95 && random_portion < 99)
					{
						item->AddAnimation(ITEM_MONEY);
						item->SetType(ITEM_MONEY);
					}
					// Knife
					else
					{
						item->AddAnimation(ITEM_KNIFE);
						item->SetType(ITEM_KNIFE);
					}
				}

				// Thêm hiệu ứng tóe lửa
				whipEffect = new Effect(GetTickCount());
				whipEffect->AddAnimation(806);
				whipEffect->SetPosition(bigfire_x, bigfire_y + (bigfire_bottom - bigfire_y) / 4);
				objects.push_back(whipEffect);
				listGrids->AddObject(whipEffect);

				whipEffect = new Effect(GetTickCount());
				whipEffect->AddAnimation(807);
				whipEffect->SetPosition(bigfire_x, bigfire_y + (bigfire_bottom - bigfire_y) / 4);
				objects.push_back(whipEffect);
				listGrids->AddObject(whipEffect);

				listRemoveObjects.push_back(bigFire);
			}
		}
		else if (dynamic_cast<Candle *>(objects.at(i)))
		{
			Candle *candle = dynamic_cast<Candle *>(objects.at(i));
			if (candle->isHitted)
			{
				float candle_x, candle_y, candle_right, candle_bottom;
				candle->GetBoundingBox(candle_x, candle_y, candle_right, candle_bottom);

				item = new Item();
				item->SetPosition(candle_x, candle_y);
				item->SetSpeed(0, -0.1);
				objects.push_back(item);
				listGrids->AddObject(item);

				// Whip item
				if (simon->whip->level < 2)
				{
					item->AddAnimation(ITEM_WHIPITEM);
					item->SetType(ITEM_WHIPITEM);
				}
				else
				{
					/**
					 * Random ra item: (do hiện tại chỉ có 3 món này)
					 * 95% heart
					 * 4% money
					 * 1% knife
					 */

					srand(time(NULL));
					int random_portion = rand() % 100;

					// Heart
					if (random_portion < 95)
					{
						item->AddAnimation(ITEM_HEART);
						item->SetType(ITEM_HEART);
					}
					// Money
					else if (random_portion >= 95 && random_portion < 99)
					{
						item->AddAnimation(ITEM_MONEY);
						item->SetType(ITEM_MONEY);
					}
					// Knife
					else
					{
						item->AddAnimation(ITEM_KNIFE);
						item->SetType(ITEM_KNIFE);
					}
				}

				// Thêm hiệu ứng tóe lửa
				whipEffect = new Effect(GetTickCount());
				whipEffect->AddAnimation(806);
				whipEffect->SetPosition(candle_x, candle_y + (candle_bottom - candle_y) / 4);
				objects.push_back(whipEffect);
				listGrids->AddObject(whipEffect);

				whipEffect = new Effect(GetTickCount());
				whipEffect->AddAnimation(807);
				whipEffect->SetPosition(candle_x, candle_y + (candle_bottom - candle_y) / 4);
				objects.push_back(whipEffect);
				listGrids->AddObject(whipEffect);

				listRemoveObjects.push_back(candle);
			}
		}
		else if (dynamic_cast<Item *>(objects.at(i)))
		{
			Item *item = dynamic_cast<Item *>(objects.at(i));

			if (item->GetEaten())
			{
				listRemoveObjects.push_back(item);
			}
		}
		else if (dynamic_cast<Effect *>(objects.at(i)))
		{
			Effect *effect = dynamic_cast<Effect *>(objects.at(i));

			if (effect->GetExposed())
			{
				listRemoveObjects.push_back(effect);
			}
		}
	}

	// Remove lần lượt từng object từ listRemoveObjects trong listGrids
	for (int i = 0; i < listRemoveObjects.size(); i++)
	{
		listGrids->RemoveObject(listRemoveObjects[i]);
		delete listRemoveObjects[i];
	}
#pragma endregion

#pragma region Camera
	if (lv1 == true)
	{
		if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;
			
		}
		else if (x > MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
		{
			game->x_cam = MAX_WIDTH_LV1 - SCREEN_WIDTH;
			
		}
		else
		{
			game->x_cam = 0;
			
		}
	}
	else if (lv2 == true)
	{
		if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;
			
		}
		else if (x > MAX_WIDTH_LV2 - SCREEN_WIDTH / 2) {
			game->x_cam = MAX_WIDTH_LV2 - SCREEN_WIDTH;
			
		}
		else if (x < SCREEN_WIDTH / 2)
		{
			game->x_cam = 0;
			
		}
	}
	else if (lv2_1 == true)
	{
		// chuyen scene
		if (game->x_cam < MAX_WIDTH_LV2 - SCREEN_WIDTH/2)
		{
			game->x_cam += SIMON_WALKING_SPEED * dt;
			
		}
		else 
		{
			if (x < 3200)
			{
				if(check1 == false)
				simon->SetState(SIMON_STATE_WALK);	
			}
			if (x > 3200 && x < 3210 ) 
			{
				check1 = true;
				check = true;
				simon->SetState(SIMON_STATE_IDLE);
			}
		}
		if (game->x_cam < MAX_WIDTH_LV2 && check == true)
		{
			game->x_cam += SIMON_WALKING_SPEED * dt;
			
		}
		else if (game->x_cam > MAX_WIDTH_LV2)
		{
			checkScene = true;
		}

		//
		//trả camera về simon
		if (checkScene == true)
		{
			if (x > MAX_WIDTH_LV2 + SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2)
			{
				game->x_cam = x - SCREEN_WIDTH / 2;
				
			}
			else if (x > MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2) 
			{
				game->x_cam = MAX_WIDTH_LV2_1 - SCREEN_WIDTH;
				
			}
			else if (x < MAX_WIDTH_LV2 + SCREEN_WIDTH / 2)
			{
				game->x_cam = MAX_WIDTH_LV2;
				
			}
		}
	}
	else if (lv2_2 == true)
	{
		if (x < SCREEN_WIDTH / 2)
		{
			game->x_cam = 0;
		}
		else if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2_2 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;

		}
		else if (x > MAX_WIDTH_LV2_2 - SCREEN_WIDTH / 2)
			game->x_cam = MAX_WIDTH_LV2_2 - SCREEN_WIDTH;
	}
	else if (boss == true)
	{
		if (game->x_cam < MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2)
		{
			game->x_cam += SIMON_WALKING_SPEED * dt;
			
		}
		else 
		{
			if (x < MAX_WIDTH_LV2_1 + 100)
			{
				if (check3 == false)
				simon->SetState(SIMON_STATE_WALK);
			}
			else if (x > MAX_WIDTH_LV2_1 + 100 && x < MAX_WIDTH_LV2_1 + 105)
			{
				simon->SetState(SIMON_STATE_IDLE);
				checkScene1 = true;
				check3 = true;
			}
		}
		
		if (checkScene1 == true)
		{
			if (game->x_cam < MAX_WIDTH_LV2_1)
				game->x_cam += SIMON_WALKING_SPEED * dt;
			else 
			{
				if (x > MAX_WIDTH_LV2_1 + SCREEN_WIDTH / 2 && x < MAX_WIDTH_BOSS - SCREEN_WIDTH / 2)
				{
					game->x_cam = x - SCREEN_WIDTH / 2;					
				}
				else if (x > MAX_WIDTH_BOSS - SCREEN_WIDTH / 2) {
					game->x_cam = MAX_WIDTH_BOSS - SCREEN_WIDTH;				
				}
				else if (x < MAX_WIDTH_LV2_1 + SCREEN_WIDTH / 2)
				{
					game->x_cam = MAX_WIDTH_LV2_1;					
				}
			}
		}
	}
#pragma endregion

#pragma region UI

	if (lv1 == true)
		ui->Update(gameTime/1000, 1,simon);
	else
		ui->Update(gameTime/1000, 2,simon);

#pragma endregion
}

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		CTextures * textures = CTextures::GetInstance();
		LPDIRECT3DTEXTURE9 tex = textures->Get(ID_TEX_LV1);
		LPDIRECT3DTEXTURE9 tex2 = textures->Get(ID_TEX_LV1_2);		
		float x, y;
		simon->GetPosition(x, y);

		LPDIRECT3DTEXTURE9 tileset = textures->Get(ID_TEX_TILESET);
		LPDIRECT3DTEXTURE9 tileset1 = textures->Get(ID_TEX_TILESET_2);
		LPDIRECT3DTEXTURE9 tileset2 = textures->Get(ID_TEX_TILESET_3);
		
		if (lv1 == true)
		{
			map = new	Map (/*48, 10,*/ tileset, 32, 32); 
			map->LoadMatrixMap("Castlevania\\Mapstate.txt");
			//map->Draw(game->x_cam, game->y_cam);
		}
		else if( lv2 == true || lv2_1 == true) {									
			map = new	Map (/*176, 11,*/ tileset1, 32, 32); 
			map->LoadMatrixMap("Castlevania\\Mapstate2.txt");
			//map->Draw(game->x_cam , game->y_cam);
		}
		else if (lv2_2 == true)
		{
			map = new	Map(/*32, 11,*/ tileset2, 32, 32);
			map->LoadMatrixMap("Castlevania\\Mapstate2_1.txt");
		}
		map->Draw(game->x_cam, game->y_cam);

		for (int i = 0; i < currentGrids.size(); i++)
		{
			vector<LPGAMEOBJECT> listObject = currentGrids[i]->GetListObject();
			int listObjectSize = listObject.size();

			for (int j = 0; j < listObjectSize; j++)
			{
				listObject[j]->Render();
			}
		}

		simon->Render();
		ui->Render(game->x_cam, game->y_cam,simon);
		/*RECT newRect;
		SetRect(&newRect, 0, 0, 30, 30);
		D3DXVECTOR3 p(0, 0, 0);
		spriteHandler->Draw(textures->Get(ID_TEX_AXE), &newRect, NULL, &p, D3DCOLOR_ARGB(255, 255, 255, 255));*/
		
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);
	
	listGrids = ListGrids::GetInstance();

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}