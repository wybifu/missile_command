#include <stdlib.h>
#include <stdio.h>

#include "include/blukpast.h"
#include "include/macro.h"
#include "include/memory.h"

/*Copyright*/
/*
 * AUTHOR: lilington
 * addr	: lilington@yahoo.fr
 *
 */

/**************************************************************************
*	Defines & Maro
**************************************************************************/

/**************************************************************************
*	Structs, Enum, Unio and Typesdef
**************************************************************************/

/**************************************************************************
*	Globals
**************************************************************************/

BKP_Screen * Screen = NULL;
BKP_Hud * Hud = NULL;
BKP_GlobalTexture * BKP_GT = NULL;
BKP_GlobalFX * BKP_FX = NULL;
LAgent * BKP_Players = NULL;
LAgent * BKP_NPC = NULL;
LAimg * BKP_AimgPlan[BKP_MAX_PLAN + 2];

/***************************************************************************
*	Prototypes
**************************************************************************/

static void SDL_init(int f,const char * title);
static void SDL_clean();
static void init_LAimg();
static void init_hud(const char *path);

/**************************************************************************
*	Main
**************************************************************************/



/**************************************************************************
*	Implementations
**************************************************************************/

int bkp_init(int W,int H,int fullscreen,const char * appname)
{
	ALLOC(BKP_Screen,Screen,1);
	Screen->defw = W;
	Screen->defh = H;
	Screen->w = W;
	Screen->h = H;
	Screen->ww = 1;
	Screen->hh = 1;
	Screen->touchscreen = 0;
	Screen->window = NULL;
	Screen->renderer = NULL;

	Screen->vp.pos.x = 0;
	Screen->vp.pos.y = 0;

	Screen->display_index = 0;
	Screen->ndisplay = 1;
	Screen->RectDisplay = NULL;
	Screen->bg = NULL;

	SDL_init(fullscreen,appname);

	ALLOC(LAgent,BKP_Players,1);
	BKP_Players->first = NULL;
	BKP_Players->last = NULL;
	BKP_Players->size = 0;

	ALLOC(LAgent,BKP_NPC,1);
	BKP_NPC->first = NULL;
	BKP_NPC->last = NULL;
	BKP_NPC->size = 0;

	ALLOC(BKP_Hud, Hud,1);
	init_hud("data/graphics/hud.png");

	init_LAimg();

	ALLOC(BKP_GlobalTexture, BKP_GT,1);
	BKP_GT->missile = NULL;
	BKP_GT->silo = NULL;
	BKP_GT->fury = NULL;
	BKP_GT->bg_play = NULL;
	BKP_GT->ship = NULL;
	BKP_GT->bazar = NULL;

	ALLOC(BKP_GlobalFX, BKP_FX,1);
	BKP_FX->lazer = NULL;
	BKP_FX->fury = NULL;
	BKP_FX->gatlin = NULL;
	BKP_FX->explosion = NULL;
	BKP_FX->touched = NULL,
	BKP_FX->shield = NULL,
	BKP_FX->expand = NULL,
	BKP_FX->menu = NULL;

	return 0;
}

