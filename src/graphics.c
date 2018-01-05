#include <stdlib.h>
#include <stdio.h>

/*Copyright*/
/*
 * AUTHOR: lilington
 * addr	: lilington@yahoo.fr
 *
 */

#include "include/graphics.h"
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

/***************************************************************************
*	Prototypes 	
**************************************************************************/

static void flushall();
static void drawPlayers();
static void drawAgent(Aimg *a);
static void drawHud();
static void drawPlan(int plan);
static void drawText(const char *mess,TTF_Font * font,int font_size,int x,int y,int w,int h,SDL_Color textcolor);
static void drawBG(int w,int h);

/**************************************************************************
*	Implementations	
**************************************************************************/

void bkp_Graphicdraw()
{
	SDL_SetRenderDrawColor(Screen->renderer,0x00,0x00,0x00,200);
	SDL_RenderClear(Screen->renderer);

	drawBG(0,0);

	drawPlan(0);
	drawPlan(1);
	drawPlan(2);

	drawHud();

	drawPlayers();

	flushall();
	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_GraphicdrawIntro(int w,int h)
{
	SDL_SetRenderDrawColor(Screen->renderer,0x00,0x00,0x00,200);
	SDL_RenderClear(Screen->renderer);

	drawBG(w,h);

	flushall();
}
/*--------------------------------------------------------------------------------*/
void bkp_GraphicdrawPause(int y)
{
	int X = Screen->w / 2 - 200 * Screen->ww;
	int Y = Screen->h / 2 - 200 * Screen->hh;
	int dy = 150 * Screen->hh;
	SDL_Color ttc = {0xff,0xff,0xff};
	SDL_Color ttc1 = {0xff,0xff,0x00};

	SDL_SetRenderDrawColor(Screen->renderer,0x00,0x00,0x00,200);
	SDL_RenderClear(Screen->renderer);

	drawBG(0,0);

	drawPlan(0);
	drawPlan(1);
	drawPlan(2);

	drawHud();

	drawPlayers();

	if(y == 0)
	{
		drawText(" Continue ",NULL,100 * Screen->hh,X,Y, 400 * Screen->ww,100 * Screen->hh,ttc);
		drawText("   Quit   ",NULL,100 * Screen->hh,X,Y + dy, 400 * Screen->ww,100 * Screen->hh,ttc1);
	}
	else if(y == 1)
	{
		drawText(" Continue ",NULL,100 * Screen->hh,X,Y, 400 * Screen->ww,100 * Screen->hh,ttc1);
		drawText("   Quit   ",NULL,100 * Screen->hh,X,Y + dy, 400 * Screen->ww,100 * Screen->hh,ttc);
	}
	else
	{
		drawText(" Continue ",NULL,100 * Screen->hh,X,Y, 400 * Screen->ww,100 * Screen->hh,ttc1);
		drawText("   Quit   ",NULL,100 * Screen->hh,X,Y + dy, 400 * Screen->ww,100 * Screen->hh,ttc1);
	}

	drawPlayers();

	flushall();
	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_GraphicdrawMenu(int y)
{
	int X = Screen->w / 2 - 200 * Screen->ww;
	int Y = Screen->h / 2 - 200 * Screen->hh;
	int dy = 150 * Screen->hh;

	SDL_SetRenderDrawColor(Screen->renderer,0x00,0x00,0x00,200);
	SDL_RenderClear(Screen->renderer);
	SDL_Color ttc = {0xff,0xff,0xff};
	SDL_Color ttc1 = {0xff,0xff,0x00};

	drawBG(320,200);

	if(y == 0)
	{
		drawText("   Play   ",NULL,100 * Screen->hh,X,Y, 400 * Screen->ww,100 * Screen->hh,ttc);
		drawText("High Score",NULL,100 * Screen->hh,X,Y + dy, 400 * Screen->ww,100 * Screen->hh,ttc1);
		drawText("   Quit   ",NULL,100 * Screen->hh,X,Y + dy + dy, 400 * Screen->ww,100 * Screen->hh,ttc1);
	}
	else if(y == 1)
	{
		drawText("   Play   ",NULL,100 * Screen->hh,X,Y, 400 * Screen->ww,100 * Screen->hh,ttc1);
		drawText("High Score",NULL,100 * Screen->hh,X,Y + dy, 400 * Screen->ww,100 * Screen->hh,ttc);
		drawText("   Quit   ",NULL,100 * Screen->hh,X,Y + dy + dy, 400 * Screen->ww,100 * Screen->hh,ttc1);
	}
    else if(y == 2)
	{
		drawText("   Play   ",NULL,100 * Screen->hh,X,Y, 400 * Screen->ww,100 * Screen->hh,ttc1);
		drawText("High Score",NULL,100 * Screen->hh,X,Y + dy, 400 * Screen->ww,100 * Screen->hh,ttc1);
		drawText("   Quit   ",NULL,100 * Screen->hh,X,Y + dy + dy, 400 * Screen->ww,100 * Screen->hh,ttc);
	}
	else
	{
		drawText("   Play   ",NULL,100 * Screen->hh,X,Y, 400 * Screen->ww,100 * Screen->hh,ttc1);
		drawText("High Score",NULL,100 * Screen->hh,X,Y + dy, 400 * Screen->ww,100 * Screen->hh,ttc1);
		drawText("   Quit   ",NULL,100 * Screen->hh,X,Y + dy + dy, 400 * Screen->ww,100 * Screen->hh,ttc1);
	}

	drawPlayers();

	flushall();
	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_GraphicdrawScore(int T[10][2],int y)
{
	int X = Screen->w / 2 - 200 * Screen->ww;
	int Y = 0 * Screen->hh;

	int dy = 80 * Screen->hh;
	char tmp[1024];

	SDL_SetRenderDrawColor(Screen->renderer,0x00,0x00,0x00,200);
	SDL_RenderClear(Screen->renderer);
	SDL_Color ttc = {0xD0,0xD0,0xa0};
	SDL_Color ttc0 = {0xff,0xff,0xff};
	SDL_Color ttc1 = {0xff,0xff,0x00};

	drawBG(320,200);

	if(y == 0)
	{
		drawText("   Back",NULL,100 * Screen->hh,X,Y, 400 * Screen->ww,100 * Screen->hh,ttc0);
	}
	else
	{
		drawText("   Back",NULL,100 * Screen->hh,X,Y, 400 * Screen->ww,100 * Screen->hh,ttc1);
	}

	Y += dy * 2;
	X = Screen->w / 2 - 500 * Screen->ww;
	int i;
	for(i = 0; i < 10; ++i)
	{
		char dd[128];
		epochtodate(dd,128,T[i][0]);
		sprintf(tmp,"#%d    %s  --------------------- %d",i+1,dd,T[i][1]);
		drawText(tmp,NULL,80 * Screen->hh,X,Y + i * dy, 800 * Screen->ww,80 * Screen->hh,ttc);
	}

	drawPlayers();

	flushall();
	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_GraphicdrawLogo(int t)
{
	if(Screen->logo == NULL)
		return;

	SDL_Rect dest,src;

	dest.w = 600 * Screen->ww;
	dest.h = 400 * Screen->hh;
	dest.x =  Screen->w/2 - dest.w /2;
	dest.y =  Screen->h/2 - dest.h /2;

	src.x = 0;
	src.y = 0;
	src.w = 0;
	src.h = 100;

	while(src.w < 368)
	{
		SDL_SetRenderDrawColor(Screen->renderer,0x00,0x00,0x00,200);
		SDL_RenderClear(Screen->renderer);
		SDL_RenderCopyEx(Screen->renderer,Screen->logo,&src,&dest,0,0,SDL_FLIP_NONE);
		src.w += 10;
		flushall();
	}

	SDL_SetRenderDrawColor(Screen->renderer,0x00,0x00,0x00,200);
	SDL_RenderClear(Screen->renderer);
	SDL_RenderCopyEx(Screen->renderer,Screen->logo,&src,&dest,0,0,SDL_FLIP_NONE);
	flushall();
	SDL_Delay(t);

	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_GraphicdrawOut(int t)
{
	if(Screen->logo == NULL)
		return;

	SDL_Rect dest,src;

	dest.w = 600 * Screen->ww;
	dest.h = 400 * Screen->hh;
	dest.x =  Screen->w/2 - dest.w /2;
	dest.y =  Screen->h/2 - dest.h /2;

	src.x = 0;
	src.y = 0;
	src.w = 368;
	src.h = 100;

	SDL_SetRenderDrawColor(Screen->renderer,0x00,0x00,0x00,200);
	SDL_RenderClear(Screen->renderer);

	SDL_RenderCopyEx(Screen->renderer,Screen->logo,&src,&dest,0,0,SDL_FLIP_NONE);
	flushall();
	SDL_Delay(t);


	while(src.w > 0)
	{
		SDL_SetRenderDrawColor(Screen->renderer,0x00,0x00,0x00,200);
		SDL_RenderClear(Screen->renderer);
		SDL_RenderCopyEx(Screen->renderer,Screen->logo,&src,&dest,0,0,SDL_FLIP_NONE);
		src.w -= 10;
		flushall();
	}
	SDL_SetRenderDrawColor(Screen->renderer,0x00,0x00,0x00,200);
	SDL_RenderClear(Screen->renderer);
	flushall();
	SDL_Delay(1000);


	return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

/************* static functions ***************/
static void flushall()
{
	SDL_RenderPresent(Screen->renderer);
	SDL_Delay(1);
}

/*--------------------------------------------------------------------------------*/
static void drawPlayers()
{
	Agent *a = BKP_Players->first;

	for(; a != NULL ;a = a->right) 
	{
		drawAgent(a->aimg);
	}

	return;
}

/*--------------------------------------------------------------------------------*/
static void drawPlan(int plan)
{
	LAimg * ll = BKP_AimgPlan[plan];
	Aimg *a = ll->first;

	for(; a != NULL ;a = a->right) 
	{
		drawAgent(a);
	}

	return;
}

/*--------------------------------------------------------------------------------*/
static void drawAgent(Aimg *a)
{
	if(a->type == OBJ_TYPE_TEXT)
	{
		drawText(a->sz,NULL,a->h,a->pos.x,a->pos.y ,a->w ,a->h,a->ttc);
	}

	if(a == NULL || a->sprite_sheet == NULL || a->visible == BKP_FALSE)
		return;

	SDL_Rect dest,
			 src;

	src.x = a->sprite_col * a->sprite_w;
	src.y = a->sprite_row * a->sprite_h;
	src.w = a->sprite_w;
	src.h = a->sprite_h;

	dest.x = a->pos.x;
	dest.y = a->pos.y;
	dest.w = a->w;
	dest.h = a->h;

	if(a->alpha > -1)
		SDL_SetTextureAlphaMod(a->sprite_sheet,a->alpha);

	if(a->RotatecenterisNull == 0)
		SDL_RenderCopyEx(Screen->renderer,a->sprite_sheet,&src,&dest,a->angle,NULL,SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(Screen->renderer,a->sprite_sheet,&src,&dest,a->angle,&a->center,SDL_FLIP_NONE);

	if(a->alpha > -1)
		SDL_SetTextureAlphaMod(a->sprite_sheet,255);

	if(a->text == BKP_TRUE)
	{
		char tmp[256];
		SDL_Color ttc = {0xff,0xff,0X10};

		sprintf(tmp,"%d",(int)a->coins);
		drawText(tmp,NULL,10 * Screen->hh,a->pos.x,Screen->h - 50 * Screen->hh ,a->w - 30 * Screen->ww,30 * Screen->hh,ttc);
		//drawText(tmp,NULL,10 * Screen->hh,a->pos.x,a->pos.y,-1,-1,ttc);
	}
	
	return;
}

/*--------------------------------------------------------------------------------*/
static void drawHud()
{
	SDL_Rect dest,
			 src;
	
	src.x = Hud->sprite_x; 
	src.y = Hud->sprite_y;
	src.w = Hud->sprite_w;
	src.h = Hud->sprite_h;

	dest.x = Hud->pos.x;
	dest.y = Hud->pos.y;
	dest.w = Hud->w;
	dest.h = Hud->h;

	SDL_RenderCopyEx(Screen->renderer,Hud->sprite_sheet,&src,&dest,0,0,SDL_FLIP_NONE);

	drawPlan(BKP_MAX_PLAN);
	drawPlan(BKP_MAX_PLAN + 1);

	char tmp[256];
	SDL_Color ttc = {0xFA,0xA0,0X90};
	int w = 20 * Screen->ww;

	sprintf(tmp,"Money : $%d ",(int)(BKP_Players->first->coins));
	ttc.r = 0xFF;
	ttc.g = 0xFF;
	ttc.b = 0x00;
	drawText(tmp,NULL,w,Screen->w - 24 * w,Hud->pos.y + 20 * Screen->hh ,-1,40 * Screen->hh,ttc);
	sprintf(tmp,"Score : %d",(int)(BKP_Players->first->score));
	ttc.r = 0x40;
	ttc.b = 0xFF;
	drawText(tmp,NULL,w,Screen->w - 24 * w,Hud->pos.y + 70 * Screen->hh ,-1,40 * Screen->hh,ttc);
	int pv =  BKP_Players->first->PV > 1 ? (int)((BKP_Players->first->PV  - 1)* 100 / 4) : 0;
	sprintf(tmp,"Shield : %d %%",pv);
	ttc.r = 0xFF;
	ttc.b = 0x40;
	ttc.g = 0x40;
	drawText(tmp,NULL,w,Screen->w - 24 * w,Hud->pos.y + 110 * Screen->hh ,-1,40 * Screen->hh,ttc);

	return;
}

/*--------------------------------------------------------------------------------*/
static void drawText(const char *mess,TTF_Font * font,int font_size,int x,int y,int w,int h,SDL_Color textcolor)
{
	if(mess == NULL)
		return;

	SDL_Surface *soe = NULL;

	if(font != NULL)
	{
		soe = TTF_RenderUTF8_Solid(font,mess,textcolor);
	}
	else
	{
		TTF_Font  * tf = TTF_OpenFont("data/font/DejaVuSans-BoldOblique.ttf",font_size);
		soe = TTF_RenderUTF8_Solid(tf,mess,textcolor);
		TTF_CloseFont(tf);
	}
	SDL_Texture * message = SDL_CreateTextureFromSurface(Screen->renderer,soe);
	SDL_FreeSurface(soe);

	if(message != NULL)
	{
		SDL_Rect dest;
		dest.x = x;
		dest.y = y;

		SDL_QueryTexture(message,NULL,NULL,&dest.w,&dest.h);
		if(h >= 0)
			dest.h = h;
		if(w >= 0)
			dest.w = w;
		SDL_RenderCopy(Screen->renderer,message,NULL,&dest);
	}

	SDL_DestroyTexture(message);

	return;
}

/*--------------------------------------------------------------------------------*/
static void drawBG(int w,int h)
{
	if(Screen->bg == NULL)
		return;
	SDL_Rect dest,src;

	dest.x = 0;
	dest.y = 0;
	dest.w = Screen->w;
	dest.h = Screen->h;

	src.x = 0;
	src.y = 0;

	if(w <= 0)
		src.w = 1131;
	else 
		src.w = w;
	if(h <= 0)
		src.h = 707;
	else
		src.h = h;

	SDL_RenderCopyEx(Screen->renderer,Screen->bg,&src,&dest,0,0,SDL_FLIP_NONE);
	return;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

