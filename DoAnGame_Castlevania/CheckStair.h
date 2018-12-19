#pragma once

#include "GameObject.h"

class CheckStair : public CGameObject
{

public:
	int id;
	int type;

	void SetId(int id) { this->id = id; }
	int GetId() { return id; }

	void SetType(int type) { this->type = type; }
	int GetType() { return type; }

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
