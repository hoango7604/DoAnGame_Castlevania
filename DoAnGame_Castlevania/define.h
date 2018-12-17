//file path
#define GAME_FONT L"Castlevania\\prstart.ttf"

#define SIMON_WALKING_SPEED				0.15f 
#define SIMON_CLIMBING_SPEED_X			0.05f
#define SIMON_CLIMBING_SPEED_Y			0.05f
#define SIMON_JUMP_SPEED_Y				0.52f
#define SIMON_JUMP_DEFLECT_SPEED		0.2f
#define SIMON_LEVELUP_SPEED				0.01f
#define SIMON_GRAVITY					0.0015f
#define SIMON_DIE_DEFLECT_SPEED			0.3f
#define SIMON_SIT_TO_STAND				15
#define SIMON_ON_LEFT_CHECKSTAIR		1
#define SIMON_ON_RIGHT_CHECKSTAIR		2
#define SIMON_ONSTAIR_ERR_RANGE			3

//state
#define SIMON_STATE_IDLE				0
#define SIMON_STATE_UNTOUCHABLE			100
#define SIMON_STATE_DIE					200
#define SIMON_STATE_ONCHECKSTAIR		300
#define SIMON_STATE_SIT					500
#define SIMON_STATE_WALK				600
#define SIMON_STATE_ONSTAIR				700
#define SIMON_STATE_ONSTAIR_IDLE		800
#define SIMON_STATE_HURT				900

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
#define SIMON_ANI_WALKDOWN_LLADDER		17 
#define SIMON_ANI_WALKUP_LLADDER		18
#define SIMON_ANI_WALKDOWN_RLADDER		19
#define SIMON_ANI_IDLE_WALKUP_RLADDER	20
#define SIMON_ANI_IDLE_WALKDOWN_LLADDER	21
#define SIMON_ANI_IDLE_WALKUP_LLADDER	22
#define SIMON_ANI_IDLE_WALKDOWN_RLADDER	23
#define SIMON_ANI_HURT_LEFT				24
#define SIMON_ANI_HURT_RIGHT			25
#define SIMON_ANI_DIE					26

// Whip
#define WHIP_RIGHT 						0
#define WHIP_LEFT						1
#define WHIP_RIGHT_1					2
#define WHIP_LEFT_1						3
#define WHIP_RIGHT_2					4
#define WHIP_LEFT_2						5

#define WHIP_LV0_BBOX_WIDTH				100
#define WHIP_LV0_BBOX_HEIGHT			15
#define WHIP_LV1_BBOX_WIDTH				105
#define WHIP_LV1_BBOX_HEIGHT			15
#define WHIP_LV2_BBOX_WIDTH				125
#define WHIP_LV2_BBOX_HEIGHT			15

// Big fire
#define BIGFIRE_BBOX_WIDTH				32
#define BIGFIRE_BBOX_HEIGHT				64

// Candle
#define CANDLE_BBOX_WIDTH				16
#define CANDLE_BBOX_HEIGHT				50

// Heart
#define HEART_BBOX_WIDTH				16
#define HEART_BBOX_HEIGHT				16

// Whip item
#define WHIPITEM_BBOX_WIDTH				32
#define WHIPITEM_BBOX_HEIGHT			32

// Knife
#define KNIFE_BBOX_WIDTH				32
#define KNIFE_BBOX_HEIGHT				18

#define KNIFE_SPEED						0.7f

// Axe
#define AXE_BBOX_WIDTH					32
#define AXE_BBOX_HEIGHT					20

#define AXE_SPEED_X						0.3f
#define AXE_SPEED_Y						0.4f

// Holy water
#define HOLYWATER_BBOX_WIDTH			32
#define HOLYWATER_BBOX_HEIGHT			23

#define HOLYWATER_SPEED_X				0.2f
#define HOLYWATER_SPEED_Y				0.25f

// Cross
#define CROSS_BBOX_WIDTH				16
#define CROSS_BBOX_HEIGHT				16

#define CROSS_SPEED						0.5f

// Rosary
#define ROSARY_BBOX_WIDTH				32
#define ROSARY_BBOX_HEIGHT				32

// Clock
#define CLOCK_BBOX_WIDTH				32
#define CLOCK_BBOX_HEIGHT				32

//prize
#define PRIZE_BBOX_WIDTH				32
#define PRIZE_BBOX_HEIGHT				32

