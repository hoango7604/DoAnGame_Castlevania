

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Simon.h"
#include "Ground.h"
#include "define.h"
#include "Zombie.h"
#include "BigFire.h"
#include "Candle.h"
#include "Map.h"
#include "Stair.h"
#include "RedBat.h"
#include "Panther.h"
#include "MerMan.h"
#include "CheckStair.h"
CGame *game;

Simon * simon;

Map *map;
CSprite *sprite;
vector<LPGAMEOBJECT> objects;
vector<int> willDeleteObjects;
bool lv2 = false;
bool countLoadResourceLv2 = false;
CSprites * sprites = CSprites::GetInstance();
CAnimations * animations = CAnimations::GetInstance();
CTextures * textures = CTextures::GetInstance();

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
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
			simon->SetAction(SIMON_ACTION_ATTACK);
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	// Ngoi
	if (KeyCode == DIK_DOWN)
	{
		simon->isSit = false;
		simon->y -= SIMON_SIT_TO_STAND;
	}
	// Di bo
	else if (KeyCode == DIK_RIGHT || KeyCode == DIK_LEFT)
	{
		simon->isMoving = false;
		simon->vx = 0;
	}
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	// Them xu ly len xuong cau thang nua, chua co

	// Ngoi
	if (game->IsKeyDown(DIK_DOWN))
	{
		if (!simon->isOnStair && !simon->isAttack && !simon->isJump)
			simon->SetState(SIMON_STATE_SIT);
	}

	// Di bo
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (!simon->isSit && !simon->isOnStair && !simon->isAttack)
			simon->SetState(SIMON_STATE_WALK);
		simon->nx = 1.0f;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (!simon->isSit && !simon->isOnStair && !simon->isAttack)
			simon->SetState(SIMON_STATE_WALK);
		simon->nx = -1.0f;
	}

	// Neu khong co gi xay ra se dung im
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
	textures->Add(ID_TEX_FIRE, L"Castlevania\\123.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"Castlevania\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP, L"Castlevania\\WHIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP_2, L"Castlevania\\WHIP_left.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET, L"Castlevania\\tileset.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"Castlevania\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LADDER, L"Castlevania\\3.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LADDER_LEFT, L"Castlevania\\3_.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BBOX, L"Castlevania\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_STAIR_BOT, L"Castlevania\\stair_bottom.png", D3DCOLOR_XRGB(0, 0, 255));//
	textures->Add(ID_TEX_STAIR_TOP, L"Castlevania\\stair_top.png", D3DCOLOR_XRGB(0, 0, 255));
	textures->Add(ID_TEX_MERMAN_LEFT, L"Castlevania\\MERMAN.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_MERMAN_RIGHT, L"Castlevania\\MERMAN_right.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_BAT, L"Castlevania\\BAT.png", D3DCOLOR_XRGB(255, 255, 255));

	LPDIRECT3DTEXTURE9 tileset1 = textures->Get(ID_TEX_TILESET);
	sprite = new CSprite(500000, 0, 0, 256, 64, tileset1);
	map = new	Map(48, 10, sprite, 32, 32); // 48 12 16 16
	map->LoadMatrixMap("Castlevania\\Mapstate.txt");


	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
#pragma region Addsprite
	sprites->Add(10001, 434, 0, 469, 64, texSimon);		// đứng im phải

	sprites->Add(10002, 375, 0, 401, 64, texSimon);		// đi phải
	sprites->Add(10003, 314, 0, 344, 64, texSimon);

	sprites->Add(10004, 120, 0, 171, 64, texSimon);		// đánh phải			
	sprites->Add(10005, 74, 0, 111, 64, texSimon);
	sprites->Add(10006, 14, 0, 59, 64, texSimon);

	sprites->Add(10028, 0, 83, 49, 131, texSimon);		// ngồi đánh phải			
	sprites->Add(10029, 435, 150, 469, 197, texSimon);
	sprites->Add(10030, 376, 151, 421, 197, texSimon);

	sprites->Add(10034, 120, 135, 168, 197, texSimon);		// trên thang phải đánh phải			
	sprites->Add(10035, 76, 134, 109, 197, texSimon);
	sprites->Add(10036, 17, 136, 59, 197, texSimon);

	sprites->Add(10040, 302, 134, 351, 197, texSimon);		// trên thang trái đánh phải			
	sprites->Add(10041, 257, 133, 290, 197, texSimon);
	sprites->Add(10042, 197, 134, 240, 197, texSimon);

	sprites->Add(10046, 141, 67, 164, 130, texSimon); // lên thang phải
	sprites->Add(10047, 199, 67, 229, 130, texSimon);

	sprites->Add(10052, 141, 67, 164, 130, texSimon); //xuống thang trái
	sprites->Add(10053, 318, 67, 351, 130, texSimon);


	sprites->Add(10018, 196, 17, 230, 64, texSimon); // ngồi phải

	sprites->Add(10020, 196, 17, 230, 64, texSimon); // nhảy phải
	
	sprites->Add(10054, 436, 67, 471, 130, texSimon);//bị đánh từ bên trái

	LPDIRECT3DTEXTURE9 texSimon2 = textures->Get(ID_TEX_SIMON_2);
	sprites->Add(10011, 11, 0, 47, 64, texSimon2);		// đứng im trái

	sprites->Add(10012, 74, 0, 107, 64, texSimon2);		// đi trái
	sprites->Add(10013, 132, 0, 171, 64, texSimon2);

	sprites->Add(10014, 313, 0, 358, 64, texSimon2);		// đánh trái				
	sprites->Add(10015, 372, 0, 404, 64, texSimon2);
	sprites->Add(10016, 421, 0, 464, 64, texSimon2);

	sprites->Add(10031, 432, 83, 477, 130, texSimon2);		// ngồi đánh trái				
	sprites->Add(10032, 13, 149, 44, 197, texSimon2);
	sprites->Add(10033, 60, 151, 105, 197, texSimon2);

	sprites->Add(10037, 131, 134, 177, 197, texSimon2);		// trên thang phải đánh trái			
	sprites->Add(10038, 193, 134, 225, 197, texSimon2);
	sprites->Add(10039, 240, 151, 284, 197, texSimon2);

	sprites->Add(10043, 312, 134, 361, 197, texSimon2);		// trên thang trái đánh trái			
	sprites->Add(10044, 371, 134, 405, 197, texSimon2);
	sprites->Add(10045, 420, 151, 466, 197, texSimon2);


	sprites->Add(10048, 187, 67, 215, 130, texSimon2); // xuống thang phải
	sprites->Add(10049, 129, 67, 163, 130, texSimon2);

	sprites->Add(10050, 187, 67, 215, 130, texSimon2); //lên thang trái
	sprites->Add(10051, 250, 67, 283, 130, texSimon2);

	sprites->Add(10019, 256, 17, 286, 64, texSimon2); // ngồi trái

	sprites->Add(10021, 256, 17, 286, 64, texSimon2);//nhảy trái

	sprites->Add(10055, 12, 70, 41, 130, texSimon2);//bị đánh từ bên phải

	sprites->Add(10099, 180, 237, 240, 264, texSimon);		// chết 

	LPDIRECT3DTEXTURE9 whipR = textures->Get(ID_TEX_WHIP);
	sprites->Add(10022, 570, 200, 554, 267, whipR);//roi phải
	sprites->Add(10023, 342, 260, 312, 316, whipR);
	sprites->Add(10024, 342, 328, 458, 364, whipR);

	LPDIRECT3DTEXTURE9 whipL = textures->Get(ID_TEX_WHIP_2);
	sprites->Add(10025, 105, 70, 165, 133, whipL);//roi trái
	sprites->Add(10026, 340, 65, 406, 117, whipL);
	sprites->Add(10027, 575, 330, 458, 364, whipL);


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
	sprites->Add(30011, 0, 0, 50, 32, texEnemy1); // báo
	sprites->Add(30012, 64, 0, 120, 32, texEnemy1);
	sprites->Add(30013, 126, 0, 188, 32, texEnemy1);
	sprites->Add(30014, 188, 0, 256, 32, texEnemy1);

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

#pragma endregion

	LPANIMATION ani;

#pragma region SimonAnimation

	ani = new CAnimation(100);	//đứng phải
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	//đứng trái
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(100);	//đi phải
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);

	ani = new CAnimation(100);	//đi trái
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
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

	ani = new CAnimation(150);//xuống thang phải
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

	ani = new CAnimation(100);//bị đánh từ bên trái
	ani->Add(10054);	
	animations->Add(420, ani);

	ani = new CAnimation(100);//bị đánh từ bên phải
	ani->Add(10055);	
	animations->Add(421, ani);

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
	simon->AddAnimation(420);	//bị đánh từ bên trái
	simon->AddAnimation(421);	//bị đánh từ bên phải
	simon->AddAnimation(599);	//chết

	simon->whip->AddAnimation(408);//roi phải
	simon->whip->AddAnimation(409);//roi trái
	simon->SetPosition(1500, 327);
	objects.push_back(simon);

