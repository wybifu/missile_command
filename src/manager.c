#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*Copyright*/
/*
 * AUTHOR: lilington
 * addr	: lilington@yahoo.fr
 *
 */

#include "include/macro.h"
#include "include/agent.h"
#include "include/physics.h"
#include "include/manager.h"
#include "include/input.h"
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

int GAT_CHN = 5,
	SHD_CHN = 6;

/***************************************************************************
*	Prototypes
**************************************************************************/

static void check_click(Agent *a);
static void update_site(Agent * a,int obj_name);
static int getsite_row(Agent * a,int what);
static void generateBomb(Agent * a);
static int check_missile_colision(Agent * p);
static int check_missile_colision2(Agent * p);
static void kill_all_obj();
static void shoot_multi(Agent *a);
void create_leure(float fx,float fy,float x,float y);

/**************************************************************************
*	Main
**************************************************************************/


/**************************************************************************
*	Implementations
**************************************************************************/

void bkp_ManagePlayers()
{
	Agent *a = BKP_Players->first;

	for(; a != NULL ;a = a->right)
	{
		bkp_ManageInput(a);

		check_click(a);

		unsigned int ticks = SDL_GetTicks();
		if(ticks - a->anim_time >= a->anim_delay)
		{
			a->sprite_col = (a->sprite_col + 1) % a->sppl;
			a->anim_time = ticks;
		}

		bkp_updateAimg(a);
		if(a->pos.y >  a->flags[1] || (a->input->Mouse.x < 28 * Screen->ww && a->input->Mouse.y < 28 * Screen->hh))
		{
			a->aimg->sprite_row = 1;
			a->aimg->w = 64 * Screen->ww;
			a->aimg->h = 64 * Screen->hh;
			if(a->pos.x > Hud->obj0->pos.x && a->pos.x < Hud->obj0->pos.x + Hud->obj0->w &&
			   a->pos.y > Hud->obj0->pos.y && a->pos.y < Hud->obj0->pos.y + Hud->obj0->h)
			{
				Hud->obj4->pos.x = Hud->obj0->pos.x;
				Hud->obj4->pos.y = Hud->obj0->pos.y;
				Hud->obj4->visible = BKP_TRUE;
			}
			else if(a->pos.x > Hud->obj1->pos.x && a->pos.x < Hud->obj1->pos.x + Hud->obj1->w &&
			   a->pos.y > Hud->obj1->pos.y && a->pos.y < Hud->obj1->pos.y + Hud->obj1->h)
			{
				Hud->obj4->pos.x = Hud->obj1->pos.x;
				Hud->obj4->pos.y = Hud->obj1->pos.y;
				Hud->obj4->visible = BKP_TRUE;
			}
			else if(a->pos.x > Hud->obj2->pos.x && a->pos.x < Hud->obj2->pos.x + Hud->obj2->w &&
			   a->pos.y > Hud->obj2->pos.y && a->pos.y < Hud->obj2->pos.y + Hud->obj2->h)
			{
				Hud->obj4->pos.x = Hud->obj2->pos.x;
				Hud->obj4->pos.y = Hud->obj2->pos.y;
				Hud->obj4->visible = BKP_TRUE;
			}
			else if(a->pos.x > Hud->obj3->pos.x && a->pos.x < Hud->obj3->pos.x + Hud->obj3->w &&
			   a->pos.y > Hud->obj3->pos.y && a->pos.y < Hud->obj3->pos.y + Hud->obj3->h)
			{
				Hud->obj4->pos.x = Hud->obj3->pos.x;
				Hud->obj4->pos.y = Hud->obj1->pos.y;
				Hud->obj4->visible = BKP_TRUE;
			}
			else
			{
				Hud->obj4->visible = BKP_FALSE;
			}
		}

	}

	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_ManageInput(Agent * a)
{
	BKP_Input * p = a->input;

	bkp_InputCapture(p);
	a->pos.x = p->Mouse.x;
	a->pos.y = p->Mouse.y;

}

/*--------------------------------------------------------------------------------*/
void bkp_ManageNPC()
{
	Agent *a = BKP_NPC->first;

	for(; a != NULL ;a = a->right)
	{

		unsigned int ticks = SDL_GetTicks();
		if(a->ttl_delay > -1 && ticks - a->ttl_time >= a->ttl_delay)
		{
			a->state = BKP_STATE_DESTROY;
		}

		if(a->state == BKP_STATE_DESTROY)
		{
			Agent *x = a->left;
			bkp_Agentdestroy(a);
			a = x;
			if(a == NULL)
				break;
			continue;
		}

		if(ticks - a->anim_time >= a->anim_delay)
		{
			a->sprite_col = (a->sprite_col + 1) % a->sppl;
			a->anim_time = ticks;
		}


		if(a->type == OBJ_TYPE_SILO)
		{
			if(a->state == BKP_STATE_IDLE)
			{
				if(a->name == OBJ_NAME_BUYSILO)
				{
					if(a->state_ == BKP_STATE_OPENFIRE)
					{

						a->state_ = BKP_STATE_IDLE;
						Agent *x = create_object(a->pos.x + 30 * Screen->ww,a->pos.y,BKP_AGENT_NPC,OBJ_TYPE_MISSILE,140 * Screen->ww,140 * Screen->hh);
						x->delays.d_idle = 20;
						x->delays.d_maxrun = 10;
						x->sprite_sheet = BKP_GT->missile;
						x->fx.x = BKP_Players->first->input->Mouse.x;
						x->fx.y = BKP_Players->first->input->Mouse.y;
						x->sprite_row = 4;
						x->w = 128 * Screen->ww;
						x->h = 128 * Screen->hh;
						x->stepx = 12;
						x->stepy = 12;
						bkp_Agentpush(x);
						bkp_updateAimg(x);
						a->flags[3] += 1;

						x->state = BKP_STATE_MOTION;
						x->fflags[0] = M_ABS_MAX(x->fx.x - x->pos.x,x->fx.y - x->pos.y);
						x->fflags[1] = (float)(x->fx.x - x->pos.x) / x->fflags[0];
						x->fflags[2] = (float)(x->fx.y - x->pos.y) / x->fflags[0];
						if(x->fx.x <= x->pos.x)
						{
							x->fflags[3] = -(90 -(180 * atan((float)(x->fx.y - x->pos.y)/(float)(x->fx.x - x->pos.x)) / 3.1415));
							x->fflags[4] = 8 * -.9;
						}
						else
						{
							x->fflags[3] = (90 +(180 * atan((float)(x->fx.y - x->pos.y)/(float)(x->fx.x - x->pos.x)) / 3.1415));
							x->fflags[4] = 8 * .9;
						}
						x->aimg->angle = x->fflags[3];
						x->fflags[0] = a->fflags[3];
						a->state = BKP_STATE_RELOADING;
						a->timers[0].t_timer = SDL_GetTicks();

						Mix_PlayChannel(-1,BKP_FX->lazer,0);

						x = (Agent *) a->ptr0;
						x->flags[0] = 0;
						Agent * ag = create_object(x->pos.x,x->pos.y,BKP_AGENT_NPC,OBJ_TYPE_DECOR,250 * Screen->ww,250 * Screen->hh);
						ag->name = OBJ_NAME_DECOR;
						ag->sprite_sheet = BKP_GT->bazar;
						ag->sprite_row = 0;
						ag->sprite_col = 0;
						ag->plan = BKP_MAX_PLAN + 1;
						ag->flags[0] = ag->h;
						bkp_Agentpush(ag);
						bkp_updateAimg(ag);
						a->ptr1 = (void *)ag;
					}
				}
				else if(a->name == OBJ_NAME_BUYFURY)
				{
					BKP_Players->first->input->Mouse.l_click.pressed = 0;
					if(a->state_ == BKP_STATE_OPENFIRE)
					{
						if(a->flags[3] < 25)
						{
							Mix_PlayChannel(-1,BKP_FX->fury,0);
							a->state_ = BKP_STATE_IDLE;
							Agent *x = create_object(a->pos.x + 25 * Screen->ww,a->pos.y,BKP_AGENT_NPC,OBJ_TYPE_MISSILE,140 * Screen->ww,140 * Screen->hh);
							x->delays.d_idle = 20;
							x->delays.d_maxrun = 10;
							x->sprite_sheet = BKP_GT->missile;
							x->w = 128 * Screen->ww;
							x->h = 128 * Screen->hh;
							x->fx.x = a->fx.x;
							x->fx.y = a->fx.y;
							x->stepx = 4;
							x->stepy = 4;
							x->fflags[4] = 2.9;
							x->state = BKP_STATE_STARTJUMP;
							x->flags[0] = a->pos.y - x->h / 2;
							x->flags[6] = 1;
							x->flags[5] = 1;
							bkp_Agentpush(x);
							bkp_updateAimg(x);
							a->flags[3] += 1;
						}
						else
						{
							a->state_ = BKP_STATE_IDLE;
							a->state = BKP_STATE_RELOADING;
							a->timers[0].t_timer = SDL_GetTicks();
							Agent * x = (Agent *) a->ptr0;
							x->flags[0] = 0;
							a->flags[3] = 0;
							Agent * ag = create_object(x->pos.x,x->pos.y,BKP_AGENT_NPC,OBJ_TYPE_DECOR,250 * Screen->ww,250 * Screen->hh);
							ag->name = OBJ_NAME_DECOR;
							ag->sprite_sheet = BKP_GT->bazar;
							ag->sprite_row = 0;
							ag->sprite_col = 0;
							ag->plan = BKP_MAX_PLAN + 1;
							ag->flags[0] = ag->h;
							bkp_Agentpush(ag);
							bkp_updateAimg(ag);
							a->ptr1 = (void *)ag;
						}
					}
				}
				else if(a->name == OBJ_NAME_BUYGATLING)
				{
					if(a->state_ == BKP_STATE_OPENFIRE)
					{
						a->state_ = BKP_STATE_FIRE;
						a->timers[1].t_timer = SDL_GetTicks();
						a->timers[1].a_timer = 6000;
						a->timers[2].t_timer = SDL_GetTicks();
						a->timers[2].a_timer = 50;
					}
					else if(a->state_ == BKP_STATE_FIRE)
					{
						if(ticks - a->timers[2].t_timer >= a->timers[2].a_timer)
						{
							if(a->flags[5] == 0)
							{
								a->flags[5] = 1;
								Mix_PlayChannel(GAT_CHN,BKP_FX->gatlin,-1);
							}
							a->timers[2].t_timer = SDL_GetTicks();
							Agent *x = create_object(a->pos.x + 25 * Screen->ww,a->pos.y,BKP_AGENT_NPC,OBJ_TYPE_MISSILE,140 * Screen->ww,140 * Screen->hh);
							x->delays.d_idle = 20;
							x->delays.d_maxrun = 10;
							x->anim_delay = 2;
							x->sprite_sheet = BKP_GT->missile;
							x->fx.x = BKP_Players->first->input->Mouse.x;
							x->fx.y = BKP_Players->first->input->Mouse.y;
							x->w = 92 * Screen->ww;
							x->h = 92 * Screen->hh;
							x->stepx = 4;
							x->stepy = 4;
							bkp_Agentpush(x);
							bkp_updateAimg(x);
							a->flags[3] += 1;

							x->state = BKP_STATE_ROTATE_STATIC;
							x->fflags[0] = M_ABS_MAX(x->fx.x - x->pos.x,x->fx.y - x->pos.y);
							x->fflags[1] = (float)(x->fx.x - x->pos.x) / x->fflags[0];
							x->fflags[2] = (float)(x->fx.y - x->pos.y) / x->fflags[0];
							x->flags[6] = 1;
							if(x->fx.x <= x->pos.x)
							{
								x->fflags[3] = -(90 -(180 * atan((float)(x->fx.y - x->pos.y)/(float)(x->fx.x - x->pos.x)) / 3.1415));
								x->fflags[4] = 8 * -.9;
							}
							else
							{
								x->fflags[3] = (90 +(180 * atan((float)(x->fx.y - x->pos.y)/(float)(x->fx.x - x->pos.x)) / 3.1415));
								x->fflags[4] = 8 * .9;
							}
						}
						if(ticks - a->timers[1].t_timer >= a->timers[1].a_timer)
						{
							a->flags[5] = 0;
							Mix_HaltChannel(GAT_CHN);
							a->state_ = BKP_STATE_IDLE;
							a->state = BKP_STATE_RELOADING;
							a->timers[0].t_timer = SDL_GetTicks();
							Agent * x = (Agent *) a->ptr0;
							x->flags[0] = 0;
							a->flags[3] = 0;
							Agent * ag = create_object(x->pos.x,x->pos.y,BKP_AGENT_NPC,OBJ_TYPE_DECOR,250 * Screen->ww,250 * Screen->hh);
							ag->name = OBJ_NAME_DECOR;
							ag->sprite_sheet = BKP_GT->bazar;
							ag->sprite_row = 0;
							ag->sprite_col = 0;
							ag->plan = BKP_MAX_PLAN + 1;
							ag->flags[0] = ag->h;
							bkp_Agentpush(ag);
							bkp_updateAimg(ag);
							a->ptr1 = (void *)ag;
						}
					}
				}
				else if(a->name == OBJ_NAME_BUYSHILD)
				{
					if(a->state_ == BKP_STATE_OPENFIRE)
					{
						Mix_PlayChannel(SHD_CHN,BKP_FX->shield,-1);
						a->timers[2].t_timer = SDL_GetTicks();
						Agent *x = create_object(a->pos.x,a->pos.y,BKP_AGENT_NPC,OBJ_TYPE_SHIELD,140 * Screen->ww,140 * Screen->hh);
						x->delays.d_idle = 128;
						x->delays.d_maxrun = 128;
						x->anim_delay = 128;
						x->sprite_sheet = BKP_GT->missile;
						x->fx.x = BKP_Players->first->input->Mouse.x;
						x->fx.y = BKP_Players->first->input->Mouse.y;
						x->sprite_row = 3;
						x->w = 128 * Screen->ww;
						x->h = 128 * Screen->hh;
						x->stepx = 4;
						x->stepy = 4;
						bkp_Agentpush(x);
						bkp_updateAimg(x);
						a->flags[3] += 1;

						x->state = BKP_STATE_STARTJUMP;
						x->fflags[0] = M_ABS_MAX(x->fx.x - x->pos.x,x->fx.y - x->pos.y);
						x->fflags[1] = (float)(x->fx.x - x->pos.x) / x->fflags[0];
						x->fflags[2] = (float)(x->fx.y - x->pos.y) / x->fflags[0];
						if(x->fx.x <= x->pos.x)
						{
							x->fflags[3] = -(90 -(180 * atan((float)(x->fx.y - x->pos.y)/(float)(x->fx.x - x->pos.x)) / 3.1415));
							x->fflags[4] = 8 * -.9;
						}
						else
						{
							x->fflags[3] = (90 +(180 * atan((float)(x->fx.y - x->pos.y)/(float)(x->fx.x - x->pos.x)) / 3.1415));
							x->fflags[4] = 8 * .9;
						}
						x->aimg->angle = x->fflags[3];
						x->fflags[0] = a->fflags[3];

						a->state_ = BKP_STATE_IDLE;
						a->state = BKP_STATE_RELOADING;
						a->timers[0].t_timer = SDL_GetTicks();
						x = (Agent *) a->ptr0;
						x->flags[0] = 0;
						a->flags[3] = 0;
						Agent * ag = create_object(x->pos.x,x->pos.y,BKP_AGENT_NPC,OBJ_TYPE_DECOR,250 * Screen->ww,250 * Screen->hh);
						ag->name = OBJ_NAME_DECOR;
						ag->sprite_sheet = BKP_GT->bazar;
						ag->sprite_row = 0;
						ag->sprite_col = 0;
						ag->plan = BKP_MAX_PLAN + 1;
						ag->flags[0] = ag->h;
						bkp_Agentpush(ag);
						bkp_updateAimg(ag);
						a->ptr1 = (void *)ag;
					}
				}
			}
			else if(a->state == BKP_STATE_RELOADING)
			{

				if(a->state_ == BKP_STATE_OPENFIRE)
					a->state_ = BKP_STATE_IDLE;

				((Agent *)(a->ptr1))->h = ((Agent *)(a->ptr1))->flags[0] -(((Agent *)(a->ptr1))->flags[0] * (ticks - a->timers[0].t_timer) * 100 / a->timers[0].a_timer / 100);

				if(ticks - a->timers[0].t_timer >= a->timers[0].a_timer)
				{
					a->state = BKP_STATE_IDLE;
					((Agent *)(a->ptr0))->flags[0] = -1;

					((Agent *)(a->ptr1))->state = BKP_STATE_DESTROY;
					a->ptr1 = NULL;
					if(a->name != OBJ_NAME_BUYSILO)
					{
						Mix_PlayChannel(-1,BKP_FX->ready,0);
						Agent * ag = create_object(a->pos.x,a->pos.y,BKP_AGENT_NPC,OBJ_TYPE_DECOR,128 * Screen->ww,128* Screen->hh);
						ag->name = OBJ_NAME_NOTICE;
						ag->sprite_sheet = BKP_GT->player;
						ag->sprite_row = 4;
						ag->sprite_col = 0;
						ag->plan = BKP_MAX_PLAN;
						bkp_Agentpush(ag);
						bkp_updateAimg(ag);
						ag->aimg->alpha = 255;
					}
				}
			}
		}
		else if(a->type == OBJ_TYPE_BOMB_GENERATOR)
		{
			generateBomb(a);
		}
		else if(a->type == OBJ_TYPE_MISSILE)
		{
			if(a->state == BKP_STATE_STARTJUMP)
			{
				a->pos.y -= a->stepy;
				if(a->pos.y <= a->flags[0])
				{
					a->state = BKP_STATE_ROTATE_STATIC;
					a->fflags[0] = M_ABS_MAX(a->fx.x - a->pos.x,a->fx.y - a->pos.y);
					a->fflags[1] = (float)(a->fx.x - a->pos.x) / a->fflags[0];
					a->fflags[2] = (float)(a->fx.y - a->pos.y) / a->fflags[0];
					if(a->fx.x <= a->pos.x)
					{
						a->fflags[3] = -(90 -(180 * atan((float)(a->fx.y - a->pos.y)/(float)(a->fx.x - a->pos.x)) / 3.1415));
						a->fflags[4] = -a->fflags[4];
					}
					else
					{
						a->fflags[3] = (90 +(180 * atan((float)(a->fx.y - a->pos.y)/(float)(a->fx.x - a->pos.x)) / 3.1415));
					}
				}
			}
			else if(a->state == BKP_STATE_ROTATE_STATIC)
			{
				a->aimg->angle += a->fflags[4];
				if(abs(a->aimg->angle) >= abs(a->fflags[3]))
					a->state = BKP_STATE_MOTION;

			}
			else if(a->state == BKP_STATE_MOTION)
			{
				a->pos.x += a->stepx * a->fflags[1];
				a->pos.y += a->stepy * a->fflags[2];
				if(a->flags[5] == 1)
				{
					if(a->pos.y <= a->fx.y)
					{
						shoot_multi(a);
					}
				}
				if(a->pos.x + a->w < 0 || a->pos.x > Screen->w  || a->pos.y + a->h < -20 )
				{
					a->state = BKP_STATE_DYING;
					a->ttl_time = SDL_GetTicks();
					a->ttl_delay = 1000;
				}
				if(check_missile_colision(a))
				{
					BKP_Players->first->coins += 100;
					BKP_Players->first->score += 10;
					if(a->flags[6] == 0)
					{
						a->state = BKP_STATE_DYING;
						a->ttl_time = SDL_GetTicks();
						a->ttl_delay = 1;
					}

				}
			}

		}
		else if(a->type == OBJ_TYPE_LEURE)
		{
			a->pos.x += a->stepx * a->fflags[1];
			a->pos.y += a->stepy * a->fflags[2];
			if(a->flags[5] == 1)
			{
				if(a->pos.y > a->fx.y)
				{
					a->state = BKP_STATE_DYING;
					a->ttl_time = SDL_GetTicks();
					a->ttl_delay = 1;
					Agent * ag = create_object(a->pos.x,a->pos.y ,BKP_AGENT_NPC,OBJ_TYPE_DECOR,64 * Screen->ww,64 * Screen->hh);
					ag->sprite_sheet = BKP_GT->bazar;
					ag->pos.y += ag->h + ag->h/3;
					ag->name = OBJ_NAME_WHITEBALL2;
					ag->flags[1] = 1;
					ag->sprite_row = 0;
					ag->sprite_col = 2;
					ag->plan = 2;
					ag->flags[0] = ag->h;
					bkp_Agentpush(ag);
					bkp_updateAimg(ag);
					ag->aimg->alpha = 255;
					Mix_PlayChannel(-1,BKP_FX->explosion,0);
				}
			}
			if(a->pos.y + a->h <= Hud->pos.y - 100 * Screen->hh && a->state != BKP_STATE_DYING)
			{
				a->state = BKP_STATE_DYING;
				a->ttl_time = SDL_GetTicks();
				a->ttl_delay = 1000;
			}
			if(check_missile_colision(a))
			{
				BKP_Players->first->coins += 100;
				BKP_Players->first->score += 10;
				if(a->flags[6] == 0)
				{
					a->state = BKP_STATE_DYING;
					a->ttl_time = SDL_GetTicks();
					a->ttl_delay = 1;
				}

			}
		}
		else if(a->type == OBJ_TYPE_SHIELD)
		{
			if(a->state == BKP_STATE_STARTJUMP)
			{
				if(a->pos.y > a->fx.y)
				{
					a->pos.x += 4 * a->fflags[1];
					a->pos.y += 4 * a->fflags[2];
				}
				else
				{
						a->aimg->angle = 0;
					a->pos.y = a->fx.y;
					a->sprite_row = 2;
					a->w += 10;
					if(a->w >= 900 * Screen->ww)
						a->w = 920 * Screen->ww;
					a->h += 10;
					if(a->h >= 256 * Screen->hh)
						a->h = 276 * Screen->hh;
					if (a->flags[6] == 0)
					{
						Mix_HaltChannel(SHD_CHN);
						Mix_PlayChannel(-1,BKP_FX->expand,0);
						a->flags[6] = 1;
					}

					if(a->w >= 900 * Screen->ww && a->h >= 256 * Screen->hh)
					{
						a->state = BKP_STATE_IDLE;
						//fprintf(stderr,"shield activate\n");
						a->flags[5] = 25;
					}
				}
			}
			else if(a->state == BKP_STATE_IDLE)
			{
				if(check_missile_colision2(a))
				{
					--a->flags[5];
					a->aimg->alpha = a->flags[5] * 255 / 25;
					BKP_Players->first->coins += 100;
					BKP_Players->first->score += 10;
					if(a->aimg->alpha < 92)
						a->aimg->alpha = 92;
					if(a->flags[5] < 1)
					{
						a->state = BKP_STATE_DYING;
						a->ttl_time = SDL_GetTicks();
						a->ttl_delay = 1;
					}
				}
			}

		}
		else if(a->type == OBJ_TYPE_BOMB)
		{
			a->pos.x += a->fflags[0] * a->fflags[1];
			a->pos.y += a->fflags[0] * a->fflags[2];
			if(a->pos.y + a->h >= a->fx.y && a->state != BKP_STATE_DYING)
			{
				a->state = BKP_STATE_DYING;
				a->ttl_time = SDL_GetTicks();
				a->ttl_delay = 1;
				BKP_Players->first->PV -= 1;
				Mix_HaltChannel(GAT_CHN);
				Mix_HaltChannel(SHD_CHN);
//				fprintf(stderr,"explosion of death %d for %p\n",(int)BKP_Players->first->PV,a);
				Hud->shield->aimg->alpha = BKP_Players->first->PV > 1 ? (int)((BKP_Players->first->PV  - 1)* 255 / 4) : 0;

				Agent * ag = create_object(a->pos.x,a->pos.y,BKP_AGENT_NPC,OBJ_TYPE_DECOR,64 * Screen->ww,64 * Screen->hh);
				ag->sprite_sheet = BKP_GT->bazar;
				ag->name = OBJ_NAME_WHITEBALL;
				ag->flags[2] = 0;
				ag->sprite_row = 0;
				ag->sprite_col = 2;
				ag->plan = 2;
				ag->flags[0] = ag->h;
				bkp_Agentpush(ag);
				bkp_updateAimg(ag);
				ag->aimg->alpha = 255;
				Mix_PlayChannel(-1,BKP_FX->touched,0);
				BKP_Players->first->flags[7] = 1;
			}
		}
		else if(a->type == OBJ_TYPE_DECOR)
		{
			if(a->name == OBJ_NAME_DOME)
			{
				a->sprite_col = 1;
			}
			else if(a->name == OBJ_NAME_WHITEBALL || a->name == OBJ_NAME_WHITEBALL2)
			{
				a->sprite_col = 2;
				if(a->name == OBJ_NAME_WHITEBALL && a->flags[2] == 0)
				{
					a->flags[2] = 1;
					kill_all_obj();
				}
				if(a->name == OBJ_NAME_WHITEBALL)
				{
					a->w += 96;
					a->h += 96;
					a->pos.x -= 48;
					a->pos.y -= 48;
					if(a->pos.x <= 0 && a->w > Screen->w)
					{
						a->aimg->alpha -= 8;
						if(a->aimg->alpha < 0)
						{
							a->aimg->alpha = 0;
							a->ttl_time = SDL_GetTicks();
							a->ttl_delay = 0;
						}
					}
				}
				else if(a->name == OBJ_NAME_WHITEBALL2 )
				{
					a->w += 8;
					a->h += 8;
					a->pos.x -= 4;
					a->pos.y -= 4;
					if(a->w > 92 * Screen->ww)
					{
						a->aimg->alpha -= 8;
						a->pos.x +=2;
						a->pos.y +=2;
						a->w -= 4;
						a->h -= 4;
						if(a->aimg->alpha < 0)
						{
							a->aimg->alpha = 0;
							a->ttl_time = SDL_GetTicks();
							a->ttl_delay = 0;
						}
					}
				}
			}
			else if(a->name == OBJ_NAME_NOTICE)
			{
				a->aimg->alpha -= 2;
				if(a->aimg->alpha < 0)
				{
					a->aimg->alpha = 0;
					a->ttl_time = SDL_GetTicks();
					a->ttl_delay = 0;
				}
			}
			else
			{
				if(a->name == OBJ_NAME_HIDEBUY)
				{
					if(a->fflags[0] <= BKP_Players->first->coins)
					{
						a->visible = BKP_FALSE;
					}
					else
					{
						a->visible = BKP_TRUE;
					}
				}
				a->sprite_col = 0;
			}
		}
		bkp_updateAimg(a);

	}

	return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

/************************** static functions ************************************/

static void check_click(Agent *a)
{
	if(a->input->Mouse.l_click.pressed > 0)
	{
		if(a->input->Mouse.y >= a->flags[1])
		{
			if( a->input->Mouse.y < Hud->pos.y)
			{
				Agent *x1 = a->ptr1,
					  *x2 = a->ptr2,
					  *x3 = a->ptr3;
				if(a->ptr0 != NULL)
					if (((Agent *)(((Agent *)(a->ptr0))->ptr1)) != NULL)
						((Agent *)(((Agent *)(a->ptr0))->ptr1))->visible = BKP_FALSE;


				if(a->input->Mouse.x > x1->pos.x && a->input->Mouse.x < x1->pos.x + x1->w)
				{
					((Agent *)(a->ptr0))->sprite_row -= 1;
					x1->sprite_row = getsite_row(x1,x1->name) + 1;
					Agent * y= ((Agent *)(a->ptr0))->ptr0;
					y->sprite_row = x1->sprite_row -1;
					x1->ptr0 = (void *) y;
					if(x1->ptr1 != NULL)
						((Agent *)(x1->ptr1))->visible = BKP_TRUE;

					a->ptr0 = (void *) x1;
					Mix_PlayChannel(-1,BKP_FX->menu,0);
				}
				else if(a->input->Mouse.x > x2->pos.x && a->input->Mouse.x < x2->pos.x + x2->w)
				{
					((Agent *)(a->ptr0))->sprite_row -= 1;
					x2->sprite_row = getsite_row(x2,x2->name) + 1;
					Agent * y= ((Agent *)(a->ptr0))->ptr0;
					y->sprite_row = x2->sprite_row -1;
					x2->ptr0 = (void *) y;
					if(x2->ptr1 != NULL)
						((Agent *)(x2->ptr1))->visible = BKP_TRUE;
					a->ptr0 = (void *) x2;
					Mix_PlayChannel(-1,BKP_FX->menu,0);
				}
				else if(a->input->Mouse.x > x3->pos.x && a->input->Mouse.x < x3->pos.x + x3->w)
				{
					((Agent *)(a->ptr0))->sprite_row -= 1;
					x3->sprite_row = getsite_row(x3,x3->name) + 1;
					Agent * y= ((Agent *)(a->ptr0))->ptr0;
					y->sprite_row = x3->sprite_row -1;
					x3->ptr0 = (void *) y;
					if(x3->ptr1 != NULL)
						((Agent *)(x3->ptr1))->visible = BKP_TRUE;
					a->ptr0 = (void *) x3;
					Mix_PlayChannel(-1,BKP_FX->menu,0);
				}
			}
			else
			{
				a->input->Mouse.l_click.pressed = 0;
				if(a->input->Mouse.x > Hud->obj0->pos.x && a->input->Mouse.x < Hud->obj0->pos.x + Hud->obj0->w)
				{
					if(BKP_Players->first->coins >= 500)
					{
						BKP_Players->first->sprite_row = 0;
						BKP_Players->first->w = 64 * Screen->ww;
						BKP_Players->first->h = 64 * Screen->hh;
						update_site((Agent *) a->ptr0,OBJ_NAME_BUYSILO);
						BKP_Players->first->coins -= 500;
						Agent * y= ((Agent *)(a->ptr0))->ptr0;
						y->sprite_row = ((Agent *)(a->ptr0))->sprite_row -1;
						Mix_PlayChannel(-1,BKP_FX->achat,0);
					}
				}
				else if(a->input->Mouse.x > Hud->obj1->pos.x && a->input->Mouse.x < Hud->obj1->pos.x + Hud->obj1->w)
				{
					if(BKP_Players->first->coins >= Hud->obj1->coins)
					{
						BKP_Players->first->sprite_row = 0;
						BKP_Players->first->w = 64 * Screen->ww;
						BKP_Players->first->h = 64 * Screen->hh;
						update_site((Agent *) a->ptr0,OBJ_NAME_BUYFURY);
						BKP_Players->first->coins -= Hud->obj1->coins;
						Hud->obj1->coins += 450;
						Agent * y= ((Agent *)(a->ptr0))->ptr0;
						y->sprite_row = ((Agent *)(a->ptr0))->sprite_row -1;
						Mix_PlayChannel(-1,BKP_FX->achat,0);
					}
				}
				else if(a->input->Mouse.x > Hud->obj2->pos.x && a->input->Mouse.x < Hud->obj2->pos.x + Hud->obj2->w)
				{
					if(BKP_Players->first->coins >= Hud->obj2->coins)
					{
						BKP_Players->first->sprite_row = 0;
						BKP_Players->first->w = 64 * Screen->ww;
						BKP_Players->first->h = 64 * Screen->hh;
						update_site((Agent *) a->ptr0,OBJ_NAME_BUYGATLING);
						BKP_Players->first->coins -= Hud->obj2->coins;
						Hud->obj2->coins += 1500;
						Agent * y= ((Agent *)(a->ptr0))->ptr0;
						y->sprite_row = ((Agent *)(a->ptr0))->sprite_row -1;
						Mix_PlayChannel(-1,BKP_FX->achat,0);
					}
				}
				else if(a->input->Mouse.x > Hud->obj3->pos.x && a->input->Mouse.x < Hud->obj3->pos.x + Hud->obj3->w)
				{
					if(BKP_Players->first->coins >= Hud->obj3->coins)
					{
						update_site((Agent *) a->ptr0,OBJ_NAME_BUYSHILD); Agent * y= ((Agent *)(a->ptr0))->ptr0; y->sprite_row = ((Agent *)(a->ptr0))->sprite_row -1;
						BKP_Players->first->coins -= Hud->obj3->coins;
						Hud->obj3->coins += 3000;

						BKP_Players->first->sprite_row = 2;
						BKP_Players->first->w = 900 * Screen->ww;
						BKP_Players->first->h = 256 * Screen->hh;
						Mix_PlayChannel(-1,BKP_FX->achat,0);
					}
				}

			}
		}
		else if(a->input->Mouse.x < 28 * Screen->ww && a->input->Mouse.y < 28 * Screen->hh)
		{
			a->flags[6] = 1;
		}
		else
		{
			((Agent *)(a->ptr0))->state_ = BKP_STATE_OPENFIRE;
			((Agent *)(a->ptr0))->fx.x = a->input->Mouse.x;
			((Agent *)(a->ptr0))->fx.y = a->input->Mouse.y;
		}
	}
}

/*--------------------------------------------------------------------------------*/
static void update_site(Agent * a,int obj_name)
{
	switch(obj_name)
	{
		case OBJ_NAME_BUYSILO:
			if(a->name != OBJ_NAME_BUYSILO)
			{
				a->timers[0].t_timer = SDL_GetTicks();
				a->timers[0].a_timer = 300;
				a->sprite_row = 1;
				a->name = OBJ_NAME_BUYSILO;
				a->text = BKP_FALSE;
			}
			break;
		case OBJ_NAME_BUYFURY:
			if(a->name != OBJ_NAME_BUYFURY)
			{
				a->timers[0].t_timer = SDL_GetTicks();
				a->timers[0].a_timer = 15000;
				a->sprite_row = 3;
				a->name = OBJ_NAME_BUYFURY;
				a->text = BKP_TRUE;
				a->coins = a->flags[3];
			}
			break;
		case OBJ_NAME_BUYGATLING:
			if(a->name != OBJ_NAME_BUYGATLING)
			{
				a->text = BKP_FALSE;
				a->timers[0].t_timer = SDL_GetTicks();
				a->timers[0].a_timer = 35000;
				a->sprite_row = 5;
				a->name = OBJ_NAME_BUYGATLING;
			}
			break;
		case OBJ_NAME_BUYSHILD:
			if(a->name != OBJ_NAME_BUYSHILD)
			{
				a->text = BKP_FALSE;
				a->timers[0].t_timer = SDL_GetTicks();
				a->timers[0].a_timer = 150000;
				a->sprite_row = 7;
				a->name = OBJ_NAME_BUYSHILD;
			}
			break;
	}
	a->state = BKP_STATE_IDLE;
	if(a->ptr0 != NULL)
		((Agent *)(a->ptr0))->flags[0] = -1;
	if(a->ptr1 != NULL)
		((Agent *)(a->ptr1))->state = BKP_STATE_DESTROY;
	a->ptr1 = NULL;

	return;
}

/*--------------------------------------------------------------------------------*/
static int getsite_row(Agent * a,int what)
{
	int i = 0;
	switch(what)
	{
		case OBJ_NAME_BUYSILO:
			i = 0;
			BKP_Players->first->sprite_row = 0;
			BKP_Players->first->w = 64 * Screen->ww;
			BKP_Players->first->h = 64 * Screen->hh;
				a->text = BKP_FALSE;
			break;
		case OBJ_NAME_BUYFURY:
			i = 2;
			BKP_Players->first->sprite_row = 0;
			BKP_Players->first->w = 64 * Screen->ww;
			BKP_Players->first->h = 64 * Screen->hh;
				a->text = BKP_TRUE;
				a->coins = a->flags[3];
			break;
		case OBJ_NAME_BUYGATLING:
			i = 4;
			BKP_Players->first->sprite_row = 0;
			BKP_Players->first->w = 64 * Screen->ww;
			BKP_Players->first->h = 64 * Screen->hh;
				a->text = BKP_FALSE;
			break;
		case OBJ_NAME_BUYSHILD:
			i = 6;
			BKP_Players->first->sprite_row = 2;
			BKP_Players->first->w = 900 * Screen->ww;
			BKP_Players->first->h = 256 * Screen->hh;
				a->text = BKP_FALSE;
			break;
		default:
			break;
	}

	return i;
}
/*--------------------------------------------------------------------------------*/
static void generateBomb(Agent * a)
{
	int ticks = SDL_GetTicks();
	if( BKP_Players->first->flags[7] == 1)
	{
		if(a->flags[7] == 0)
		{
			a->flags[7] = 1;
			a->timers[7].t_timer = SDL_GetTicks();
		}
		else
		{
			if(ticks - a->timers[7].t_timer >= a->timers[7].a_timer)
			{
				BKP_Players->first->flags[7] = 0;
				a->flags[7] = 0;
			}
		}

		return;
	}

	if(a->state == BKP_STATE_IDLE)
	{
		a->state = BKP_STATE_RELOADING;
		a->timers[0].t_timer = SDL_GetTicks();
		++a->flags[0];
		a->flags[1] += 1;
		a->flags[2] = a->flags[1];
		a->fflags[3] += .06;
		if(a->flags[3] > 2)
			a->flags[3] = 2;
		a->timers[1].a_timer -= 100;
		if(a->timers[1].a_timer < 300)
			a->timers[1].a_timer = 300;
		if(a->flags[0] % 10 == 0)
		{
			a->timers[1].a_timer -= 100;
			if(a->timers[1].a_timer < 500)
				a->timers[1].a_timer = 500;

			a->flags[1] += 10;
			//a->flags[1] = 1;
			a->flags[2] = a->flags[1];

			if(a->timers[0].a_timer < 10000)
				a->timers[0].a_timer += 1000;
		}
		Agent *x = create_object(Screen->w / 2 - 100 * Screen->ww,Screen->h / 3 - 128 * Screen->hh,BKP_AGENT_NPC,OBJ_TYPE_TEXT,400 * Screen->ww,200 * Screen->hh);
		Mix_PlayChannel(-1,BKP_FX->alert,0);
		x->sprite_sheet = NULL;
		x->ttl_time = SDL_GetTicks();
		x->ttl_delay = 3000;
		x->sprite_row = 1;
		x->plan = BKP_MAX_PLAN;
		bkp_Agentpush(x);
		bkp_updateAimg(x);
		x->aimg->ttc.r = 0xAA;
		x->aimg->ttc.g = 0xAA;
		x->aimg->ttc.b = 0x1A;
		sprintf(x->aimg->sz,"%d  Kamikazes Detected",a->flags[1]);
	}
	else if(a->state == BKP_STATE_RELOADING)
	{
		if(ticks - a->timers[0].t_timer >= a->timers[0].a_timer)
		{
			a->state = BKP_STATE_FIRE;
		}
	}
	else if(a->state == BKP_STATE_FIRE)
	{
		if(ticks - a->timers[1].t_timer >= a->timers[1].a_timer)
		{
			a->timers[1].t_timer = ticks;
			--a->flags[2];

			Agent *x = create_object(120 + rand() % (Screen->w - 140),-20,BKP_AGENT_NPC,OBJ_TYPE_BOMB,140 * Screen->ww,140 * Screen->hh);
			x->delays.d_idle = 20;
			x->delays.d_maxrun = 10;
			x->anim_delay = 20;
			x->sprite_sheet = BKP_GT->missile;
			x->fx.x = 120 + rand() % (Screen->w - 140);
			x->fx.y = BKP_Players->first->flags[1];
			x->stepx = 1;
			x->stepy = 1;
			x->sprite_row = 1;
			bkp_Agentpush(x);
			bkp_updateAimg(x);

			x->fflags[0] = M_ABS_MAX(x->fx.x - x->pos.x,x->fx.y - x->pos.y);
			x->fflags[1] = (float)(x->fx.x - x->pos.x) / x->fflags[0];
			x->fflags[2] = (float)(x->fx.y - x->pos.y) / x->fflags[0];
			if(x->fx.x > x->pos.x)
			{
				x->fflags[3] = -(90 -(180 * atan((float)(x->fx.y - x->pos.y)/(float)(x->fx.x - x->pos.x)) / 3.1415));
				x->fflags[4] = 8 * -.9;
			}
			else
			{
				x->fflags[3] = (90 +(180 * atan((float)(x->fx.y - x->pos.y)/(float)(x->fx.x - x->pos.x)) / 3.1415));
				x->fflags[4] = 8 * .9;
			}
			x->aimg->angle = x->fflags[3];
			x->fflags[0] = a->fflags[3];
			if(a->flags[2] == 0)
			{
				a->state = BKP_STATE_IDLE;
			}
		}
	}

}

/*--------------------------------------------------------------------------------*/
static int check_missile_colision(Agent * p)
{
	Agent *a = BKP_NPC->first;

	for(; a != NULL ;a = a->right)
	{
		if(a->type  == OBJ_TYPE_BOMB && a->state != BKP_STATE_DYING  && a->state != BKP_STATE_DEAD)
		{
			if(abs((p->pos.x + p->w /2) - (a->pos.x + a->w /2)) > p->w /3 + a->w /3)
				continue;
			if(abs((p->pos.y + p->h /2) - (a->pos.y + a->h /2)) > p->h /3 + a->h /3)
				continue;

			a->state = BKP_STATE_DYING;
			a->ttl_time = SDL_GetTicks();
			a->ttl_delay = 1;
			//		fprintf(stderr,"colision\n");
			Agent * ag = create_object(a->pos.x,a->pos.y,BKP_AGENT_NPC,OBJ_TYPE_DECOR,64 * Screen->ww,64 * Screen->hh);
			ag->sprite_sheet = BKP_GT->bazar;
			ag->name = OBJ_NAME_WHITEBALL2;
			ag->pos.y += ag->h + ag->h/3;
			ag->flags[1] = 1;
			ag->sprite_row = 0;
			ag->sprite_col = 2;
			ag->plan = 2;
			ag->flags[0] = ag->h;
			bkp_Agentpush(ag);
			bkp_updateAimg(ag);
			ag->aimg->alpha = 255;
			Mix_PlayChannel(-1,BKP_FX->explosion,0);
			return 1;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------------*/
static int check_missile_colision2(Agent * p)
{
	Agent *a = BKP_NPC->first;

	for(; a != NULL ;a = a->right)
	{

		if(a->type  == OBJ_TYPE_BOMB && a->state != BKP_STATE_DYING  && a->state != BKP_STATE_DEAD)
		{


			if(abs((p->pos.x + p->w /2) - (a->pos.x + a->w /2)) > abs(p->w/2 + a->w /3 ))
				continue;
			if(abs((p->pos.y + p->h /2) - (a->pos.y + a->h / 2)) > abs(p->h/2 + (a->h /3)))
				continue;

			a->state = BKP_STATE_DYING;
			a->ttl_time = SDL_GetTicks();
			a->ttl_delay = 1;
			//		fprintf(stderr,"colision\n");
			Agent * ag = create_object(a->pos.x,a->pos.y ,BKP_AGENT_NPC,OBJ_TYPE_DECOR,64 * Screen->ww,64 * Screen->hh);
			ag->sprite_sheet = BKP_GT->bazar;
			ag->pos.y += ag->h + ag->h/3;
			ag->name = OBJ_NAME_WHITEBALL2;
			ag->flags[1] = 1;
			ag->sprite_row = 0;
			ag->sprite_col = 2;
			ag->plan = 2;
			ag->flags[0] = ag->h;
			bkp_Agentpush(ag);
			bkp_updateAimg(ag);
			ag->aimg->alpha = 255;
			Mix_PlayChannel(-1,BKP_FX->explosion,0);
			return 1;
		}
	}
	return 0;
}

/*--------------------------------------------------------------------------------*/
static void kill_all_obj()
{
	Agent *a = BKP_NPC->first;

	Mix_HaltChannel(GAT_CHN);
	Mix_HaltChannel(SHD_CHN);
	for(; a != NULL ;a = a->right)
	{
		if(a->type  == OBJ_TYPE_BOMB || a->type == OBJ_TYPE_MISSILE || a->type == OBJ_TYPE_SHIELD)
		{
			a->state = BKP_STATE_DYING;
			a->ttl_time = SDL_GetTicks();
			a->ttl_delay = 1;
		}
	}
	return;
}

/*--------------------------------------------------------------------------------*/
static void shoot_multi(Agent *a)
{
	a->state = BKP_STATE_DYING;
	a->ttl_time = SDL_GetTicks();
	a->ttl_delay = 1;
	create_leure(350 * Screen->ww,300 * Screen->hh,a->pos.x,a->pos.y);
	create_leure(150 * Screen->ww,300 * Screen->hh,a->pos.x,a->pos.y);
	create_leure(0,300 * Screen->hh,a->pos.x,a->pos.y);
	create_leure(-350,300 * Screen->hh,a->pos.x,a->pos.y);
	create_leure(-150,300 * Screen->hh,a->pos.x,a->pos.y);
}

/*--------------------------------------------------------------------------------*/
void create_leure(float fx,float fy,float x,float y)
{
	Agent * ag = create_object(x,y ,BKP_AGENT_NPC,OBJ_TYPE_DECOR,64 * Screen->ww,64 * Screen->hh);
	ag->sprite_sheet = BKP_GT->bazar;
	ag->pos.y += ag->h + ag->h/3;
	ag->name = OBJ_NAME_WHITEBALL2;
	ag->flags[1] = 1;
	ag->sprite_row = 0;
	ag->sprite_col = 2;
	ag->plan = 2;
	ag->flags[0] = ag->h;
	bkp_Agentpush(ag);
	bkp_updateAimg(ag);
	ag->aimg->alpha = 255;
	Mix_PlayChannel(-1,BKP_FX->explosion,0);

	ag = create_object(x,y ,BKP_AGENT_NPC,OBJ_TYPE_LEURE,40 * Screen->ww,40 * Screen->hh);
	ag->sprite_sheet = BKP_GT->missile;
	ag->fx.x =ag->pos.x + fx;
	ag->fx.y =ag->pos.y + fy;
	ag->name = OBJ_NAME_LEURE;
	ag->sprite_row = 3;
	ag->sprite_col = 0;
	ag->plan = 1;
	ag->stepx = 6;
	ag->stepy = 6;
	ag->flags[5] = 1;
	ag->state = BKP_STATE_MOTION;
	bkp_Agentpush(ag);
	bkp_updateAimg(ag);
	ag->fflags[0] = M_ABS_MAX(ag->fx.x - ag->pos.x,ag->fx.y - ag->pos.y);
	ag->fflags[1] = (float)(ag->fx.x - ag->pos.x) / ag->fflags[0];
	ag->fflags[2] = (float)(ag->fx.y - ag->pos.y) / ag->fflags[0];
	if(ag->fx.x <= ag->pos.x)
	{
		ag->fflags[3] = -(90 -(180 * atan((float)(ag->fx.y - ag->pos.y)/(float)(ag->fx.x - ag->pos.x)) / 3.1415));
		ag->fflags[4] = 8 * -.9;
	}
	else
	{
		ag->fflags[3] = (90 +(180 * atan((float)(ag->fx.y - ag->pos.y)/(float)(ag->fx.x - ag->pos.x)) / 3.1415));
		ag->fflags[4] = 8 * .9;
	}
	ag->aimg->angle = ag->fflags[3];
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

