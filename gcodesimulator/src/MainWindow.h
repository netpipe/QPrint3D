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

#ifndef QTOPENGL_H
#define QTOPENGL_H

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "GLDrawer.h"
#include "DockOpenGL.h"
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void loadGCode(string fileName);
private:
    Ui::QtOpenGLClass ui;
    DockOpenGL* GLDock;
    
private slots:
    void onBtLoadGCode();
};

#endif // QTOPENGL_H