// Money
#define MONEY_BBOX_WIDTH				30
#define MONEY_BBOX_HEIGHT				30

// Zombie
#define ZOMBIE_WALKING_SPEED			0.07f

#define ZOMBIE_BBOX_WIDTH				32
#define ZOMBIE_BBOX_HEIGHT				64
#define ZOMBIE_BBOX_HEIGHT_DIE			9

#define ZOMBIE_STATE_WALKING			100
#define ZOMBIE_STATE_DIE				200

#define ZOMBIE_ANI_WALKING_LEFT			0
#define ZOMBIE_ANI_WALKING_RIGHT		1
#define ZOMBIE_ANI_DIE					2

// Bat
#define BAT_FLYING_SPEED				0.12f
#define BAT_FLYING_GRAVITY				0.0005f
#define BAT_STABLE_GRAVITY				0.25f

#define BAT_BBOX_WIDTH					32
#define BAT_BBOX_HEIGHT					40

#define BAT_STATE_FLYING				0

#define BAT_ANI_FLYING_LEFT				0
#define BAT_ANI_FLYING_RIGHT			1

//Panther
#define PANTHER_WALKING_SPEED			0.3f
#define PANTHER_JUMPING_SPEED			0.2f
#define PANTHER_JUMP_POINT_1_X			1380
#define PANTHER_JUMP_POINT_2_X			1510
#define PANTHER_JUMP_POINT_3_X			1865

#define PANTHER_DISTANCE_SIMON_X		140
#define PANTHER_DISTANCE_SIMON_Y		110

#define PANTHER_BBOX_WIDTH				64
#define PANTHER_BBOX_HEIGHT				32

#define PANTHER_STATE_WAIT				100
#define PANTHER_STATE_JUMP				150
#define PANTHER_STATE_RUN				200

#define PANTHER_ANI_WAIT				0
#define PANTHER_ANI_RUN_LEFT			1
#define PANTHER_ANI_RUN_RIGHT			2
#define PANTHER_ANI_JUMP				3

//MerMan
#define MERMAN_BBOX_WIDTH				32
#define MERMAN_BBOX_HEIGHT				64
 
#define MERMAN_STATE_JUMP				5
#define MERMAN_STATE_WALK				10
#define MERMAN_STATE_HIT				15
#define MERMAN_STATE_DIE				20

#define MERMAN_ANI_JUMP					0
#define MERMAN_ANI_WALK_LEFT			1
#define MERMAN_ANI_HIT_LEFT				2
#define MERMAN_ANI_WALK_RIGHT			3
#define MERMAN_ANI_HIT_RIGHT			4
#define MERMAN_ANI_DIE					5

#define MERMAN_ATTACK_DURATION_TIME		2000
#define MERMAN_ATTACK_TIME				1000

// Enemy bullet
#define ENEMY_BULLET_BBOX_WIDTH			10
#define ENEMY_BULLET_BBOX_HEIGHT		10

#define ENEMY_BULLET_SPEED				0.2f

//BossBat
#define BOSSBAT_BBOX_WIDTH				100	
#define BOSSBAT_BBOX_HEIGHT				45

#define BOSSBAT_STATE_WAIT				500
#define BOSSBAT_STATE_ATTACK			600
#define BOSSBAT_STATE_DIE				700

#define BOSSBAT_ANI_WAIT				0
#define BOSSBAT_ANI_ATTACK				1

#define BOSSBAT_ATTACK_TIME				400
#define BOSSBAT_WAITING_TIME			1000
#define BOSSBAT_HURTING_TIME			1000

#define BOSSBAT_SPEED_UP				0.1f
#define BOSSBAT_SPEED_DOWN				0.15f

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

#define SIMON_STAND_BBOX_WIDTH			32
#define SIMON_STAND_BBOX_HEIGHT			60
#define SIMON_SIT_BBOX_WIDTH			32
#define SIMON_SIT_BBOX_HEIGHT			47

#define SIMON_UNTOUCHABLE_TIME			2000

#define WINDOW_CLASS_NAME L"Castlevania"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define ROSARY_COLOR D3DCOLOR_XRGB(131, 135, 142)

