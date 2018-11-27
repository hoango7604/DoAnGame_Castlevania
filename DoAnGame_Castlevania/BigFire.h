#include "GameObject.h"
#include "Heart.h"
#define BIGFIRE_BBOX_WIDTH  32
#define BIGFIRE_BBOX_HEIGHT 32

class BigFire : public CGameObject
{
	
public:
	bool isHited;
	Heart *heart;
	BigFire() :CGameObject() {
		isHited = false;
		heart = new Heart();
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
		heart->x = x;
		heart->y = y;
	}
};
