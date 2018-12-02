#include "GameObject.h"
#include "Heart.h"
#include "Effect.h"
#include "WhipItem.h"

class BigFire : public CGameObject
{
	
public:
	bool isHitted;
	Heart *heart;
	Effect *effect;
	WhipItem *whipitem;
	BigFire() :CGameObject() {
		heart = new Heart();
		whipitem = new WhipItem();
		isHitted = false;
	}
	void SetHit();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};
