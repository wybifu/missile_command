#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*Copyright*/
/*
 * AUTHOR: lilington
 * addr	: lilington@yahoo.fr
 *
 */

#include "include/helper.h"

/**************************************************************************
*	Defines & Maro
**************************************************************************/

/**************************************************************************
*	Structs, Enum, Unio and Typesdef	
**************************************************************************/

/**************************************************************************
*	Globals	
**************************************************************************/

/*************************************************************************** *	Prototypes 	
**************************************************************************/

/**************************************************************************
*	Main	
**************************************************************************/



/**************************************************************************
*	Implementations	
**************************************************************************/

Agent * create_object(float x,float y,int agent,int type,float w,float h)
{
	Agent * a = bkp_Agentnew(agent);

	a->state  = BKP_STATE_IDLE;
	a->type = type;
	a->state_ = BKP_STATE_NONE;
	a->plan = 1;
	a->visible = BKP_TRUE;

	a->sprite_w = a->sprite_h = 256 ;
	if(w < 1)
		a->w =  256 * Screen->ww;
	else
		a->w = w * Screen->ww;

	if(h < 1)
		a->h = 256 * Screen->hh;
	else
		a->h = h * Screen->hh;

	a->pos.x  =  x;
	a->pos.y  =  y;
	a->sppl = 4;
	a->anim_time = SDL_GetTicks();
	a->ttl_time = SDL_GetTicks();
	a->ttl_delay = -1;
	a->PV = 90;
	a->coins = 1000;
	a->text = BKP_FALSE;
	a->name = OBJ_NAME_NONE;

	a->delays.d_idle = 192;
	a->delays.d_maxrun = 32;
	a->delays.d_transit = BKP_PLAYER_TRANSITION_DELAY;

	a->anim_delay = a->delays.d_idle;
	a->sprite_row = 0;
	a->sprite_col = 0;
	a->dir = BKP_RIGHT;
	a->ptr0 = a->ptr1 = a->ptr2 = a->ptr3 = NULL;
	int i;
	for(i = 0;i<16;++i)
	{
		a->flags[i] = 0;
		a->fflags[i] = 0.0;
	}

	return a;
}

/*--------------------------------------------------------------------------------*/
void create_silo()
{
	int x_init = 480 * Screen->ww;
	int y_init = Hud->pos.y - 100 * Screen->hh;
	int i;

	BKP_Players->first->flags[1] = y_init;

	Agent *a = NULL;

	for(i = 0; i < 3 ;++i)
	{
		a = create_object(x_init + ( i * x_init) - (128 * Screen->ww/2), y_init,BKP_AGENT_NPC,OBJ_TYPE_SILO,128 * Screen->ww,128 * Screen->hh);

		a->name = OBJ_NAME_BUYSILO;
		a->sprite_sheet = BKP_GT->silo ;
		a->sppl = 4;
		a->timers[0].t_timer = SDL_GetTicks();
		a->timers[0].a_timer = 300;
//		a->timers[0].a_timer = 8000;

		if(i == 0)
		{
			BKP_Players->first->flags[0] = i;  //selected
			BKP_Players->first->ptr0 = a;
			BKP_Players->first->ptr1 = a;
			a->flags[0] = 1;
			a->sprite_row = 1;
			Agent * x  = create_object(128 * Screen->ww ,Hud->pos.y + 10 * Screen->hh,BKP_AGENT_NPC,OBJ_TYPE_DECOR_SILO,250 * Screen->ww ,250 * Screen->hh);
			//		fprintf(stderr,"DECOR_SILO %p\n",(Agent*)x);
			x->sprite_sheet = BKP_GT->silo ;
			x->flags[0] = -1;
			x->sprite_row = 0;
			x->plan = BKP_MAX_PLAN;
			a->ptr0 = (void *) x;
			bkp_Agentpush(x);
		}
		else if(i == 1)
		{
			BKP_Players->first->ptr2 = a;
		}
		else if(i == 2)
		{
			BKP_Players->first->ptr3 = a;
		}

		bkp_Agentpush(a);
	}
	return;
}

/*--------------------------------------------------------------------------------*/
void create_generatorBomb()
{
	Agent * a = create_object(0,-20,BKP_AGENT_NPC,OBJ_TYPE_BOMB_GENERATOR,0,0);

	a->name = OBJ_NAME_BUYSILO;
	a->sprite_sheet = NULL ;
	a->sppl = 4;
	a->timers[0].t_timer = SDL_GetTicks();
	a->timers[0].a_timer = 5000; //wait for a while
	a->timers[1].t_timer = SDL_GetTicks();
	a->timers[1].a_timer = 2000; //wait for a while
	a->timers[7].a_timer = 5000;
	a->flags[0] = -1;
	a->fflags[3] = 0.1;
	a->flags[1] = -1;
	a->flags[2] = a->flags[1];
	a->state = BKP_STATE_IDLE;

	bkp_Agentpush(a);
	return;
}

/*--------------------------------------------------------------------------------*/
void epochtodate(char buf[],int N,int t)
{
	time_t now = t;
	struct tm  ts;

	ts = *localtime(&now);
	strftime(buf, N, "%Y-%m-%d %H:%M:%S", &ts);
	return ;
}
/*--------------------------------------------------------------------------------*/
int getepoch()
{
	time_t     now;

	time(&now);

	return now;
}
/*--------------------------------------------------------------------------------*/

/******************** static functions ********************************/

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

