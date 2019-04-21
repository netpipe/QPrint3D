/*
 * GLDrawer.h
 *
 *  Created on: 27.11.2010
 *      Author: fabian
 */

#ifndef GLDRAWER_H_
#define GLDRAWER_H_

#include <QtOpenGL/QGLWidget>
#include <QtGui/QMouseEvent>
#include <QWidget>

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

#include <QtGui/QWheelEvent>
#include <iostream>
#include "GCodeInterpreter.h"

using namespace std;

class GLDrawer : public QGLWidget
{
	Q_OBJECT
public:
	GLDrawer(QWidget* parent=NULL);
	~GLDrawer();
	
	void loadGCode(string fileName);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);

private:
    void drawBox();
    
    QPoint lastMousePosition;
    bool midMousePressed;
    bool leftMousePressed;
    bool rightMousePressed;
    int transX, transY, transZ;
    int rotX, rotY, rotZ;

    GCodeInterpreter interpreter;
};

#endif /* GLDRAWER_H_ */
