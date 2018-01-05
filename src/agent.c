#include <stdlib.h>
#include <stdio.h>

/*Copyright*/
/*
 * AUTHOR: lilington
 * addr	: lilington@yahoo.fr
 *
 */
#include "include/macro.h"
#include "include/agent.h"
#include "include/memory.h"
#include "include/input.h"

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

static void aimg_push(Agent *a);
static void aimg_remove(Agent * a);


/**************************************************************************
*	Implementations	
**************************************************************************/

/*--------------------------------------------------------------------------------*/
void bkp_AgentClearNPC() 
{
	Agent *a = BKP_NPC->first;

	for(; a != NULL ;a = a->right) 
	{
		bkp_Agentdestroy(a);
	}

	return;
}
/*--------------------------------------------------------------------------------*/
Agent * bkp_Agentnew(int type)
{
	Agent * a = bkp_MemoryGet(BKP_MemOBJ_AGENT);
	static int Ids = 0;

	a->input = NULL;
	a->right = a->left = NULL;
	a->agent = type;

	if(type == BKP_AGENT_PLAYER)
	{
		ALLOC(BKP_Input,a->input,1);
		bkp_InputInit(a->input);
	}

	a->id = Ids++;

	return a;
}

/*--------------------------------------------------------------------------------*/
void bkp_Agentdestroy(Agent *a)
{
	if(a->input != NULL)
	{
		free(a->input);
	}
	if(a->agent == BKP_AGENT_PLAYER)
	{
		//should remove from list
		aimg_remove(a);
		SDL_DestroyTexture(a->sprite_sheet);
	}
	else
	{
		aimg_remove(a);

		if(a->left != NULL)
			a->left->right = a->right;
		else
			BKP_NPC->first = a->right;

		if(a->right != NULL)
			a->right->left = a->left;
		else
			BKP_NPC->last = a->left;

		--BKP_NPC->size;

	}

	bkp_MemoryRelease(a,BKP_MemOBJ_AGENT);

	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_AgentpushPlayer(Agent *a)
{
	if(BKP_Players->first != NULL)
	{
		a->right = BKP_Players->first;
		BKP_Players->first->left = a;
	}
	else
	{
		BKP_Players->last = a;
	}

	BKP_Players->first = a;

	BKP_Players->size +=1;

	aimg_push(a);

	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_Agentpush(Agent *a)
{
	if(BKP_NPC->first != NULL)
	{
		a->right = BKP_NPC->first;
		BKP_NPC->first->left = a;
	}
	else
	{
		BKP_NPC->last = a;
	}

	BKP_NPC->first = a;
	BKP_NPC->size +=1;

	aimg_push(a);

	return;
}

/*--------------------------------------------------------------------------------*/

/************************* Static functions ***********************************/
static void aimg_push(Agent *a)
{
	Aimg * ai = bkp_MemoryGet(BKP_MemOBJ_AIMG);
	ai->left = NULL;
	ai->right = NULL;
	ai->type = a->type;

	ai->pos.x = a->pos.x;ai->pos.y = a->pos.y; ai->w = ai->sprite_w = a->sprite_w ;
	ai->h = ai->sprite_h = a->sprite_h;
	ai->visible = a->visible,ai->sprite_col = a->sprite_col;ai->sprite_row= a->sprite_row;
	ai->sprite_sheet = a->sprite_sheet;
	ai->coins = a->coins;
	ai->text = a->text;
	ai->agent = a->agent;
	ai->RotatecenterisNull = 0;
	ai->angle = 0;
	ai->alpha = -1;
	a->aimg = ai;

	if(BKP_AimgPlan[a->plan]->first != NULL)
	{
		ai->right = BKP_AimgPlan[a->plan]->first;
		BKP_AimgPlan[a->plan]->first->left = ai;
	}
	else
	{
		BKP_AimgPlan[a->plan]->last = ai;
	}

	BKP_AimgPlan[a->plan]->first = ai;

	BKP_AimgPlan[a->plan]->size +=1;

	return;
}

/*--------------------------------------------------------------------------------*/
static void aimg_remove(Agent * a)
{
	if(a->aimg->left != NULL)
		a->aimg->left->right = a->aimg->right;
	else
		BKP_AimgPlan[a->plan]->first = a->aimg->right;

	if(a->aimg->right != NULL)
		a->aimg->right->left = a->aimg->left;
	else
		BKP_AimgPlan[a->plan]->last= a->aimg->left;

	--BKP_AimgPlan[a->plan]->size;

	bkp_MemoryRelease(a->aimg,BKP_MemOBJ_AIMG);

	return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
