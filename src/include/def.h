#ifndef __DEF_H_
#define __DEF_H_

/*********************************************************************
 * Defines 
*********************************************************************/

#define BKP_DEFAULT_WIDTH 1920
#define BKP_DEFAULT_HEIGHT 1080

#define BKP_INPUT_PRESSURE_TIMER 200 

#define BKP_MAX_PLAN 5

//Player Constante
#define BKP_PLAYER_TRANSITION_DELAY 20

/*********************************************************************
 * Type def & enum
*********************************************************************/

enum{BKP_FALSE = 0,BKP_TRUE};

enum{BKP_DOWN,BKP_RIGHT,BKP_UP,BKP_LEFT};

enum{BKP_AGENT_NONE= 0,BKP_AGENT_PLAYER,BKP_AGENT_NPC,BKP_AGENT_OBJECT,BKP_AGENT_TOTAL};

enum{OBJ_TYPE_MISSILE,OBJ_TYPE_SILO,OBJ_TYPE_BOMB,OBJ_TYPE_BOMB_GENERATOR,
	 OBJ_TYPE_SHIELD,OBJ_TYPE_TEXT,OBJ_TYPE_LEURE,
	 OBJ_TYPE_DECOR,OBJ_TYPE_DECOR_SILO,OBJ_TYPE_BUY,
	 OBJ_TOTAL,
	};

enum{OBJ_NAME_BUYSILO,OBJ_NAME_BUYFURY,OBJ_NAME_BUYGATLING,OBJ_NAME_BUYSHILD,OBJ_NAME_DOME,
	 OBJ_NAME_LEURE,
	 OBJ_NAME_WHITEBALL,OBJ_NAME_WHITEBALL2,OBJ_NAME_DECOR,OBJ_NAME_NONE,OBJ_NAME_NOTICE,OBJ_NAME_HIDEBUY,
	};

enum{BKP_STATE_NONE,BKP_STATE_IDLE     ,BKP_STATE_STARTWALKING,BKP_STATE_WALKING,BKP_STATE_TRANSITION,
	BKP_STATE_TURN, BKP_STATE_STARTJUMP,BKP_STATE_JUMP        ,BKP_STATE_TOPJUMP,BKP_STATE_FALLING,
	BKP_STATE_OPENFIRE,BKP_STATE_FIRE,BKP_STATE_RELOADING,
	BKP_STATE_MOTION,
	BKP_STATE_DESTROY,BKP_STATE_DYING,BKP_STATE_STANDBY,BKP_STATE_DEAD,	
	BKP_STATE_ROTATE_STATIC,BKP_STATE_ROTATE,
	BKP_STATE_TOTAL};

/*********************************************************************
 * Macro 
*********************************************************************/

/*********************************************************************
 * Struct 
*********************************************************************/

/*********************************************************************
 * Global 
*********************************************************************/

/*********************************************************************
 * Struct 
*********************************************************************/

/*********************************************************************
 * Functions 
*********************************************************************/

#endif

