#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "include/blukpast.h"
#include "include/game.h"
#include "include/macro.h"

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

/***************************************************************************
*	Prototypes
**************************************************************************/

static void pwdMe(const char *arg);
static void load_asset();
static void release_asset();
static void initPlayer(Agent *a);

/**************************************************************************
*	Main
**************************************************************************/

//------------- Main fucntion ---------------
int main(int argc, char **argv)
{

	pwdMe(argv[0]);

	srand((unsigned)time(NULL));
	if(argc > 1 && argv[1][0] == 'f')
		bkp_init(BKP_DEFAULT_WIDTH,BKP_DEFAULT_HEIGHT,BKP_FALSE,"my application 0.1");
	else
		bkp_init(BKP_DEFAULT_WIDTH,BKP_DEFAULT_HEIGHT,BKP_TRUE,"my application 0.1");
	
/*
	if(argc > 1 && argv[1][0] == 'f')
		bkp_init(BKP_DEFAULT_WIDTH,BKP_DEFAULT_HEIGHT,BKP_TRUE,"my application 0.1");
	else
		bkp_init(BKP_DEFAULT_WIDTH,BKP_DEFAULT_HEIGHT,BKP_FALSE,"my application 0.1");

	 * should be menu here
	 */


	load_asset();
	Agent * p = bkp_Agentnew(BKP_AGENT_PLAYER);
	initPlayer(p);
	bkp_AgentpushPlayer(p);

	Screen->bg = BKP_GT->bg_play;
	Screen->logo = BKP_GT->logo;

	logo();

	while(1)
	{
		int val = menu();
		if(val == 0)
			Play();
		else if (val == 1)
			Hiscore();
		else
			break;
	}

	outlogo();

	bkp_Agentdestroy(p);
	release_asset();
	/*
	 * end of menu here
	 */
	bkp_shutdown();
	return EXIT_SUCCESS;
}


/**************************************************************************
*	Implementations
**************************************************************************/

/*--------------------------------------------------------------------------------*/

static void pwdMe(const char * arg)
{
	int j;
	int i = strlen(arg)-1;
	char path[256] = {0};

	for(; i>0; --i)
	{
#ifdef _WIN32
		if(arg[i]=='\\')
			break;
#else
		if(arg[i]=='/')
			break;
#endif
	}

	if(i<1)
		return;

	for(j=0; j<=i; ++j)
		path[j]=arg[j];

	fprintf(stdout,"Moving to directory `%s`\n",path);
	if(chdir(path)<0)
	{
		fprintf(stderr,"Error on '%s'\n",path);
		exit(EXIT_FAILURE);
	}
	return;
}

