#include "GridObjects.h"

void GridObjects::GetPoint(float &left, float &right)
{
	left = this->left;
	right = this->right;
}

vector<LPGAMEOBJECT> GridObjects::GetListObject()
{
	return listObject;
}

void GridObjects::AddObject(LPGAMEOBJECT object)
{
	listObject.push_back(object);
}

void GridObjects::RemoveObject(int i)
{
	listObject.erase(listObject.begin() + i);
}

ListGrids *ListGrids::instance = NULL;

#define GRID_WIDTH	(SCREEN_WIDTH / 2)

void ListGrids::InitList(float mapWidth)
{
	GridObjects *grid;
	int gridCount = ceil(mapWidth / GRID_WIDTH);
	for (int i = 0; i < gridCount; i++)
	{
		float left, right;
		left = i * GRID_WIDTH;
		right = (i + 1) * GRID_WIDTH;
		grid = new GridObjects(left, right);
		listGrids.push_back(grid);
	}
}

void ListGrids::ReleaseList()
{
	vector<LPGAMEOBJECT> listRemoveObjects;

	int listGridSize = listGrids.size();
	for (int i = 0; i < listGridSize; i++)
	{
		int listObjectSize = listGrids[i]->listObject.size();
		GridObjects *grid = listGrids[i];
		for (int j = 0; j < listObjectSize; j++)
		{
			listRemoveObjects.push_back(grid->listObject[j]);
		}
	}

	for (int i = 0; i < listRemoveObjects.size(); i++)
	{
		RemoveObject(listRemoveObjects[i]);
		delete listRemoveObjects[i];
	}

	for (int i = 0; i < listGrids.size(); )
	{
		GridObjects *grid = listGrids[i];
		RemoveGrid(0);
		delete grid;
	}
}

void ListGrids::RemoveGrid(int i)
{
	listGrids.erase(listGrids.begin() + i);
}

void ListGrids::AddObject(LPGAMEOBJECT object)
{
	

	// Dựa vào vị trí x của object để đặt vật vào grid tương ứng
	float objectX = object->x;
	// Tránh trường hợp vật ra khỏi map
	if (objectX < 0)
		objectX = 0;

	int gridNumber = floor(objectX / GRID_WIDTH);
	// Tránh trường hợp vật ra khỏi map
	if (gridNumber == this->listGrids.size())
		gridNumber = this->listGrids.size() - 1;

	object->gridNumber = gridNumber;
	listGrids[gridNumber]->AddObject(object);
}

