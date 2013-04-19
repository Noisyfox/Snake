#include <gl/glut.h>
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include "Snake.h"
#include "AI.h"

#define FPSCOUNT 60.0

//总可视区域大小
int width=WIDTH * PIXELUNIT + EDGE * 4 ;
int height=HEIGHT * PIXELUNIT + EDGE * 4 ;

CSnake snake;
CSnakeAI ai;

double CalFrequency()
{
	static int count;
	static double save;
	static clock_t last, current;
	double timegap;

	++count;
	if( count <= FPSCOUNT )
		return save;
	count = 0;
	last = current;
	current = clock();
	timegap = (current-last)/(double)CLK_TCK;
	save = FPSCOUNT/timegap;
	return save;
}

void myDisplay(void)
{
	double FPS = CalFrequency();
	printf("FPS = %f\n", FPS);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	snake.drawSnake();

	glFlush();
	glutSwapBuffers();
}

void keyboardEvent(int key, int x, int y){
	//switch(key){
	//case GLUT_KEY_UP:
	//	snake.moveTowards(DIR_UP);
	//	break;
	//case GLUT_KEY_DOWN:
	//	snake.moveTowards(DIR_DOWN);
	//	break;
	//case GLUT_KEY_LEFT:
	//	snake.moveTowards(DIR_LEFT);
	//	break;
	//case GLUT_KEY_RIGHT:
	//	snake.moveTowards(DIR_RIGHT);
	//	break;
	//}
}

void aiProcess(int value){
	time_t stime = time(0);
	bool result = snake.moveTowards(ai.AICallBack(snake));
	if(result){
		time_t dtime = time(0) - stime;
		if(dtime >= 300){
			glutTimerFunc(1, aiProcess, 0);
		}else{
			glutTimerFunc(300 - dtime, aiProcess, 0);
		}
	}
}

void fpsControl(int value){
	glutPostRedisplay();
	glutTimerFunc(1000.0/60,fpsControl,0);
}

void myIdle(void){
	Sleep(1);
}

void myReshape(int w,int h){
	double scale = min((double)h / height, (double)w / width);
	glViewport((w - width * scale)/2, (h - height * scale)/2, width * scale, height * scale);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,height,0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(){
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("贪吃蛇");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);
	glutSpecialFunc(keyboardEvent);
	glutTimerFunc(10,fpsControl,0);
	glutTimerFunc(1, aiProcess, 0);
	init();
	glutMainLoop();

	return 0;
}
