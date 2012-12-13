#include "game_of_life.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#define ALIVE	1
#define DEAD	0
/*MACROS finding cell*/
#define __J(gol_i, j)		(((j) >= ((gol_i)->w)) ? (0) : ((j < 0) ? (((gol_i)->w) - 1) : (j)))
#define __I(gol_i, i)		(((i) >= ((gol_i)->h)) ? (0) : ((i < 0) ? (((gol_i)->h) - 1) : (i)))
#define CELL(gol_i, i, j) 	(((gol_i)->cells)+((((gol_i)->w)*(i)+(j))))
#define CELL_SAFE(gol_i, i, j) 	(((gol_i)->cells)+((((gol_i)->w)*(__I(gol_i, i)))+(__J(gol_i, j))))
/*MACROS queries for cell information*/
#define CELL_7_2(cell)			((*(cell)) >> 2)
#define CELL_1(cell)			(((*(cell)) & 0x02) >> 1)
#define	CELL_0(cell)			((*(cell)) & 0x01)
#define	CELL_SET_7_2(cell, neigh)	(*(cell)) = (((*(cell)) & 0x03) | ((neigh) << 2))
#define CELL_SET_1(cell, isalive)	(*(cell)) = (((*(cell)) & 0xFD) | ((isalive) << 1))
#define	CELL_SET_0(cell, isalive)	(*(cell)) = (((*(cell)) & 0xFE) | (isalive))
#define CELL_ALIVE(cell, gol_i)		((((gol_i)->tick) == 0) ? (CELL_0(cell)) : (CELL_1(cell)))
#define CELL_ALIVE_PAST(cell, gol_i)	((((gol_i)->tick) == 0) ? (CELL_1(cell)) : (CELL_0(cell)))
#define CELL_SET_ALIVE(cell, isalive, gol_i)	\
		((((gol_i)->tick) == 0) ? (CELL_SET_1(cell, isalive)) : (CELL_SET_0(cell, isalive)))
#define	CELL_INC(cell)			CELL_SET_7_2(cell, (CELL_7_2(cell) + 1))
#define CELL_RESET(cell)		CELL_SET_7_2(cell, 0)
#define CELL_SET(cell, num)		CELL_SET_7_2(cell, num)
#define CELL_NEIGH(cell)		CELL_7_2(cell)
/*MACRO game of life manipulation*/
#define RESET_ALL_CELLS(gol_i)		(bzero(gol_i->cells, sizeof(unsigned char)*(gol_i->w)*(gol_i->h)))
static int num_offsets = 8;
static int neighboor_offsets[8][2] = 	{{-1, -1},	// 0
					{-1,  0},	// 1
					{-1,  1},	// 2
					{ 0, -1},	// 3
					{ 0,  1},	// 4
					{ 1, -1},	// 5
					{ 1,  0},	// 6
					{ 1,  1}};	// 7

inline void cell_live_or_die(gol* gol_i, int i, int j);
inline void cell_glDisplay(gol* gol_i, int i, int j);
inline void cell_display(gol* gol_i, int i, int j);

gol* gol_begin(unsigned int w, unsigned int h)
{
	gol* gol_i = (gol*)malloc(sizeof(gol));
	gol_i->cells = (char*)malloc(sizeof(char)*w*h);
	gol_i->w = w;
	gol_i->h = h;
	gol_i->tick = 0;
	RESET_ALL_CELLS(gol_i);
	return gol_i;	
}
void gol_step(gol* gol_i)
{
	int i, j;
	for(i = 0; i < gol_i->h; i++)
	{
		for(j = 0; j < gol_i->w; j++)
		{
			cell_live_or_die(gol_i, i, j);					
		}
	}	
	gol_i->tick = (gol_i->tick + 1) % 2;
}
void gol_glDisplay(gol* gol_i)
{
	int i, j;
	for(i = 0; i < gol_i->h; i++)
	{
		for(j = 0; j < gol_i->w; j++)
		{
			cell_glDisplay(gol_i, i, j);	
		}
	}	
}
void gol_display(gol* gol_i)
{ 
	int i, j;
	for(i = 0; i < gol_i->h; i++)
	{
		printf("[");
		for(j = 0; j < gol_i->w; j++)
		{
			cell_display(gol_i, i, j);					
		}
		printf("]\n");
	}	
}
void gol_free(gol* gol_i)
{
	free(gol_i->cells);
	free(gol_i);
}
inline void cell_display(gol* gol_i, int i, int j)
{
	unsigned char *cell = CELL(gol_i, i, j);
	printf(" (%d,%d)", CELL_NEIGH(cell), CELL_ALIVE_PAST(cell, gol_i));
}
float cube_w = 5.0f / 100.0f;
inline void cell_glDisplay(gol* gol_i, int i, int j)
{
	unsigned char *cell = CELL(gol_i, i, j);
	if(CELL_ALIVE_PAST(cell, gol_i))
	{
		float i_f = (float)i;
		float j_f = (float)j;
		glPushMatrix();
		glColor3f((GLfloat)(((GLfloat)CELL_NEIGH(cell))/8.0f),(GLfloat)0.0,(GLfloat)0.0);
		glTranslatef((i_f*cube_w) - 2.5, 0, -1.0*((j_f*cube_w) - 2.5));
		glutSolidCube((double)cube_w);
		glPopMatrix();
	}
}
inline void cell_live_or_die(gol* gol_i, int i, int j)
{
	unsigned char *cell = CELL(gol_i, i, j);
	unsigned char n;
	unsigned char num_neighboors = 0;
	for(n = 0; n < 8; n++)
	{
		int i_pos = i+neighboor_offsets[n][0];
		int j_pos = j+neighboor_offsets[n][1];
		unsigned char *n_cell = CELL_SAFE(gol_i, i_pos, j_pos);
		num_neighboors += CELL_ALIVE(n_cell, gol_i);
	}

	if(CELL_ALIVE(cell, gol_i))
		if(num_neighboors < 2)
			CELL_SET_ALIVE(cell, DEAD, gol_i);
		else if(num_neighboors <= 3)
			CELL_SET_ALIVE(cell, ALIVE, gol_i);
		else
			CELL_SET_ALIVE(cell, DEAD, gol_i);
	else
		if(num_neighboors == 3)
			CELL_SET_ALIVE(cell, ALIVE, gol_i);	
		else
			CELL_SET_ALIVE(cell, DEAD, gol_i);
	CELL_SET(cell, num_neighboors);
}
