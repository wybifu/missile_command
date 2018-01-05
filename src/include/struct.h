#ifndef __STRUCT_
#define __STRUCT_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "def.h"

/*********************************************************************
 * Defines 
*********************************************************************/


/*********************************************************************
 * Type def & enum
*********************************************************************/

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
typedef struct BKP_BASIC_HITBOX
{
	float x,y;
	float w,h;
}BKP_BASIC_HITBOX;

/*---------------------------- Position --------------------------*/
typedef struct BKP_Position
{
	float x,y,z;
}BKP_Position;

/*------------------------- Camera View Port -----------------------*/
typedef struct BKP_AABB
{
	float x,y,z;
	float halfw,halfh;
}BKP_AABB;

/*------------------------- Camera View Port -----------------------*/
typedef struct BKP_ViewPort
{
	int w,h;
	BKP_Position pos;

}BKP_ViewPort;

/*---------------------------- Game Screen --------------------------*/
typedef struct BKP_Screen
{
	int w,h; 		//width heigth
	int defw,defh;	//expected default size for creating ww and hh
	float ww,hh;		//ratio for size adjust
	
	int touchscreen ;  // activate touche screen or not
	int ndisplay,display_index;
	int cursor;

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Rect * RectDisplay;

	BKP_ViewPort vp;
	SDL_Texture * bg;
	SDL_Texture * logo;

}BKP_Screen;

/*------------------------------ Input ----------------------------*/
typedef struct InputKey
{
	int pressed;
	unsigned int s_time;
}InputKey;

/*------------------------------ Input ----------------------------*/
typedef struct InputMouse
{
	InputKey l_click, r_click,m_click;
	float x,y;
}InputMouse;

/*------------------------------ Input ----------------------------*/
typedef struct BKP_Input 
{
	int keypressed;
	int Start,Cancel;

	InputKey Action0,Action1,Action2,Action3;
	InputKey Gachette0,Gachette1,Gachette2,Gachette3;
	InputKey left,right,up,down;
	InputMouse Mouse;

}BKP_Input;

/*------------------------------ Timer ----------------------------*/
struct Delays
{
	float d_idle,d_maxrun;
	float d_transit;
}Delays;

/*------------------------------ Timer ----------------------------*/
struct Timer
{
	float t_timer;
	float a_timer;
}Timer;

/*------------------------------ AimgList----------------------------*/
typedef struct Aimg
{
	BKP_Position pos;
	int agent,type;
	int RotatecenterisNull;
	int sprite_w,sprite_h;   //sprite sheet size
	float w,h;				//size to draw
	double angle;
	int sppl; //sprite per line for current animation
	int sprite_col,sprite_row;
	int visible;
	int alpha;
	int text;
	float coins;
	char sz[256];
	SDL_Color ttc;
	
	
	SDL_Texture * sprite_sheet;
	SDL_Point center;

	struct Aimg * left;
	struct Aimg * right;
}Aimg;

/*------------------------------ Agent ----------------------------*/
typedef struct Agent
{
	int agent,type,name;
	int state,state_;
	int sprite_col,sprite_row;
	int sprite_w,sprite_h;   //sprite size in file
	int sppl; //sprite per line for current animation
	int id;
	int text;
	int score;

	int visible;
	int plan;
	int dir;

	float w,h;
	float coins;
	float PV;

	float ttl_time,ttl_delay;
	float anim_time,anim_delay,anim_delay_;
	float stepx,stepy;

	struct Delays delays;
	struct Timer timers[8];
	int flags[16];
	float fflags[16];
	
	struct Agent * left;
	struct Agent * right;

	void *ptr0,*ptr1,*ptr2,*ptr3;
	int FXchn[8];

	SDL_Texture * sprite_sheet;
	Aimg * aimg;
	BKP_Position pos,fx;
	BKP_AABB AABB;
	BKP_Input * input;
	BKP_BASIC_HITBOX hb;

}Agent;

/*------------------------------ Agent List----------------------------*/
typedef struct LAgent
{
	int size;
	Agent* first;
	Agent* last;
}LAgent;

/*------------------------------ AimgList----------------------------*/
typedef struct LAimg
{
	int size;
	Aimg * first;
	Aimg * last;
}LAimg;

/*------------------------------ Hud----------------------------*/
typedef struct BKP_Hud
{
	int sprite_x,sprite_y,sprite_w,sprite_h;
	float w,h;

	SDL_Texture * sprite_sheet;
	BKP_Position pos;
	Agent * obj1,
		  * obj2,
		  * obj3,
		  * obj4,
		  * obj0,
		  * shield;

}BKP_Hud;
/*------------------------------ ----------------------------*/
/*------------------------------ Global Texture ----------------------------*/
typedef struct BKP_GlobalTexture
{
	SDL_Texture * missile,
				* silo,
				* fury,
				* ship,
				* bg_play,
				* player,
				* logo ,
				* bazar;
}BKP_GlobalTexture;

/*------------------------------ ----------------------------*/
typedef struct BKP_GlobalFX
{
	Mix_Chunk * lazer,
				* fury,
				* gatlin,
				* explosion,
				* touched,
				* shield,
				* expand,
				* brr,
				* ready,
				* achat,
				* alert,
				* menu;
}BKP_GlobalFX;
/*------------------------------ ----------------------------*/
/*------------------------------ ----------------------------*/
/*------------------------------ ----------------------------*/
/*------------------------------ ----------------------------*/
#endif


