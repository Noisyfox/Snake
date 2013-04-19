#include "GLUtil.h"
#define _USE_MATH_DEFINES
#include <cmath>

GLvoid _uDrawCircle(GLint x, GLint y, GLfloat r, GLint segs, GLenum mode){
	if(segs < 3)return;

	const GLfloat coef = 2.0f * (GLfloat) (M_PI) /segs;
	
	glBegin(mode);
	for(GLint i=0;i<=segs;i++){
		GLfloat rads = i*coef;
		glVertex2f(r * cosf(rads) + x, r * sinf(rads) + y);
	}
	glEnd();
}

GLvoid uDrawCircle(GLint x, GLint y, GLfloat r, GLint segs){
	_uDrawCircle(x, y, r, segs, GL_LINE_LOOP);
}

GLvoid uDrawSolidCircle(GLint x, GLint y, GLfloat r, GLint segs){
	_uDrawCircle(x, y, r, segs, GL_TRIANGLE_FAN);
}
