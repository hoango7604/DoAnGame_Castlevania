#define SIMON_WALKING_SPEED				0.1f 
#define SIMON_JUMP_SPEED_Y				0.52f
#define SIMON_JUMP_DEFLECT_SPEED		0.2f
#define SIMON_GRAVITY					0.002f
#define SIMON_DIE_DEFLECT_SPEED			0.5f
#define SIMON_SIT_TO_STAND				15

//state
#define SIMON_STATE_IDLE				0
#define SIMON_STATE_UNTOUCHABLE			100
#define SIMON_STATE_DIE					200
#define SIMON_STATE_STAIR_MOVING		300
#define SIMON_STATE_SIT					500
#define SIMON_STATE_WALK				700

//action
#define SIMON_ACTION_JUMP				300
#define SIMON_ACTION_ATTACK				600

//animation
#define SIMON_ANI_IDLE_RIGHT			0
#define SIMON_ANI_IDLE_LEFT				1
#define SIMON_ANI_WALKING_RIGHT			2
#define SIMON_ANI_WALKING_LEFT			3
#define SIMON_ANI_ATTACK_RIGHT			4
#define SIMON_ANI_ATTACK_LEFT			5
#define SIMON_ANI_SIT_RIGHT				6
#define SIMON_ANI_SIT_LEFT				7
#define SIMON_ANI_JUMP_RIGHT			8
#define SIMON_ANI_JUMP_LEFT				9
#define SIMON_ANI_SIT_ATTACK_RIGHT		10
#define SIMON_ANI_SIT_ATTACK_LEFT		11
#define SIMON_ANI_HITUP_RLADDER			12 // hit right on stair which has up-direction from left to right
#define SIMON_ANI_HITDOWN_LLADDER		13  
#define SIMON_ANI_HITDOWN_RLADDER		14 //hit right on stair which has up-direction from right to left 
#define SIMON_ANI_HITUP_LLADDER			15 //use dik_ and nx to select ani
#define SIMON_ANI_WALKUP_RLADDER		16 // walk up on stair which has up-direction from left to right
#define SIMON_ANI_WALKDOWN_RLADDER		17 //
#define SIMON_ANI_WALKUP_LLADDER		18
#define SIMON_ANI_WALKDOWN_LLADDER		19
#define SIMON_ANI_HURT_LEFT				20
#define SIMON_ANI_HURT_RIGHT			21
#define SIMON_ANI_DIE					22

// Whip
#define WHIP_RIGHT 						0
#define WHIP_LEFT						1

#define WHIP_BBOX_WIDTH					125
#define WHIP_BBOX_HEIGHT				15

// Zombie
#define ZOMBIE_WALKING_SPEED			0.07f

#define ZOMBIE_BBOX_WIDTH				32
#define ZOMBIE_BBOX_HEIGHT				32
#define ZOMBIE_BBOX_HEIGHT_DIE			9

#define ZOMBIE_STATE_WALKING			100
#define ZOMBIE_STATE_DIE				200

#define ZOMBIE_ANI_WALKING_LEFT			0
#define ZOMBIE_ANI_WALKING_RIGHT		1
#define ZOMBIE_ANI_DIE					2

// Check stair
#define CHECKSTAIR_UP_LEFT				0
#define CHECKSTAIR_UP_RIGHT				1
#define CHECKSTAIR_DOWN_LEFT			2
#define CHECKSTAIR_DOWN_RIGHT			3

#define CHECKSTAIR_BBOX_WIDTH			32
#define CHECKSTAIR_BBOX_HEIGHT			32

// Timer
#define SIMON_TIMER_ATTACK				450

#define	SIMON_LEVEL_SMALL				1
#define	SIMON_LEVEL_BIG					2

#define SIMON_STAND_BBOX_WIDTH			36
#define SIMON_STAND_BBOX_HEIGHT			60
#define SIMON_SIT_BBOX_WIDTH			36
#define SIMON_SIT_BBOX_HEIGHT			47

#define SIMON_UNTOUCHABLE_TIME			5000

#define WINDOW_CLASS_NAME L"Castlevania"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH					640
#define SCREEN_HEIGHT					580
#define MAX_WIDTH_LV1					1536
#define MAX_WIDTH_LV2					5634
#define MAX_FRAME_RATE					120

//texture
#define ID_TEX_SIMON					0
#define ID_TEX_LV1						10
#define ID_TEX_LV1_2					15
#define ID_TEX_BRICK					20
#define ID_TEX_SIMON_2					30
#define ID_TEX_PANTHER					25
#define ID_TEX_ZOMBIE					35
#define ID_TEX_ZOMBIE_RIGHT				36
#define ID_TEX_FIRE						40
#define ID_TEX_CANDLE					45
#define ID_TEX_WHIP						50
#define ID_TEX_WHIP_2					55
#define ID_TEX_TILESET					60
#define ID_TEX_TILESET_2				61
#define ID_TEX_BRICK2					65
#define ID_TEX_LADDER					70
#define ID_TEX_LADDER_LEFT				75
#define ID_TEX_STAIR_BOT				80
#define ID_TEX_STAIR_TOP				85
#define ID_TEX_MERMAN_RIGHT				90
#define ID_TEX_MERMAN_LEFT				95
#define ID_TEX_BAT                      100 