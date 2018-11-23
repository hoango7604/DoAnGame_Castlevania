#include "UI.h"

UI::UI()
{
	simonHP = NULL;
	simonHP = NULL;
}


UI::~UI()
{
	delete[] simonHPList;
	delete[] enemyHPList;
}

bool UI::Initialize(LPDIRECT3DDEVICE9 d3ddv)
{
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
	information += "ENEMY                P=3\n";
	return false;
}

void UI::Update(int bossHP, int time, int life, int stage)
{
}

void UI::Render()
{
	RECT newRect;
	SetRect(&newRect, 0, 0, SCREEN_WIDTH, 120);
	if (font)
		font->DrawTextA(NULL, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 0, 255));
}


