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

#ifndef DOCKOPENGL_H_
#define DOCKOPENGL_H_

#include <QtGui/QDockWidget>
#include "GLDrawer.h"
#include <QWidget>

class DockOpenGL : public QDockWidget
{
	Q_OBJECT
public:
	DockOpenGL(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0);
	void loadGCode(string fileName);
	
protected:
	void resizeEvent(QResizeEvent* event);
	
private:
	GLDrawer* drawer;
};

#endif /* DOCKOPENGL_H_ */
