#pragma once
#include "GameObject.h"
#include "define.h"
#include <math.h>
#include "BigFire.h"
#include "CheckPoint.h"
#include "Wall.h"
#include "Candle.h"
#include "Zombie.h"
#include "Panther.h"
#include "Door.h"
#include "RedBat.h"
#include "Dracula.h"
#include "SuperDracula.h"
#include "BossBat.h"
#include "BigBat.h"
#include "Skeleton.h"
#include "Bird.h"
#include "Hunchback.h"
class GridObjects
{
	float left, right;
	
public:
	vector<LPGAMEOBJECT> listObject;

	GridObjects(float left, float right) 
	{ 
		this->left = left;
		this->right = right;
	}

	void GetPoint(float &left, float &right);
	vector<LPGAMEOBJECT> GetListObject();

	void AddObject(LPGAMEOBJECT object);
	void RemoveObject(int i);
};

class ListGrids
{
	static ListGrids *instance;
	vector<GridObjects*> listGrids;

public:
	void InitList(float mapWidth);
	void ReleaseList();

	void RemoveGrid(int i);

	void AddObject(LPGAMEOBJECT object);
	void AddObject(LPCSTR fileSource,Simon *simon,CGame *game);
	//void AddObject(LPCSTR fileSource, float x, int y);
	void AddObject(LPCSTR fileSource, float x,float y );
	void AddObject(LPCSTR fileSource, float x, float y,Simon *simon);
	void AddObject(LPCSTR fileSource, float x, int count1, int lv, Simon *simon);
	void RemoveObject(LPGAMEOBJECT object);

	static ListGrids *GetInstance();
	vector<GridObjects*> GetCurrentGrids(float cam_x);
	void UpdateObjectInGrid(LPGAMEOBJECT object);
};