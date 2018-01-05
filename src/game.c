#include <stdlib.h>
#include <stdio.h>

#include "include/helper.h"
#include "include/game.h"
#include "include/manager.h"
#include "include/graphics.h"
#include "include/macro.h"
#include "include/input.h"

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

static void initPlayer(Agent *a);
static void create_hudobj();
static void update_score();
static int menu_pause();
static void zoom_in();
static void zoom_out();

/**************************************************************************
*	Implementations
**************************************************************************/

int logo()
{
	bkp_GraphicdrawLogo(3000);
	return 0;
}

/*--------------------------------------------------------------------------------*/
int outlogo()
{
	bkp_GraphicdrawOut(2000);
	return 0;
}

/*--------------------------------------------------------------------------------*/
int menu()
{
	int y = 3;
	int Y = Screen->h / 2 - 200 * Screen->hh;
	int hh = 50 * Screen->hh;
	int dy = 150 * Screen->hh;

	Agent * p = BKP_Players->first;


	while(1)
	{
		bkp_ManageInput(p);
		BKP_Players->first->aimg->sprite_row = 0;
		BKP_Players->first->sprite_row = 0;
		BKP_Players->first->w = 64 * Screen->ww;
		BKP_Players->first->h = 64 * Screen->hh;
		bkp_GraphicdrawMenu(y);
		bkp_updateAimg(p);
		if(p->input->Mouse.x > 800 * Screen->ww && p->input->Mouse.x < 1113 * Screen->ww &&p->input->Mouse.y > Y && p->input->Mouse.y < Y + hh)
		{
			if(y != 0)
				Mix_PlayChannel(-1,BKP_FX->menu,0);
			y = 0;
		}
		else if(p->input->Mouse.x > 714 * Screen->ww && p->input->Mouse.x < 1150 * Screen->ww && p->input->Mouse.y > Y + dy && p->input->Mouse.y < Y + dy + hh)
		{
			if(y != 1)
				Mix_PlayChannel(-1,BKP_FX->menu,0);
			y = 1;
		}
		else if(p->input->Mouse.x > 800 * Screen->ww && p->input->Mouse.x < 1113 * Screen->ww && p->input->Mouse.y > Y + dy + dy && p->input->Mouse.y < Y + dy + dy + hh)
		{
			if(y != 2)
				Mix_PlayChannel(-1,BKP_FX->menu,0);
			y = 2;
		}
		else
		{
			y = 3;
		}

//		fprintf(stderr,"%.2f vs %.2f and %.2f %.2f\n",p->input->Mouse.x,500 * Screen->ww, 817 * Screen->ww,Screen->ww);
		if(p->input->Mouse.l_click.pressed != 0)
		{
			if(y < 3)
			{
				break;
			}
		}

	}

	bkp_resetkeys(p->input);
	return y;
}

/*--------------------------------------------------------------------------------*/
int Hiscore()
{
	FILE * inf = fopen("data/save","rb");
	int T[10][2];
	int i;
	int hh = 100 * Screen->hh;

	if(inf == NULL)
	{
		fprintf(stderr,"Cannot open '%s'\n","data/save");
		for(i = 0;i<10 ; ++i)
		{
			T[i][0] = 0;
			T[i][1] = (10 - i) * 10;
		}
	}
	else
	{
		for(i = 0;i<10 ; ++i)
		{
			fread(&T[i][0],sizeof(int),1,inf);
			fread(&T[i][1],sizeof(int),1,inf);
//			fprintf(stdout,"%d  ---------   %d\n",T[i][0],T[i][1]);
		}
		fclose(inf);
	}

	Agent * p = BKP_Players->first;
	int y = 1;

	while(1)
	{
		bkp_ManageInput(p);
		bkp_GraphicdrawScore(T,y);
		BKP_Players->first->aimg->sprite_row = 0;
		BKP_Players->first->sprite_row = 0;
		BKP_Players->first->w = 64 * Screen->ww;
		BKP_Players->first->h = 64 * Screen->hh;
		bkp_updateAimg(p);
		if(p->input->Mouse.x > 800 * Screen->ww && p->input->Mouse.x < 1113 * Screen->ww && p->input->Mouse.y < hh)
		{
			if(y != 0)
				Mix_PlayChannel(-1,BKP_FX->menu,0);
			y = 0;
		}
		else
			y = 1;

		if(p->input->Mouse.l_click.pressed != 0)
		{
			if(y < 1)
			{
				break;
			}
		}

	}

	bkp_resetkeys(p->input);
	return 0;
}
/*--------------------------------------------------------------------------------*/

