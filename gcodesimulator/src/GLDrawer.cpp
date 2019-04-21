/*
Copyright 2010 by Fabian Schwartau
E-Mail: fabian@opencode.eu

This file is part of RepRapGCodeSimulator.

RepRapGCodeSimulator is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RepRapGCodeSimulator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BoostComPort.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GLDrawer.h"
#include <GL/glu.h>
#include <GL/gl.h>

GLDrawer::GLDrawer(QWidget* parent) :
	QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
	lastMousePosition(0,0),
	midMousePressed(false),
	leftMousePressed(false),
	transX(0),
	transY(0),
	transZ(-160),
	rotX(0),
	rotY(0),
	rotZ(0)
{
	setMouseTracking(true);
	resize(800, 600);
}

GLDrawer::~GLDrawer()
{

}

void GLDrawer::initializeGL()
{
	//cout<<"Init..."<<endl;
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glClearColor(0, 0, 0, 255);
}

void GLDrawer::resizeGL(int width, int height)
{
	//cout<<"resize..."<<endl;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(0, width, 0, height); // set origin to bottom left corner
    gluPerspective(90, (double)width/(double)height, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLDrawer::paintGL()
{
	//cout<<"Repaint..."<<endl;
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
   	glTranslatef(((float)transX)/7.0, ((float)transY)/7.0, (float)transZ);
    glRotatef(-rotX, 1, 0, 0);
    glRotatef(-rotY, 0, 1, 0);
    
    float dx=(interpreter.getMaxX()+interpreter.getMinX())/2.0;
    float dy=(interpreter.getMaxY()+interpreter.getMinY())/2.0;
    float dz=(interpreter.getMaxZ()+interpreter.getMinZ())/2.0;
    glColor3f(1,0,0);
    glBegin(GL_LINES);
		glVertex3f(0,0,-100);
		glVertex3f(0,0,100);
    glEnd();
    glBegin(GL_LINES);
		glVertex3f(0,-100,0);
		glVertex3f(0,100,0);
    glEnd();
    glBegin(GL_LINES);
		glVertex3f(-100,-0,0);
		glVertex3f(100,0,0);
    glEnd();
    glTranslatef(-dx, -dy, -dz);
    glColor3f(0,0,1);
    
    if(!interpreter.isOpen())
    {
    	glPopMatrix();
    	return;
    }
    drawBox();

    glColor3f(0,1,0);
    
    glBegin(GL_LINES);
		glVertex3f(0,0,-20);
		glVertex3f(0,0,20);
    glEnd();
    glBegin(GL_LINES);
		glVertex3f(0,-20,0);
		glVertex3f(0,20,0);
    glEnd();
    glBegin(GL_LINES);
		glVertex3f(-20,-0,0);
		glVertex3f(20,0,0);
    glEnd();
    
    glColor3f(1,1,0);
    
    vector<GCodeLine>& codeLines=interpreter.getCodeLines();
    float oldx=0, oldy=0, oldz=0;
    for(unsigned int i=0; i<codeLines.size(); i++)
    {
    	if(codeLines[i].command=="G1")  // draw a line
    	{
    		float x=oldx, y=oldy, z=oldz;
    		bool hasE=false;
    		for(unsigned int j=0; j<codeLines[i].parameters.size(); j++)
    		{
    			if(codeLines[i].parameters[j].name=='E')
    				hasE=true;
    		}
    		for(unsigned int j=0; j<codeLines[i].parameters.size(); j++)
    		{
				if(codeLines[i].parameters[j].name=='X')
					x=codeLines[i].parameters[j].value;
				if(codeLines[i].parameters[j].name=='Y')
					y=codeLines[i].parameters[j].value;
				if(codeLines[i].parameters[j].name=='Z')
					z=codeLines[i].parameters[j].value;
    		}
    		if(hasE)
    		{
				glBegin(GL_LINES);
					glVertex3f(oldx, oldy, oldz);
					glVertex3f(x, y, z);
				glEnd();
    		}
    		oldx=x;
    		oldy=y;
    		oldz=z;
    	}
    }
    
    glPopMatrix();
}

void GLDrawer::drawBox()
{
    glBegin(GL_LINES);
		// vorne
		glVertex3f(interpreter.getMinX(),interpreter.getMinY(),interpreter.getMinZ());
		glVertex3f(interpreter.getMinX(),interpreter.getMinY(),interpreter.getMaxZ());

		glVertex3f(interpreter.getMinX(),interpreter.getMinY(),interpreter.getMaxZ());
		glVertex3f(interpreter.getMaxX(),interpreter.getMinY(),interpreter.getMaxZ());

		glVertex3f(interpreter.getMaxX(),interpreter.getMinY(),interpreter.getMaxZ());
		glVertex3f(interpreter.getMaxX(),interpreter.getMinY(),interpreter.getMinZ());

		glVertex3f(interpreter.getMaxX(),interpreter.getMinY(),interpreter.getMinZ());
		glVertex3f(interpreter.getMinX(),interpreter.getMinY(),interpreter.getMinZ());

		// hinten
		glVertex3f(interpreter.getMinX(),interpreter.getMaxY(),interpreter.getMinZ());
		glVertex3f(interpreter.getMinX(),interpreter.getMaxY(),interpreter.getMaxZ());

		glVertex3f(interpreter.getMinX(),interpreter.getMaxY(),interpreter.getMaxZ());
		glVertex3f(interpreter.getMaxX(),interpreter.getMaxY(),interpreter.getMaxZ());

		glVertex3f(interpreter.getMaxX(),interpreter.getMaxY(),interpreter.getMaxZ());
		glVertex3f(interpreter.getMaxX(),interpreter.getMaxY(),interpreter.getMinZ());

		glVertex3f(interpreter.getMaxX(),interpreter.getMaxY(),interpreter.getMinZ());
		glVertex3f(interpreter.getMinX(),interpreter.getMaxY(),interpreter.getMinZ());

		// seiten
		glVertex3f(interpreter.getMinX(),interpreter.getMinY(),interpreter.getMinZ());
		glVertex3f(interpreter.getMinX(),interpreter.getMaxY(),interpreter.getMinZ());

		glVertex3f(interpreter.getMaxX(),interpreter.getMinY(),interpreter.getMinZ());
		glVertex3f(interpreter.getMaxX(),interpreter.getMaxY(),interpreter.getMinZ());

		glVertex3f(interpreter.getMinX(),interpreter.getMinY(),interpreter.getMaxZ());
		glVertex3f(interpreter.getMinX(),interpreter.getMaxY(),interpreter.getMaxZ());

		glVertex3f(interpreter.getMaxX(),interpreter.getMinY(),interpreter.getMaxZ());
		glVertex3f(interpreter.getMaxX(),interpreter.getMaxY(),interpreter.getMaxZ());
		
    glEnd();
}

void GLDrawer::mouseReleaseEvent(QMouseEvent* event)
{
	//cout<<"Mouse released"<<endl;
	if(event->button()==Qt::LeftButton)
		leftMousePressed=false;
	else if(event->button()==Qt::MidButton)
		midMousePressed=false;
	else if(event->button()==Qt::RightButton)
		rightMousePressed=false;
}

void GLDrawer::mousePressEvent(QMouseEvent* event) {
	//cout<<"Mouse pressed"<<endl;
	lastMousePosition=event->pos();
	if(event->button()==Qt::LeftButton)
		leftMousePressed=true;
	else if(event->button()==Qt::MidButton)
		midMousePressed=true;
	else if(event->button()==Qt::RightButton)
		rightMousePressed=true;
}
void GLDrawer::mouseMoveEvent(QMouseEvent* event) {
	//cout<<transX<<"/"<<transY<<endl;
    if(midMousePressed)
    {
		transX+=-(lastMousePosition.x()-event->x());
		transY+=lastMousePosition.y()-event->y();
		updateGL();
    }
    if(leftMousePressed)
    {
    	rotX+=(lastMousePosition.y())-event->y();
    	rotY+=(lastMousePosition.x())-event->x();
    	//cout<<"rotY: "<<rotY<<endl;
		updateGL();
    }
	lastMousePosition=event->pos();
}

void GLDrawer::wheelEvent(QWheelEvent* event)
{
	transZ+=event->delta()/10.0;
	//cout<<"transZ: "<<transZ<<endl;
	updateGL();
}

void GLDrawer::loadGCode(string fileName)
{
	interpreter.clear();
	interpreter.open(fileName);
}
