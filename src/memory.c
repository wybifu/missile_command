#include <stdlib.h>
#include <stdio.h>

/*Copyright*/
/*
 * AUTHOR: lilington
 * addr	: lilington@yahoo.fr
 *
 */

#include "include/macro.h"
#include "include/memory.h"

/**************************************************************************
*	Defines & Maro
**************************************************************************/

/**************************************************************************
*	Structs, Enum, Unio and Typesdef	
**************************************************************************/

struct MMem
{
	struct MMem * next;
}MMem;

/**************************************************************************
*	Globals	
**************************************************************************/

struct MMem * O_agent_head = NULL,
			* O_agent_first = NULL,

			* O_aimg_head = NULL,
			* O_aimg_first = NULL;

int PoolSize_agent = DEFAULT_AGENT_POOL_SIZE;
int PoolSize_aimg = DEFAULT_AGENT_POOL_SIZE;

/***************************************************************************
*	Prototypes 	
**************************************************************************/

static void pool_expande(int o_type);
static size_t get_poolSize(int o_type,size_t *size);
static void  set_poolFirst(int o_type,struct MMem * mm);
static void  set_poolHead(int o_type,struct MMem * mm);

/**************************************************************************
*	Implementations	
**************************************************************************/

void bkp_MemorySetPoolSize(int pool,int size)
{
	if(size < 1)
		return;

	switch(pool)
	{
		case BKP_MemOBJ_AGENT:
			PoolSize_agent = size;
			break;
		case BKP_MemOBJ_AIMG:
			PoolSize_aimg = size;
			break;
		 default:
			break;
	}

	return;
}

/*--------------------------------------------------------------------------------*/
void * bkp_MemoryGet(int o_type)
{
	struct MMem * head = NULL;

	if(BKP_MemOBJ_AGENT == o_type)
	{
		if(NULL == O_agent_head)
			pool_expande(BKP_MemOBJ_AGENT);

		head = O_agent_head;
		O_agent_head = head->next;

		return (Agent *)head;
	}
	else if(BKP_MemOBJ_AIMG == o_type)
	{
		if(NULL == O_aimg_head)
			pool_expande(BKP_MemOBJ_AIMG);

		head = O_aimg_head;
		O_aimg_head = head->next;

		return (Aimg *)head;
	}

	return NULL;
}

/*--------------------------------------------------------------------------------*/
void bkp_MemoryRelease(void * ptr,int o_type)
{
	struct MMem * head = (struct MMem *) ptr;

	if(BKP_MemOBJ_AGENT == o_type)
	{
		head->next = O_agent_head;
		O_agent_head = head;
	}
	else if(BKP_MemOBJ_AIMG == o_type)
	{
		head->next = O_aimg_head;
		O_aimg_head = head;
	}

	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_MemoryClean(int o_type)
{
	struct MMem * ptr;

	if(BKP_MemOBJ_AGENT == o_type || BKP_MemOBJ_ALL == o_type)
	{
		ptr = O_agent_head;
		if(ptr != NULL)
		{
			for(;ptr;ptr = O_agent_head)
			{
				O_agent_head = O_agent_head->next;
				free(ptr);
			}
		}
		O_agent_head = NULL;
		O_agent_first = NULL;
	}
	if(BKP_MemOBJ_AIMG == o_type || BKP_MemOBJ_ALL == o_type)
	{
		ptr = O_aimg_head;
		if(ptr != NULL)
		{
			for(;ptr;ptr = O_aimg_head)
			{
				O_aimg_head = O_aimg_head->next;
				free(ptr);
			}
		}
		O_aimg_head = NULL;
		O_aimg_first = NULL;
	}

	return;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

/********************** Static functions *******************/

static void pool_expande(int o_type)
{
	int i;
	size_t size = 0,poolsize;
	struct MMem * head = NULL;

	poolsize = get_poolSize(o_type,&size);
	ALLOC(struct MMem,head,size);

	set_poolFirst(o_type,head);
	set_poolHead(o_type,head);


	for(i=0; i < poolsize; ++i)
	{
		ALLOC(struct MMem,head->next,size);
		head = head->next;
	}

	head->next = NULL;

	return;
}

/*--------------------------------------------------------------------------------*/
static size_t get_poolSize(int o_type,size_t *size)
{
	size_t s = 0;


	switch(o_type)
	{
		case BKP_MemOBJ_AGENT:
			s = PoolSize_agent;
			*size = (sizeof(Agent) > sizeof(struct MMem)) ? sizeof(Agent) : sizeof(MMem);
			break;
		case BKP_MemOBJ_AIMG:
			s = PoolSize_aimg;
			*size = (sizeof(Aimg) > sizeof(struct MMem)) ? sizeof(Aimg) : sizeof(MMem);
			break;
		default:
			s = -1;
			break;
	}
	return s;
}

/*--------------------------------------------------------------------------------*/
static void  set_poolFirst(int o_type,struct MMem * mm)
{
	switch(o_type)
	{
		case BKP_MemOBJ_AGENT:
			if(NULL == O_agent_first)
				O_agent_first = mm;
			break;
		case BKP_MemOBJ_AIMG:
			if(NULL == O_aimg_first)
				O_aimg_first = mm;
			break;
		default:
			BKP_DIE("[Fatal Error] unknown pool \n",-1);
	}

	return ;
}
/*--------------------------------------------------------------------------------*/
static void set_poolHead(int o_type,struct MMem * mm)
{
	switch(o_type)
	{
		case BKP_MemOBJ_AGENT:
			O_agent_head = mm;
			break;
		case BKP_MemOBJ_AIMG:
			O_aimg_head = mm;
			break;
		default:
			BKP_DIE("[Fatal Error] unknown pool \n",-1);
	}

	return ;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

