

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Simon.h"
#include "Ground.h"
#include"define.h"
#include "Zombie.h"
#include"BigFire.h"

#include"Map.h"


CGame *game;

Simon * simon;

Map *map;
CSprite *sprite;
vector<LPGAMEOBJECT> objects;
bool lv2 = false;
class CSampleKeyHander : public CKeyEventHandler
{
	
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
public:
	DWORD timeStartWhip = 0;
	DWORD timeStartJump = 0;
	bool NotOnWhip = true;
	bool NotOnJump = true;
	
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	if (NotOnWhip == true && NotOnJump == true)
	{
		switch (KeyCode)
		{
		case DIK_SPACE:
			timeStartJump = GetTickCount();
			NotOnJump = false;
			simon->SetState(SIMON_STATE_JUMP);
			break;
		case DIK_B: // reset
			simon->SetState(SIMON_STATE_IDLE);
			simon->SetLevel(SIMON_LEVEL_BIG);
			simon->SetPosition(50.0f, 150.0f);
			simon->SetSpeed(0, 0);
			break;
		case DIK_A:
		{

			timeStartWhip = GetTickCount();
			NotOnWhip = false;
			simon->SetState(SIMON_STATE_HIT);
			break;
		}

		}

	}
	
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	if (NotOnWhip == true )
	{
		if (simon->GetState() == SIMON_STATE_DIE) return;
		if (game->IsKeyDown(DIK_RIGHT))
			simon->SetState(SIMON_STATE_WALKING_RIGHT);
		else if (game->IsKeyDown(DIK_LEFT))
			simon->SetState(SIMON_STATE_WALKING_LEFT);

		/*else if (game->IsKeyDown(DIK_A))
			simon->SetState(SIMON_STATE_HIT);*/
		else if (game->IsKeyDown(DIK_DOWN))
			simon->SetState(SIMON_STATE_SIT);
		else
			simon->SetState(SIMON_STATE_IDLE);
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

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	CTextures * textures = CTextures::GetInstance();
	
	
	textures->Add(ID_TEX_SIMON, L"Castlevania\\Simon_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIMON_2, L"Castlevania\\Simon_ver_editted.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LV1, L"Castlevania\\lv1.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_LV1_2, L"Castlevania\\lv2.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_BRICK, L"Castlevania\\Brick.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_ZOMBIE, L"Castlevania\\ZOMBIE.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_PANTHER, L"Castlevania\\PANTHER.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_FIRE, L"Castlevania\\123.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"Castlevania\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP, L"Castlevania\\WHIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP_2, L"Castlevania\\WHIP_left.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET, L"Castlevania\\tileset.png", D3DCOLOR_XRGB(255, 0, 255));

	LPDIRECT3DTEXTURE9 tileset1 = textures->Get(ID_TEX_TILESET);
	sprite = new CSprite(500000, 0, 0, 256, 64, tileset1);
	map = new	Map(24, 96, sprite, 16, 16);
	map->LoadMatrixMap("Castlevania\\Mapstate.txt");


	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);


	sprites->Add(10001, 434, 0, 469, 64, texSimon);		// đứng im phải

	sprites->Add(10002, 375, 0, 401, 64, texSimon);		// đi phải
	sprites->Add(10003, 314, 0, 344, 64, texSimon);

	sprites->Add(10004, 120, 0, 171, 64, texSimon);		// đánh phải			
	sprites->Add(10005, 74,0,111,64, texSimon);
	sprites->Add(10006, 14, 0, 59, 64, texSimon);
	

	sprites->Add(10018, 196, 17, 230, 64, texSimon); // ngồi phải

	sprites->Add(10020, 196, 17, 230, 64, texSimon); // nhảy phải

	LPDIRECT3DTEXTURE9 texSimon2 = textures->Get(ID_TEX_SIMON_2);
	sprites->Add(10011, 11, 0, 47, 64, texSimon2);		// đứng im trái

	sprites->Add(10012, 74, 0, 107, 64, texSimon2);		// đi trái
	sprites->Add(10013, 132, 0, 171, 64, texSimon2);

	sprites->Add(10014, 313, 0, 358, 64, texSimon2);		// đánh trái				
	sprites->Add(10015, 372, 0, 404, 64, texSimon2);
	sprites->Add(10016, 421, 0, 464, 64, texSimon2);


	sprites->Add(10019, 254, 17, 286, 64, texSimon2); // ngồi trái

	sprites->Add(10021, 254, 17, 286, 64, texSimon2);//nhảy trái

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



	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ZOMBIE);
	sprites->Add(30001, 0, 0, 32, 64, texEnemy); // đi trái 
	sprites->Add(30002, 32, 0, 68, 64, texEnemy);