/*--------------------------------------------------------------------------------*/
void bkp_shutdown()
{
	int i;

	bkp_MemoryClean(BKP_MemOBJ_ALL);
	free(BKP_Players);
	free(BKP_NPC);

	free(BKP_GT);

	SDL_DestroyTexture(Hud->sprite_sheet);
	free(Hud);

	for(i = 0; i < BKP_MAX_PLAN + 2; ++i)
	{
		free(BKP_AimgPlan[i]);
	}

	SDL_clean();
	free(Screen->RectDisplay);
	free(Screen);
//should free NPC and PLAYERS
	return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

/************************* Static functions ***************************************/

static void SDL_init(int f,const char * title)
{
	SDL_DisplayMode current;
	int i;

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		fprintf(stderr,"Could not initialize SDL: %s\n",SDL_GetError());
		BKP_DIE("Exiting ...\n",1);
	}

	Screen->ndisplay = SDL_GetNumVideoDisplays();
	ALLOC(SDL_Rect,Screen->RectDisplay,Screen->ndisplay);

	// display selection, by default selecting 0
	if(Screen->ndisplay <= 1 || Screen->display_index < 0 || Screen->display_index >= Screen->ndisplay)
		Screen->display_index = 0;

	for(i = 0; i < Screen->ndisplay; ++i)
	{
		int zero = SDL_GetCurrentDisplayMode(i,&current);
		SDL_GetDisplayBounds(i,&Screen->RectDisplay[i]);

		if(zero != 0)
			SDL_Log("Could not get Display mode for video display #%d: %s",i,SDL_GetError());
		else
			SDL_Log("Display #%d: %dx%dpx @%dhz\n",i,current.w,current.h,current.refresh_rate);
	}

	Screen->w = Screen->RectDisplay[Screen->display_index].w;
	Screen->h = Screen->RectDisplay[Screen->display_index].h;
	Screen->ww =  (float)Screen->w /(float)Screen->defw; // image ratio w  a 64 width tile in 1920 will be 34 in 1024
	Screen->hh = (float)Screen->h  /(float)Screen->defh ; // image ration h

	if(f == BKP_TRUE)
	{
		Screen->window = SDL_CreateWindow(title,
				SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
				Screen->w,Screen->h,SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		Screen->window = SDL_CreateWindow(title,
				SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
				Screen->w,Screen->h,SDL_WINDOW_SHOWN);
	}

	SDL_SetWindowPosition(Screen->window,Screen->RectDisplay[Screen->display_index].x,Screen->RectDisplay[Screen->display_index].y);
	Screen->renderer =  SDL_CreateRenderer(Screen->window,-1,SDL_RENDERER_PRESENTVSYNC);
	if(Screen->window == NULL || Screen->renderer == NULL)
	{
		fprintf(stderr,"Unable to initialize screen %d x %d :%s\n",Screen->w,Screen->h,SDL_GetError());
		BKP_DIE("Exiting ...\n",1);
	}

	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags) & imgFlags))
	{
		fprintf(stderr,"SDL_image not initialized s:%s\n",SDL_GetError());
		BKP_DIE("Exiting ...\n",1);
	}

	SDL_ShowCursor(SDL_DISABLE);

	if (TTF_Init() < 0)
	{
		printf("Impossible d'initialiser SDL TTF: %s\n", TTF_GetError());
		BKP_DIE("Exiting ...\n",1);
	}

	int flags = MIX_INIT_OGG;
	int initted = Mix_Init(flags);
	if ((initted & flags) != flags)
	{
		printf("Mix_Init: Failed to init SDL_Mixer\n");
		printf("Mix_Init: %s\n", Mix_GetError());
		MSG_EXIT("Exiting ",1);
	}

	/* Open 44.1KHz, signed 16bit, system byte order,
	   stereo audio, using 1024 byte chunks (voir la doc pour plus d'infos) */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		MSG_EXIT("Exiting ",1);
	}

	// Définit le nombre de pistes audio (channels) à mixer
	Mix_AllocateChannels(63);

	return;
}

/*--------------------------------------------------------------------------------*/
static void SDL_clean()
{

	SDL_DestroyRenderer(Screen->renderer);
	SDL_DestroyWindow(Screen->window);
	/*
	if(joystick != NULL)
	{
		//	SDL_JoystickClose(joystick);
		SDL_GameControllerClose( pad );
	}
	*/
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();

	return;
}

/*--------------------------------------------------------------------------------*/
SDL_Texture *bkp_loadImage(const char *path)
{
	SDL_Surface *image = NULL;
	SDL_Texture	*texture = NULL;
	image = IMG_Load(path);
	if(image != NULL)
	{
		texture = SDL_CreateTextureFromSurface(Screen->renderer,image);
		SDL_FreeSurface(image);
		image = NULL;
	}
	else
		fprintf(stderr,"Unable to load image '%s' : %s\n",path,SDL_GetError());

	return texture;
}

/*--------------------------------------------------------------------------------*/
void bkp_updateAimg(Agent *a)
{
	Aimg *ai = a->aimg;
	ai->visible = a->visible;
	ai->pos.x = a->pos.x;
	ai->pos.y = a->pos.y;
	ai->w = a->w;
	ai->h = a->h;
	ai->sprite_col = a->sprite_col;
	ai->sprite_row = a->sprite_row;
	ai->coins = a->coins;

	return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

/***************** Static functions ******************/

static void init_LAimg()
{
	int i;
	for(i = 0; i < BKP_MAX_PLAN + 2 ; ++i)
	{
		ALLOC(LAimg,BKP_AimgPlan[i],1);
		BKP_AimgPlan[i]->first = BKP_AimgPlan[i]->last = NULL;
	}

	return;
}

/*--------------------------------------------------------------------------------*/
static void init_hud(const char *path)
{

	Hud->sprite_x = 0;
	Hud->sprite_y = 0;
	Hud->sprite_w = 1024;
	Hud->sprite_h = 256;
	Hud->w = Screen->w;
	Hud->h = 192* Screen->hh;
	Hud->pos.y = Screen->h - (192* Screen->hh);
	Hud->pos.x = 0;

	Hud->sprite_sheet = bkp_loadImage(path);

	return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
