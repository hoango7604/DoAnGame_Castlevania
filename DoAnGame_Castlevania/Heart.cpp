#include "Heart.h"

void Heart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Ground *>(coObjects->at(i)))
		{
			Ground *ground = dynamic_cast<Ground *>(coObjects->at(i));
			float zl, zr, zt, zb;
			ground->GetBoundingBox(zl, zt, zr, zb);
			if (x < zl && x + HEART_BBOX_WIDTH > zr && y > zt && y + HEART_BBOX_HEIGHT < zb)
			{
				this->SetSpeed(0, 0);
			}
		}
	}
	y += vy * dt;
}

void Heart::Render()
{
	//animations[0]->Render(x, y);
	RenderBoundingBox();
}

void Heart::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + HEART_BBOX_WIDTH;
	b = y + HEART_BBOX_HEIGHT;
}