static void load_asset()
{
	BKP_GT->silo = bkp_loadImage("data/graphics/silo_1.png");
	BKP_GT->missile = bkp_loadImage("data/graphics/missile.png");
	BKP_GT->bazar = bkp_loadImage("data/graphics/bazar.png");
	BKP_GT->bg_play = bkp_loadImage("data/graphics/bg.png");
	BKP_GT->ship= bkp_loadImage("data/graphics/ship.png");
	BKP_GT->player = bkp_loadImage("data/graphics/aim.png");
	BKP_GT->logo = bkp_loadImage("data/graphics/logo.png");

	BKP_FX->gatlin = Mix_LoadWAV("data/audio/gatlin.ogg");
	if(BKP_FX->gatlin== NULL)
		MSG_EXIT("Cannot load  gatlin.ogg",1);
	BKP_FX->lazer= Mix_LoadWAV("data/audio/lazer.ogg");
	if(BKP_FX->lazer== NULL)
		MSG_EXIT("Cannot load  lazer.ogg",1);
	BKP_FX->fury= Mix_LoadWAV("data/audio/fury.ogg");
	if(BKP_FX->fury== NULL)
		MSG_EXIT("Cannot load  fury.ogg",1);
	BKP_FX->shield= Mix_LoadWAV("data/audio/shield.ogg");
	if(BKP_FX->shield== NULL)
		MSG_EXIT("Cannot load  shield.ogg",1);
	BKP_FX->expand= Mix_LoadWAV("data/audio/expand.ogg");
	if(BKP_FX->expand== NULL)
		MSG_EXIT("Cannot load  shield.expand",1);
	BKP_FX->touched= Mix_LoadWAV("data/audio/touched.ogg");
	if(BKP_FX->touched== NULL)
		MSG_EXIT("Cannot load  shield.touched",1);
	BKP_FX->explosion= Mix_LoadWAV("data/audio/explosion.ogg");
	if(BKP_FX->explosion== NULL)
		MSG_EXIT("Cannot load  explosion.ogg",1);
	BKP_FX->menu= Mix_LoadWAV("data/audio/menu.ogg");
	if(BKP_FX->menu== NULL)
		MSG_EXIT("Cannot load  menu.ogg",1);
	BKP_FX->brr= Mix_LoadWAV("data/audio/brr.ogg");
	if(BKP_FX->brr== NULL)
		MSG_EXIT("Cannot load  brr.ogg",1);
	BKP_FX->ready = Mix_LoadWAV("data/audio/ready.ogg");
	if(BKP_FX->ready == NULL)
		MSG_EXIT("Cannot load  ready.ogg",1);
	BKP_FX->achat= Mix_LoadWAV("data/audio/achat.ogg");
	if(BKP_FX->achat== NULL)
		MSG_EXIT("Cannot load  achat.ogg",1);
	BKP_FX->alert= Mix_LoadWAV("data/audio/alert.ogg");
	if(BKP_FX->alert== NULL)
		MSG_EXIT("Cannot load  alert.ogg",1);

}

/*--------------------------------------------------------------------------------*/
static void release_asset()
{
	if(BKP_GT->silo != NULL)
	{
		SDL_DestroyTexture(BKP_GT->silo);
		BKP_GT->silo = NULL;
	}
	else if(BKP_GT->missile != NULL)
	{
		SDL_DestroyTexture(BKP_GT->missile);
		BKP_GT->missile = NULL;
	}
	else if(BKP_GT->fury != NULL)
	{
		SDL_DestroyTexture(BKP_GT->fury);
		BKP_GT->fury = NULL;
	}
	else if(BKP_GT->bazar!= NULL)
	{
		SDL_DestroyTexture(BKP_GT->bazar);
		BKP_GT->bazar= NULL;
	}
	else if(BKP_GT->logo != NULL)
	{
		SDL_DestroyTexture(BKP_GT->logo);
		BKP_GT->logo= NULL;
	}

	return;
}

/*--------------------------------------------------------------------------------*/
static void initPlayer(Agent *a)
{
	a->sprite_sheet = BKP_GT->player;
	a->state  = BKP_STATE_IDLE;
	a->state_ = BKP_STATE_NONE;

	a->plan = 1;
	a->visible = BKP_TRUE;
	a->sprite_w = a->sprite_h = 256 ;
	a->pos.x  =  Screen->w / 2;
	a->pos.y  =  Screen->h / 2;
	a->sppl = 4;
	a->anim_time = SDL_GetTicks();
	a->ttl_time = SDL_GetTicks();
	a->ttl_delay = -1;
	a->text = BKP_FALSE;
	a->name = OBJ_NAME_NONE;

	a->delays.d_idle = 192;
	a->delays.d_maxrun = 32;
	a->delays.d_transit = BKP_PLAYER_TRANSITION_DELAY;

	a->anim_delay = a->delays.d_idle;
	a->sprite_row = 0;
	a->sprite_col = 0;
	a->dir = BKP_RIGHT;
	a->w = 64 * Screen->ww;
	a->h = 64 * Screen->hh;
	a->ptr0 = a->ptr1 = a->ptr2 = a->ptr3 = NULL;

	int i;
	for(i = 0;i<16;++i)
	{
		a->flags[i] = 0;
		a->fflags[i] = 0.0;
	}

	return;
}