int Play()
{
	Agent *p = BKP_Players->first;
	p->coins = 4000.00;
	p->score = 0;
	p->PV = 5;
	initPlayer(p);





	create_silo();
	create_hudobj();
	create_generatorBomb();

	zoom_in();

	Mix_PlayChannel(7,BKP_FX->brr,-1);
	while(1)
	{
		bkp_ManagePlayers();
		bkp_ManageNPC();
		bkp_Graphicdraw();
		if(p->input->Cancel == 1 || p->PV < 1)
		{
			break;
		}
		if(p->flags[6] == 1 )
		{
		    int k = BKP_Players->first->sprite_row;
		    int kw = BKP_Players->first->w ;
			int kh = BKP_Players->first->h ;
		    BKP_Players->first->sprite_row = 1;
            BKP_Players->first->w = 64 * Screen->ww;
			BKP_Players->first->h = 64 * Screen->hh;
			Mix_HaltChannel(-1);

			int val = menu_pause();
            BKP_Players->first->w = kw;
			BKP_Players->first->h = kh;
			p->flags[6] = 0;
			BKP_Players->first->sprite_row = k;
			if(val == 1) break;
		}
	}

	Mix_HaltChannel(7);

	update_score();


	bkp_AgentClearNPC() ;
	bkp_resetkeys(p->input);


	SDL_Delay(500);

	zoom_out();

	return 0;
}