#pragma endregion

#pragma region Ground
	for (int i = 0; i < 50; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(0 + i * 32.0f, 407);
		objects.push_back(ground);
	}
#pragma endregion

#pragma region BigFire
	BigFire *bigfire = new BigFire();
	bigfire->AddAnimation(700);
	bigfire->SetPosition(335, 350);
	objects.push_back(bigfire);
	BigFire *bigfire1 = new BigFire();
	bigfire1->AddAnimation(700);
	bigfire1->SetPosition(464, 350);
	objects.push_back(bigfire1);
	BigFire *bigfire2 = new BigFire();
	bigfire2->AddAnimation(700);
	bigfire2->SetPosition(657, 350);
	objects.push_back(bigfire2);
	BigFire *bigfire3 = new BigFire();
	bigfire3->AddAnimation(700);
	bigfire3->SetPosition(851, 350);
	objects.push_back(bigfire3);
	BigFire *bigfire4 = new BigFire();
	bigfire4->AddAnimation(700);
	bigfire4->SetPosition(1090, 350);
	objects.push_back(bigfire4);
	BigFire *bigfire5 = new BigFire();
	bigfire5->AddAnimation(700);
	bigfire5->SetPosition(1267, 350);
	objects.push_back(bigfire5);
#pragma endregion
}

