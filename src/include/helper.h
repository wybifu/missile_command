#ifndef __HELPER_H_
#define __HELPER_H_

#include "blukpast.h"
#include "agent.h"

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

Agent * create_object(float x,float y,int agent,int type,float w,float h);
void create_silo();
void create_generatorBomb();
void setZero(char *p,int t);
void epochtodate(char buf[],int N,int t);
int getepoch();

#endif

