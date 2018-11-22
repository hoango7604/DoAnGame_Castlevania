#include "GameObject.h"

#define BIGFIRE_BBOX_WIDTH  32
#define BIGFIRE_BBOX_HEIGHT 32

class BigFire : public CGameObject
{
	
public:
	bool isHited;
	BigFire() :CGameObject() {
		isHited = false;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Update();
};