void LoadResourceLv2() {
	for (int i = 0; i < 96; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(0 + i * 32.0f, 414);
		objects.push_back(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1377 + i * 32.0f, 284);
		objects.push_back(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1503 + i * 32.0f, 218);
		objects.push_back(ground);
	}
	for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1854 + i * 32.0f, 284);
		objects.push_back(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(2782 + i * 32.0f, 220);
		objects.push_back(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Zombie *zombie = new Zombie();
		zombie->AddAnimation(602);
		zombie->AddAnimation(604);
		zombie->SetPosition(0 + i * 64, 350);
		zombie->SetState(ZOMBIE_STATE_WALKING);
		objects.push_back(zombie);
	}
	for (int i = 0; i < 5; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(65 + i * 255, 350);
		objects.push_back(candle);
	}
	for (int i = 0; i < 4; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(195 + i * 257, 290);
		objects.push_back(candle);
	}
	for (int i = 0; i < 4; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(801);
		stair->SetPosition(1247 + i * 32, 379 - i * 32);
		objects.push_back(stair);
	}
	for (int i = 0; i < 2; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(801);
		stair->SetPosition(1439 + i * 32.0, 252 - i * 32);
		objects.push_back(stair);
	}
	
	for (int i = 0; i < 2; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(802);
		stair->SetPosition(1824 + i * 32.0, 220 + i * 32);
		objects.push_back(stair);

	}
	for (int i = 0; i < 6; i++)
	{
		Stair *stair = new Stair();
		stair->AddAnimation(801);
		stair->SetPosition(2590 + i * 32.0, 379 - i * 32);
		objects.push_back(stair);
	}
	
	CheckStair *checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(1247, 379);
	objects.push_back(checkstair);

	CheckStair *checkstair1 = new CheckStair();
	checkstair1->AddAnimation(804);
	checkstair1->SetPosition(1343,283);
	objects.push_back(checkstair1);
	//1250 335 1265 320 1280 305*/ 3 10 6 8
}
void Update(DWORD dt)
{
	float x, y;
	simon->GetPosition(x, y);

#pragma region Resource
	if (x > 1536 && lv2 == false) {
		for (int i = objects.size(); i > 1; i--)
			objects.pop_back();

		lv2 = true;
	}
	if (lv2 == true)
	{
		if (countLoadResourceLv2 == false)
		{
			LoadResourceLv2();
			countLoadResourceLv2 = true;
			simon->SetPosition(30, 150);
		}

	}

#pragma endregion

#pragma region Collision
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
#pragma endregion	

#pragma region Remove Object
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Zombie *>(objects.at(i)))
		{
			Zombie *zombie = dynamic_cast<Zombie *>(objects.at(i));

			if (zombie->GetState() == ZOMBIE_STATE_DIE) {
				objects.erase(objects.begin() + i);
				delete zombie;
			}
		}
	}