/*--------------------------------------------------------------------------------*/
void initPlayer(Agent *a)
{
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


/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
static void create_hudobj()
{
	int x_init = 512 * Screen->ww;
	int y_init = Hud->pos.y + 16 * Screen->hh;
	int x;
	Agent * a = NULL;

	a  = create_object(128 * Screen->ww ,Hud->pos.y + 10 * Screen->hh,BKP_AGENT_NPC,OBJ_TYPE_DECOR,250 * Screen->ww ,250 * Screen->hh);
	a->name = OBJ_NAME_DECOR;
	a->text = BKP_FALSE;
	a->sprite_sheet = BKP_GT->bazar;
	a->sppl = 1;
	a->sprite_row = 1;
	a->plan = 1;
	bkp_Agentpush(a);

	a  = create_object(550 * Screen->ww ,Hud->pos.y + 10 * Screen->hh,BKP_AGENT_NPC,OBJ_TYPE_DECOR,128 ,128);
	a->name = OBJ_NAME_DECOR;
	a->text = BKP_FALSE;
	a->sprite_sheet = BKP_GT->bazar;
	a->sppl = 1;
	a->sprite_row = 1;
	a->plan = 1;
	a->visible = BKP_FALSE;
	bkp_Agentpush(a);
	Hud->obj4 = a;

	a = create_object(x_init, y_init,BKP_AGENT_NPC,OBJ_TYPE_SILO,128,128);
	a->name = OBJ_NAME_BUYSILO;
	a->sprite_sheet = BKP_GT->silo ;
	a->sppl = 4;
	a->sprite_row = 0;
	a->plan = BKP_MAX_PLAN;
	a->coins = 500;
	a->text = BKP_TRUE;
	Hud->obj0 = a;
	bkp_Agentpush(a);
	a = create_object(x_init,y_init,BKP_AGENT_NPC,OBJ_TYPE_DECOR,128 ,128);
	a->name = OBJ_NAME_HIDEBUY;
	a->fflags[0] = 500;
	a->text = BKP_FALSE;
	a->sprite_sheet = BKP_GT->bazar;
	a->sppl = 4;
	a->sprite_row = 0;
	a->plan = BKP_MAX_PLAN + 1;
	bkp_Agentpush(a);
	x = a->pos.x + 5 * Screen->ww + a->w;


	a = create_object(x, y_init,BKP_AGENT_NPC,OBJ_TYPE_SILO,128,128);
	a->name = OBJ_NAME_BUYFURY;
	a->text = BKP_TRUE;
	a->sprite_sheet = BKP_GT->silo ;
	a->sppl = 4;
	a->sprite_row = 2;
	a->plan = BKP_MAX_PLAN;
	a->coins = 1800;
	Hud->obj1 = a;
	bkp_Agentpush(a);
	a = create_object(x,y_init,BKP_AGENT_NPC,OBJ_TYPE_DECOR,128 ,128);
	a->name = OBJ_NAME_HIDEBUY;
	a->text = BKP_FALSE;
	a->fflags[0] = 1800;
	a->sprite_sheet = BKP_GT->bazar;
	a->sppl = 4;
	a->sprite_row = 0;
	a->plan = BKP_MAX_PLAN + 1;
	bkp_Agentpush(a);
	x += 5 * Screen->ww + a->w;

	a = create_object(x, y_init,BKP_AGENT_NPC,OBJ_TYPE_SILO,128,128);
	a->text = BKP_TRUE;
	a->name = OBJ_NAME_BUYGATLING;
	a->sprite_sheet = BKP_GT->silo ;
	a->sppl = 4;
	a->sprite_row = 4;
	a->plan = BKP_MAX_PLAN;
	a->coins = 5000;
	Hud->obj2 = a;
	bkp_Agentpush(a);
	a = create_object(x,y_init,BKP_AGENT_NPC,OBJ_TYPE_DECOR,128 ,128);
	a->name = OBJ_NAME_HIDEBUY;
	a->text = BKP_FALSE;
	a->fflags[0] = 5000;
	a->sprite_sheet = BKP_GT->bazar;
	a->sppl = 4;
	a->sprite_row = 0;
	a->plan = BKP_MAX_PLAN + 1;
	bkp_Agentpush(a);
	x += 5 * Screen->ww + a->w;

	a = create_object(x, y_init,BKP_AGENT_NPC,OBJ_TYPE_SILO,128,128);
	a->text = BKP_TRUE;
	a->name = OBJ_NAME_BUYSHILD;
	a->coins = 13000;
	a->sprite_sheet = BKP_GT->silo ;
	a->sppl = 4;
	a->sprite_row = 6;
	a->plan = BKP_MAX_PLAN;
	Hud->obj3 = a;
	bkp_Agentpush(a);
	a = create_object(x,y_init,BKP_AGENT_NPC,OBJ_TYPE_DECOR,128 ,128);
	a->name = OBJ_NAME_HIDEBUY;
	a->text = BKP_FALSE;
	a->fflags[0] = 13000;
	a->sprite_sheet = BKP_GT->bazar;
	a->sppl = 4;
	a->sprite_row = 0;
	a->plan = BKP_MAX_PLAN + 1;
	bkp_Agentpush(a);
	x += 5 * Screen->ww + a->w;

	a = create_object(0,BKP_Players->first->flags[1] ,BKP_AGENT_NPC,OBJ_TYPE_DECOR,Screen->w,Hud->pos.y - BKP_Players->first->flags[1]);
	a->name = OBJ_NAME_DOME;
	a->pos.y -= 50 * Screen->hh;
	a->text = BKP_FALSE;
	a->sprite_sheet = BKP_GT->bazar;
	a->sppl = 4;
	a->sprite_row = 0;
	a->plan = 0;
	a->w = Screen->w;
	a->h *= 2;
	a->h += 100 * Screen->hh;
	Hud->shield = a;
	bkp_Agentpush(a);
	a->aimg->alpha = 255;

	a = create_object(0,BKP_Players->first->flags[1] + 20 * Screen->hh,BKP_AGENT_NPC,OBJ_TYPE_DECOR,Screen->w,Hud->pos.y - BKP_Players->first->flags[1]);
	a->name = OBJ_NAME_DECOR;
	a->text = BKP_FALSE;
	a->sprite_sheet = BKP_GT->ship;
	a->sprite_w = 1142;
	a->sprite_h = 252;
	a->sppl = 4;
	a->sprite_row = 0;
	a->plan = 0;
	a->w = Screen->w;
	a->h += 100 * Screen->hh;
	bkp_Agentpush(a);

	a = create_object(0,0,BKP_AGENT_NPC,OBJ_TYPE_DECOR,64 ,64);
	a->name = OBJ_NAME_DECOR;
	a->text = BKP_FALSE;
	a->sprite_sheet = BKP_GT->player;
	a->sppl = 4;
	a->sprite_row = 3;
	a->plan = BKP_MAX_PLAN;
	bkp_Agentpush(a);
	return;
}

/*--------------------------------------------------------------------------------*/
static void update_score()
{
	FILE * inf = fopen("data/save","rb");
	int now = getepoch();
	char buf[128];
	int T[12][2];
	int i;

	epochtodate(buf,128,now);
//	fprintf(stdout,"%d - ",now);
//	fprintf(stdout,"%s  --------------------  %d\n", buf,BKP_Players->first->score);

	if(inf == NULL)
	{
		fprintf(stderr,"Cannot open '%s'\n","data/save");
		for(i = 0;i<10 ; ++i)
		{
			T[i][0] = 0;
			T[i][1] = (10 - i) * 10;
		}
	}
	else
	{
		for(i = 0;i<10 ; ++i)
		{
			fread(&T[i][0],sizeof(int),1,inf);
			fread(&T[i][1],sizeof(int),1,inf);
//			fprintf(stdout,"%d  ---------   %d\n",T[i][0],T[i][1]);
		}
		fclose(inf);
	}

	FILE * outf = fopen("data/save","wb");
	if(outf == NULL)
	{
		fprintf(stderr,"Cannot open '%s'\n","data/dave");
	}

	for(i = 0 ;i < 10 ; ++i)
	{
		if(BKP_Players->first->score >= T[i][1])
		{
			int j;
			for(j = 10;j > i;--j)
			{
				T[j][0] = T[j-1][0]	;
				T[j][1] = T[j-1][1]	;
			}
			T[i][0] = 	now;
			T[i][1] = 	BKP_Players->first->score	;
			break;
		}
	}

	for(i = 0 ;i < 10 ; ++i)
	{
		fwrite(&T[i][0],sizeof(int),1,outf);
		fwrite(&T[i][1],sizeof(int),1,outf);
	}
	fclose(outf);

	return;
}

/*--------------------------------------------------------------------------------*/
static int menu_pause()
{
	int Y = Screen->h / 2 - 200 * Screen->hh;
	int hh = 50 * Screen->hh;
	int dy = 150 * Screen->hh;
	int Start = SDL_GetTicks();

	Agent * p = BKP_Players->first;
	int y = 2;

	while(1)
	{
		bkp_ManageInput(p);
		bkp_updateAimg(p);
		bkp_GraphicdrawPause(y);

		if(p->input->Mouse.x > 800 * Screen->ww && p->input->Mouse.x < 1113 * Screen->ww &&p->input->Mouse.y > Y && p->input->Mouse.y < Y + hh)
		{
			if(y != 0)
				Mix_PlayChannel(-1,BKP_FX->menu,0);
			y = 0;
		}
		else if(p->input->Mouse.x > 714 * Screen->ww && p->input->Mouse.x < 1150 * Screen->ww && p->input->Mouse.y > Y + dy && p->input->Mouse.y < Y + dy + hh)
		{
			if(y != 1)
				Mix_PlayChannel(-1,BKP_FX->menu,0);
			y = 1;
		}
		else
		{
			y = 3;
		}

		if(p->input->Mouse.l_click.pressed != 0 && y < 2)
			break;
	}

	Start = SDL_GetTicks() - Start;

	Agent *a = BKP_NPC->first;
	int i;
	for(; a != NULL; a = a->right)
	{
		for(i = 0; i < 8; ++i)
			if(a->timers[i].t_timer > 0)
				a->timers[i].t_timer += Start;
	}
	a = BKP_Players->first;
	for(; a != NULL; a = a->right)
	{
		for(i = 0; i < 8; ++i)
			if(a->timers[i].t_timer > 0)
				a->timers[i].t_timer += Start;
	}

	bkp_resetkeys(p->input);

	return y;
}

/*--------------------------------------------------------------------------------*/
static void zoom_in()
{
	int w = 320,
		h=200,
		ww = (1131 - 320) / 60,
		hh = (707 - 200) / 60;
	while(1)
	{
		w += ww;
		h += hh;
		if(w >= 1131)
			w = 1131;
		if(h >= 707)
			h = 707;
		bkp_GraphicdrawIntro(w,h);
		if(w == 1131 && h == 707)
			break;
	}
	return;
}
/*--------------------------------------------------------------------------------*/
static void zoom_out()
{
	int w = 1131,h= 707,
		ww = (1131 - 320) / 60,
		hh = (707 - 200) / 60;
	while(1)
	{
		w -= ww;
		h -= hh;
		if(w <= 320)
			w = 320;
		if(h <= 200)
			h = 200;
		bkp_GraphicdrawIntro(w,h);
		if(w == 320 && h == 200)
			break;
	}
	return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

