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

#include "MainWindow.h"
#include <QtGui/QFileDialog>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	
	GLDock = new DockOpenGL(tr("OpenGL View"), this);
	GLDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
	GLDock->setFloating(false);
	addDockWidget(Qt::LeftDockWidgetArea, GLDock);
	GLDock->setMinimumWidth(400);
}

MainWindow::~MainWindow()
{
	delete GLDock;
}

void MainWindow::onBtLoadGCode()
{
	QString qFileName=QFileDialog::getOpenFileName(this, "Select a G-Code to interprete", ".", "*.*");
	if(qFileName=="")
		return;
	string fileName=qFileName.toStdString();
	GLDock->loadGCode(fileName);
}

void MainWindow::loadGCode(string fileName)
{
	GLDock->loadGCode(fileName);
}