	LPDIRECT3DTEXTURE9 texEnemy1 = textures->Get(ID_TEX_PANTHER);
	sprites->Add(30011, 0, 0, 50, 32, texEnemy1); // nằm chờ
	sprites->Add(30012, 64, 0, 120, 32, texEnemy1);
	sprites->Add(30013, 126, 0, 188, 32, texEnemy1);
	sprites->Add(30014, 188, 0, 256, 32, texEnemy1);

	LPDIRECT3DTEXTURE9 texEnemy2 = textures->Get(ID_TEX_FIRE);
	sprites->Add(40011, 0, 0, 32, 64, texEnemy2);
	sprites->Add(40012, 32, 0, 64, 64, texEnemy2);


	LPANIMATION ani;
	// start simon
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


	ani = new CAnimation(100);	//chết	
	ani->Add(10099);
	animations->Add(599, ani);
	// end simon
	ani = new CAnimation(100);	//đất
	ani->Add(20001);
	animations->Add(601, ani);

	ani = new CAnimation(100); //zombie
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(602, ani);

	ani = new CAnimation(100); //fire
	ani->Add(40011);
	ani->Add(40012);
	animations->Add(700, ani);

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
	simon->AddAnimation(599);	//chết
	simon->whip->AddAnimation(408);//roi phải
	simon->whip->AddAnimation(409);//roi trái
	simon->SetPosition(150, 327);
	objects.push_back(simon);
	for (int i = 0; i < 50; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(0 + i * 32.0f, 407);
		objects.push_back(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Zombie *zombie = new Zombie();
		zombie->AddAnimation(602);
		zombie->SetPosition(0 + i * 64, 350);
		zombie->SetState(ZOMBIE_STATE_WALKING);
		objects.push_back(zombie);
	}

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
	

}


void Update(DWORD dt)
{	
	if (GetTickCount() - keyHandler->timeStartJump > 100)
	{
		keyHandler->NotOnJump = true;
	}
	if (GetTickCount() - keyHandler->timeStartWhip > 450)
	{
		keyHandler->NotOnWhip = true;
	}
	float x, y;
	simon->GetPosition(x, y);
	
	if (x > 1536) {
		for (int i = objects.size(); i > 51  ; i--)
			objects.pop_back();
		simon->SetPosition(50, 150);
		lv2 = true;
	}

		
		
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	
	

	if (x > SCREEN_WIDTH / 2 && x< 1536 - SCREEN_WIDTH/2)
	{
		game->x_cam = x - SCREEN_WIDTH / 2;
		game->y_cam = 0 ;
	}
	else if(x > 1536 - SCREEN_WIDTH / 2){
		game->x_cam = 1536-SCREEN_WIDTH;
		game->y_cam = 0;
	}
	else
	{
		game->x_cam =0;
		game->y_cam = 0;
	}
	map->Draw(game->x_cam, game->y_cam);
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
		/*if (lv2 == false) {
			if (x < SCREEN_WIDTH / 2) {
				D3DXVECTOR3 p(0, 80, 0);
				RECT r;
				r.left = 0;
				r.top = 0;
				r.right = 640;
				r.bottom = 480;
				spriteHandler->Draw(tex, &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
			}
			else if (x < 1536 - SCREEN_WIDTH / 2 && x > SCREEN_WIDTH / 2) {

				D3DXVECTOR3 p(0, 80, 0);
				RECT r;
				r.left = x - SCREEN_WIDTH / 2;
				r.top = 0;
				r.right = 640 + x;
				r.bottom = 480;
				spriteHandler->Draw(tex, &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
			}
			else {
				D3DXVECTOR3 p(0, 80, 0);
				RECT r;
				r.left = 1536 - SCREEN_WIDTH;
				r.top = 0;
				r.right = 640 + x;
				r.bottom = 480;
				spriteHandler->Draw(tex, &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
			}
		}
		else {
			D3DXVECTOR3 p(0, 80, 0);
			RECT r;
			r.left = 0;
			r.top = 0;
			r.right = 640;
			r.bottom = 480;
			spriteHandler->Draw(tex2, &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
		}*/
		
		for (int i = 0; i < objects.size(); i++)
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

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH , SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}