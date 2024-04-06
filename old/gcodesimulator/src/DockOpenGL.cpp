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

#include "DockOpenGL.h"

DockOpenGL::DockOpenGL(const QString & title, QWidget * parent, Qt::WindowFlags flags) :
	QDockWidget(title, parent, flags)
{
	drawer = new GLDrawer(this);
	drawer->setGeometry(0, 30, 100, 500);
}

void DockOpenGL::resizeEvent(QResizeEvent*)
{
	cout<<"Resized to: "<<this->width()<<"x"<<this->height()<<endl;
	drawer->resize(width(), height());
}

void DockOpenGL::loadGCode(string fileName)
{
	drawer->loadGCode(fileName);
}