#pragma endregion	

#pragma region Camera
	if (lv2 == false)
	{
		if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
		{

			game->x_cam = x - SCREEN_WIDTH / 2;
			game->y_cam = 0;
		}
		else if (x > MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
		{
			game->x_cam = MAX_WIDTH_LV1 - SCREEN_WIDTH;
			game->y_cam = 0;
		}
		else
		{
			game->x_cam = 0;
			game->y_cam = 0;
		}
	}
	else
	{
		if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;
			game->y_cam = 0;
		}
		else if (x > MAX_WIDTH_LV2 - SCREEN_WIDTH / 2) {
			game->x_cam = MAX_WIDTH_LV2 - SCREEN_WIDTH;
			game->y_cam = 0;
		}
		else
		{
			game->x_cam = 0;
			game->y_cam = 0;
		}

	}
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
		float z, t;
		simon->GetSpeed(z, t);
		float x, y;
		simon->GetPosition(x, y);

		if (lv2 == false)
			map->Draw(game->x_cam, game->y_cam);
		else {
			if (x < SCREEN_WIDTH / 2) {
				D3DXVECTOR3 p(0, 92, 0);
				RECT r;
				r.left = 0;
				r.top = 18;
				r.right = 640;
				r.bottom = 480;
				spriteHandler->Draw(tex2, &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
			}
			else if (x < MAX_WIDTH_LV2 - SCREEN_WIDTH / 2 && x > SCREEN_WIDTH / 2) {

				D3DXVECTOR3 p(0, 92, 0);
				RECT r;
				r.left = (x - SCREEN_WIDTH / 2);
				r.top = 18;
				r.right = z > 0 ? 642 + x : 638 + x;
				r.bottom = 480;
				spriteHandler->Draw(tex2, &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
			}
			else {
				D3DXVECTOR3 p(0, 92, 0);
				RECT r;
				r.left = MAX_WIDTH_LV2 - SCREEN_WIDTH;
				r.top = 18;
				r.right = (640 + x);
				r.bottom = 480;
				spriteHandler->Draw(tex2, &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
			}


		}
		for (int i = objects.size() - 1; i > -1; i--)
			objects[i]->Render();



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


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}