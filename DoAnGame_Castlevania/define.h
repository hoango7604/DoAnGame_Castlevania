#define SIMON_WALKING_SPEED		0.1f 
//0.1f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_MOVE			500

#define SIMON_ANI_BIG_IDLE_RIGHT		0
#define SIMON_ANI_BIG_IDLE_LEFT			1
#define SIMON_ANI_SMALL_IDLE_RIGHT		2
#define SIMON_ANI_SMALL_IDLE_LEFT			3

#define SIMON_ANI_BIG_WALKING_RIGHT			4
#define SIMON_ANI_BIG_WALKING_LEFT			5
#define SIMON_ANI_SMALL_WALKING_RIGHT		6
#define SIMON_ANI_SMALL_WALKING_LEFT		7

#define SIMON_ANI_DIE				8

#define	SIMON_LEVEL_SMALL	1
#define	SIMON_LEVEL_BIG		2

#define SIMON_BIG_BBOX_WIDTH  15
#define SIMON_BIG_BBOX_HEIGHT 60

#define SIMON_SMALL_BBOX_WIDTH  13
#define SIMON_SMALL_BBOX_HEIGHT 15

#define SIMON_UNTOUCHABLE_TIME 5000

#define WINDOW_CLASS_NAME L"Castlevania"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 580

#define MAX_FRAME_RATE 120

#define ID_TEX_SIMON 0
#define ID_TEX_LV1 10
#define ID_TEX_LV1_2 15
#define ID_TEX_BRICK 20
#define ID_TEX_SIMON_2 30
#define ID_TEX_PANTHER 25
#define ID_TEX_ZOMBIE 35
#define ID_TEX_FIRE 40
#define ID_TEX_CANDLE 45
#define ID_TEX_WHIP 50