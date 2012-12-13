#ifndef __GAME_OF_LIFE_H__
#define __GAME_OF_LIFE_H__
/*game of life MACROS*/
typedef struct
{
	unsigned int   w, h;
	unsigned char *cells;
	unsigned char  tick;
} gol;
/*game of life FUNCTION definitions*/
extern gol* gol_begin(unsigned int w, unsigned int h);
extern void gol_step(gol*);
extern void gol_glDisplay(gol*);
extern void gol_display(gol*);
extern void gol_free(gol*);
#endif