#define SCREEN_WIDTH					640
#define SCREEN_HEIGHT					580
#define MAX_WIDTH_LV1					1536
#define MAX_WIDTH_LV2					3085
#define MAX_WIDTH_LV2_1					4096
#define MAX_WIDTH_LV2_2					1024
#define MAX_WIDTH_BOSS					5650
#define MAX_WIDTH_LV3_5					3072
#define MAX_WIDTH_LV3_4					1536
#define MAX_WIDTH_LV3_3					1536
#define MAX_WIDTH_LV3_2					544
#define MAX_WIDTH_LV3_1					506
#define MAX_FRAME_RATE					120

//texture
#define ID_TEX_SIMON					1
#define ID_TEX_SIMON_2					2
#define ID_TEX_BRICK					3
#define ID_TEX_BRICK2					4
#define ID_TEX_ZOMBIE					5
#define ID_TEX_ZOMBIE_RIGHT				6
#define ID_TEX_PANTHER					7
#define ID_TEX_PANTHER_RIGHT			8
#define ID_TEX_FIRE						9
#define ID_TEX_CANDLE					10
#define ID_TEX_WHIP						11
#define ID_TEX_WHIP_2					12
#define ID_TEX_TILESET					13
#define ID_TEX_TILESET_2				14
#define ID_TEX_TILESET_3				15
#define ID_TEX_LADDER					16
#define ID_TEX_LADDER_LEFT				17
#define ID_TEX_BBOX						18
#define ID_TEX_STAIR_BOT				19
#define ID_TEX_STAIR_TOP				20
#define ID_TEX_MERMAN_LEFT				21
#define ID_TEX_MERMAN_RIGHT				22
#define ID_TEX_BAT                      23
#define ID_TEX_BOSS						24
#define ID_TEX_AXE_ACTION				25
#define ID_TEX_HOLY_WATER				26
#define ID_TEX_HOLY_WATER_ACTION		27
#define ID_TEX_CROSS_ACTION				28
#define ID_TEX_KNIFE_ACTION				29
#define ID_TEX_UI						30
#define ID_TEX_EFFECT1					31
#define ID_TEX_EFFECT2					32
#define ID_TEX_WHIP_VIP					33
#define ID_TEX_MONEY					34
#define ID_TEX_ROSARY					35
#define ID_TEX_TILESET31				36
#define ID_TEX_TILESET32				37
#define ID_TEX_TILESET33				38
#define ID_TEX_TILESET34				39
#define ID_TEX_TILESET35				40
#define ID_TEX_GATE						41
#define ID_TEX_BOSSES					42
#define ID_TEX_BOSSES_RIGHT				43
#define ID_TEX_ENEMIES					44
#define ID_TEX_ENEMIES_RIGHT			45

#define BRICK_BBOX_WIDTH				32
#define BRICK_BBOX_HEIGHT				32

#define WALL_BBOX_WIDTH					128
#define WALL_BBOX_HEIGHT				160

#define DOOR_BBOX_WIDTH					32
#define DOOR_BBOX_HEIGHT				96

#define DOOR_STATE_DEACTIVE				10
#define DOOR_STATE_ACTIVE				11
#define DOOR_STATE_0					12
#define DOOR_STATE_01					13


#define DOOR_ANI_OPEN					0
#define DOOR_ANI_CLOSE					1

#define EFFECT_TIME						200
#define LEVELUP_TIME					3000
#define ITEM_LIVE_TIME					5000
#define CLOCK_WEAPON_FREEZE_TIME		2000

// Item
#define ITEM_WHIPITEM					808
#define ITEM_HEART						805
#define ITEM_MONEY						810
#define ITEM_AXE						4430
#define ITEM_HOLYWATER					4431
#define ITEM_CROSS						4432
#define ITEM_KNIFE						4433
#define ITEM_ROSARY						4434
#define ITEM_CLOCK						4435
#define ITEM_PRIZE						813

// Weapon in action
#define WEAPON_ANI_AXE					430
#define WEAPON_ANI_HOLYWATER			431
#define WEAPON_ANI_HOLYWATER_FIRE		4310
#define WEAPON_ANI_CROSS				432
#define WEAPON_ANI_KNIFE_LEFT			4330
#define WEAPON_ANI_KNIFE_RIGHT			4331

#define WEAPON_ACTIVATE_TIME			300

// Checkpoint type
#define CHECKPOINT_LEVELUP				0
#define CHECKPOINT_BONUS				1