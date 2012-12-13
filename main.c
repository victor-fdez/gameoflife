#include <GL/glut.h>
#include <stdio.h>
#include <unistd.h>
#include "game_of_life.h"
#define CELL(gol_i, i, j) 	(((gol_i)->cells)+((((gol_i)->w)*(i)+(j))))
#define CELL_SET_1(cell, isalive)	(*(cell)) = (((*(cell)) & 0xFD) | ((isalive) << 1))
#define	CELL_SET_0(cell, isalive)	(*(cell)) = (((*(cell)) & 0xFE) | (isalive))
#define CELL_SET_ALIVE(cell, isalive, gol_i)	\
		((((gol_i)->tick) == 0) ? (CELL_SET_0(cell, isalive)) : (CELL_SET_1(cell, isalive)))
gol* gol_game;
void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gol_glDisplay(gol_game);
	glutSwapBuffers();
} /*END display()*/


void idle() {
	gol_step(gol_game);
	usleep(200000);
	glutPostRedisplay();
} /*END idle()*/


static GLfloat lightPos[] = {0, -1, 0, 0};
static GLfloat specMat[] = {1,1,1,1};
static GLfloat shnyMat[] = { 50 };
static GLfloat whiteLight[] = { 1, 1, 1, 1};
static GLfloat ambLight[] = { 0.51, 0.51, 0.51, 1.0};
void gl_init()
{
	
	/*SET UP PROJECTION AND MODELVIEW MATRICES*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-1, -1, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	/*SETTINGS*/	
	glEnable(GL_DEPTH_TEST);
	/*SET LIGHTING SOURCES*/
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

} /*END gl_init()*/

int main(int argc, char *argv[]) 
{
	int i;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(750, 750);
	glutInitWindowPosition(300, 200);
	glutCreateWindow("fluid simulation");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	gl_init();
	gol_game = gol_begin(100,100);
	for(i = 0; i < 100; i++)
	{
		unsigned char *cell = CELL(gol_game, i, i); 	
		CELL_SET_ALIVE(cell, 1, gol_game);
	}
	/*gol_display(gol_game);
	printf("\n");
	for(i = 0; i < 5; i++)
	{
		gol_step(gol_game);
		gol_display(gol_game);
		printf("\n");
	}*/
	glutMainLoop();
	gol_free(gol_game);
	return 0;
} /*END main() */