void ListGrids::AddObject(LPCSTR fileSource,Simon *simon , CGame *game)
{
	fstream pFile;
	pFile.open(fileSource, fstream::in);
	string linestring;
	float x,  y;
	int animation;
	int anotation;
	while (pFile.good())
	{

		getline(pFile, linestring);
		stringstream ss;
		ss.str(linestring);
		if (linestring == "BigFire")
			anotation = 0;
		else if (linestring == "CheckPoint")
			anotation = 1;
		else if (linestring == "Wall")
			anotation = 2;
		else if (linestring == "Candle")
			anotation = 3;
		else if (linestring == "CheckStair")
			anotation = 4;
		else if (linestring == "Panther")
			anotation = 5;
		else if (linestring == "Door")
			anotation = 6;
		else if (linestring == "RedBat")
			anotation = 7;
		else if (linestring == "BossBat")
			anotation = 8;
		else if (linestring == "BigBat")
			anotation = 9;
		else if (linestring == "Skeleton")
			anotation = 10;
		else if (linestring == "Dracula")
			anotation = 11;
		
		if (linestring != "BigFire" && anotation == 0)
		{
			
			ss >> animation >> x >> y;			
			BigFire *bigfire = new BigFire();
			bigfire->AddAnimation(animation);
			bigfire->SetPosition(x, y);
			ListGrids::GetInstance()->AddObject(bigfire);
		}
		else if (linestring != "CheckPoint" && anotation == 1)
		{
			ss >> animation >> x >> y;
			CheckPoint *checkPoint = new CheckPoint();
			checkPoint->SetType(animation);
			checkPoint->SetPosition(x, y);
			ListGrids::GetInstance()->AddObject(checkPoint);
		}
		else if (linestring != "Wall" && anotation == 2)
		{
			float width, height;
			ss >> width >> height >> x >> y;
			Wall *wall = new Wall();
			wall->SetBB(width,height);
			wall->SetPosition(x, y);
			ListGrids::GetInstance()->AddObject(wall);
		}
		else if (linestring != "Candle" && anotation == 3)
		{

			ss >> animation >> x >> y;
			Candle *candle = new Candle();
			candle->AddAnimation(animation);
			candle->SetPosition(x, y);
			ListGrids::GetInstance()->AddObject(candle);
		}
		else if (linestring != "CheckStair" && anotation == 4)
		{
			int type, id;
			ss >> animation >> x >> y >> id >> type;
			CheckStair *checkstair = new CheckStair();
			checkstair->AddAnimation(animation);
			checkstair->SetPosition(x, y);
			checkstair->SetId(id);
			checkstair->SetType(type);
			ListGrids::GetInstance()->AddObject(checkstair);
		}
		else if (linestring != "Panther" && anotation == 5)
		{
			int ani, ani1, ani2, ani3,state;
			ss >> ani>>ani1>>ani2>>ani3 >> x >> y>>state;
			Panther *panther = new Panther();
			panther->AddAnimation(ani);
			panther->AddAnimation(ani1);
			panther->AddAnimation(ani2);
			panther->AddAnimation(ani3);
			panther->SetPosition(x, y);
			panther->SetState(state);
			ListGrids::GetInstance()->AddObject(panther);
		}
		else if (linestring != "Door" && anotation == 6)
		{
			int ani, ani1, ani2;
			ss >> ani >> ani1 >> ani2 >> x >> y ;
			Door *door = new Door();
			door->AddAnimation(ani);
			door->AddAnimation(ani1);
			door->AddAnimation(ani2);
			door->SetPosition(x, y);
			ListGrids::GetInstance()->AddObject(door);
		}
		else if (linestring != "RedBat" && anotation == 7)
		{
			int state,ani;
			ss >> ani >>  x >> y >>state;
			Enemy *enemy = new RedBat();
			enemy->AddAnimation(ani);
			enemy->SetPosition(x, y);
			enemy->SetState(state);
			ListGrids::GetInstance()->AddObject(enemy);
		}
		else if (linestring != "BossBat" && anotation == 8)
		{
			int state, ani,ani1;
			ss >> ani >> ani1 >> x >> y >> state;
			BossBat *bossbat = new BossBat(simon,game);
			bossbat->AddAnimation(ani);
			bossbat->AddAnimation(ani1);
			bossbat->SetPosition(x, y);
			bossbat->SetState(state);
			ListGrids::GetInstance()->AddObject(bossbat);
		}
		else if (linestring != "BigBat" && anotation == 9)
		{
			int  ani;
			ss >> ani >>x >>y;
			BigBat *bigbat;
			bigbat = new BigBat(simon);
			bigbat->AddAnimation(ani);
			bigbat->SetPosition(x, y);
			ListGrids::GetInstance()->AddObject(bigbat);
		}
		else if (linestring != "Skeleton" && anotation == 10)
		{
			int  ani,ani1,state;
			ss >> ani >>ani1 >> x >> y >>state;
			Skeleton *skeleton;
			skeleton = new Skeleton();
			skeleton->AddAnimation(ani);
			skeleton->AddAnimation(ani1);
			skeleton->SetPosition(x, y);
			skeleton->SetState(state);
			ListGrids::GetInstance()->AddObject(skeleton);
		}
		else if (linestring != "Dracula" && anotation == 11)
		{
			int  ani, ani1,ani2,ani3,ani4,ani5,ani6;
			ss >> ani >> ani1 >>ani2>>ani3>>ani4>>ani5 >>ani6 >>x >> y  ;
			Dracula *dracula;
			dracula = new Dracula(simon,game);
			dracula->AddAnimation(ani);
			dracula->AddAnimation(ani1);
			dracula->AddAnimation(ani2);
			dracula->AddAnimation(ani3);
			dracula->AddAnimation(ani4);
			dracula->AddAnimation(ani5);
			dracula->AddAnimation(ani6);
			dracula->SetPosition(x, y);
			
			ListGrids::GetInstance()->AddObject(dracula);
		}
		
	}
}

void ListGrids::RemoveObject(LPGAMEOBJECT object)
{
	GridObjects *gridObjects = listGrids[object->gridNumber];
	int listObjectSize = gridObjects->listObject.size();
	int objectNumber = -1;

	// Duyệt qua từng phần tử trong gridObjects để lấy chỉ số của object
	for (int i = 0; i < listObjectSize; i++)
	{
		if (gridObjects->listObject[i] == object)
		{
			objectNumber = i;
			break;
		}
	}

	// Nếu tồn tại object trong gridObjects thì chỉ số sẽ khác -1
	if (objectNumber != -1)
	{
		gridObjects->RemoveObject(objectNumber);
	}
}

ListGrids * ListGrids::GetInstance()
{
	if (instance == NULL) instance = new ListGrids();
	return instance;
}

vector<GridObjects*> ListGrids::GetCurrentGrids(float cam_x)
{
	vector<GridObjects*> result;
	int gridNumber = floor(cam_x / GRID_WIDTH);
	
	// Trường hợp màn hình game nằm giữa 2 grid
 	result.push_back(listGrids[gridNumber]);
	
	if (gridNumber - 1 > 0)
		result.push_back(listGrids[gridNumber - 2]);
	if (gridNumber > 0)
		result.push_back(listGrids[gridNumber - 1]);

	if (gridNumber + 1 < listGrids.size())
		result.push_back(listGrids[gridNumber + 1]);
	if (gridNumber + 2 < listGrids.size())
		result.push_back(listGrids[gridNumber + 2]);

	return result;
}

void ListGrids::UpdateObjectInGrid(LPGAMEOBJECT object)
{
	// Dựa vào vị trí x của object để đặt vật vào grid tương ứng
	int gridNumber = floor(object->x / GRID_WIDTH);

	if (gridNumber < 0)
		gridNumber = 0;
	else if (gridNumber >= listGrids.size())
		gridNumber = listGrids.size() - 1;

	if (gridNumber != object->gridNumber)
	{
		// Xóa object khỏi grid cũ
		RemoveObject(object);

		// Đổi gridNumber hiện tại của object và thêm object vào grid mới
		object->gridNumber = gridNumber;
		listGrids[gridNumber]->AddObject(object);
	}
}
