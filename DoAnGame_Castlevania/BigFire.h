#include "GameObject.h"

#define BIGFIRE_BBOX_WIDTH  32
#define BIGFIRE_BBOX_HEIGHT 32

class BigFire : public CGameObject
{
	ViewPort bfviewport;
public:
	BigFire() :CGameObject() {
	bfviewport: ViewPort(0, 0);
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Update();
};
