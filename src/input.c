#include <stdlib.h>
#include <stdio.h>

/*Copyright*/
/*
 * AUTHOR: lilington
 * addr	: lilington@yahoo.fr
 *
 */

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


/**************************************************************************
*	Implementations	
**************************************************************************/

void bkp_InputInit(BKP_Input *input)
{
	input->keypressed = 0;
	input->Start = input->Cancel = 0;
	bkp_resetkeys(input);

	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_resetkeys(BKP_Input *p)
{
	p->Action0.pressed = 0; p->Action0.s_time = 0;
	p->Action1.pressed = 0; p->Action1.s_time = 0;
	p->Action2.pressed = 0; p->Action2.s_time = 0;
	p->Action3.pressed = 0; p->Action3.s_time = 0;

	p->Gachette0.pressed = 0; p->Gachette0.s_time = 0;
	p->Gachette1.pressed = 0; p->Gachette1.s_time = 0;
	p->Gachette2.pressed = 0; p->Gachette2.s_time = 0;
	p->Gachette3.pressed = 0; p->Gachette3.s_time = 0;

	p->left.pressed = 0; p->left.s_time = 0;
	p->right.pressed = 0; p->right.s_time = 0;
	p->up.pressed = 0; p->up.s_time = 0;
	p->down.pressed = 0; p->down.s_time = 0;
	p->Cancel = 0;

	p->Mouse.l_click.pressed = p->Mouse.l_click.s_time = 0;
	p->Mouse.r_click.pressed = p->Mouse.r_click.s_time = 0;
	return;
}

/*--------------------------------------------------------------------------------*/
void bkp_InputCapture(BKP_Input * p)
{
	SDL_Event event;
	unsigned int ticks = SDL_GetTicks();

	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT)
		{
			p->Cancel = 1;
			return;
		}
		if(event.type == SDL_MOUSEMOTION)
		{
			p->Mouse.x = event.motion.x;
			p->Mouse.y = event.motion.y;
		}
		if(event.type == SDL_MOUSEBUTTONDOWN)
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(p->Mouse.l_click.pressed == 0)
					p->Mouse.l_click.s_time = ticks;

				if(ticks - p->Mouse.l_click.s_time < BKP_INPUT_PRESSURE_TIMER)
					p->Mouse.l_click.pressed = 1;
				else
					p->Mouse.l_click.pressed = 2;
			}
			if(event.button.button == SDL_BUTTON_RIGHT)
			{
				if(p->Mouse.r_click.pressed == 0)
					p->Mouse.r_click.s_time = ticks;

				if(ticks - p->Mouse.r_click.s_time < BKP_INPUT_PRESSURE_TIMER)
					p->Mouse.r_click.pressed = 1;
				else
					p->Mouse.r_click.pressed = 2;
			}
		}
		if(event.type == SDL_MOUSEBUTTONUP)
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				p->Mouse.l_click.pressed = 0;
			}
			if(event.button.button == SDL_BUTTON_RIGHT)
			{
				p->Mouse.r_click.pressed = 0;
			}
		}
	}
	//SDL_PumpEvents();
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if(state[SDL_SCANCODE_RIGHT])
	{
		if(p->right.pressed == 0)
			p->right.s_time = ticks;

		if(ticks - p->right.s_time < BKP_INPUT_PRESSURE_TIMER)
			p->right.pressed = 1;
		else
			p->right.pressed = 2;
	}
	else if(p->right.pressed > 0)
	{
		p->right.pressed = 0;
		p->right.s_time = 0;
	}

	if(state[SDL_SCANCODE_LEFT])
	{
		if(p->left.pressed == 0)
			p->left.s_time = ticks;

		if(ticks - p->left.s_time < BKP_INPUT_PRESSURE_TIMER)
			p->left.pressed = 1;
		else
			p->left.pressed = 2;
	}
	else if(p->left.pressed > 0)
	{
		p->left.pressed = 0;
		p->left.s_time = 0;
	}

	if(state[SDL_SCANCODE_UP])
	{
		if(p->up.pressed == 0)
			p->up.s_time = ticks;

		if(ticks - p->up.s_time < BKP_INPUT_PRESSURE_TIMER)
			p->up.pressed = 1;
		else
			p->up.pressed = 2;
	}
	else if(p->up.pressed > 0)
	{
		p->up.pressed = 0;
		p->up.s_time = 0;
	}

	if(state[SDL_SCANCODE_DOWN])
	{
		if(p->down.pressed == 0)
			p->down.s_time = ticks;

		if(ticks - p->down.s_time < BKP_INPUT_PRESSURE_TIMER)
			p->down.pressed = 1;
		else
			p->down.pressed = 2;
	}
	else if(p->down.pressed > 0)
	{
		p->down.pressed = 0;
		p->down.s_time = 0;
	}

	if(state[SDL_SCANCODE_ESCAPE])
	{
			p->Cancel = 1;
			return;
	}
	return;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
