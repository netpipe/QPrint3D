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

#ifndef DOCKLAYERLIST_H_
#define DOCKLAYERLIST_H_

#include <QtGui/QDockWidget>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <string>
#include <iostream>
#include <vector>
#include "GCodeInterpreter.h"
#include "Helper.hpp"
#include <QWidget>

using namespace std;

class DockLayerList : public QDockWidget
{
	Q_OBJECT
public:
	DockLayerList(const QString & title, GCodeInterpreter& interpreter, QWidget * parent = 0, Qt::WindowFlags flags = 0);
	virtual ~DockLayerList();
	
	void refresh();
	
private:
	GCodeInterpreter& interpreter;
	
	QListWidget* layerList;
	QPushButton* btClear;
	QPushButton* btSet;
	
protected:
	void resizeEvent(QResizeEvent*);
	void closeEvent(QCloseEvent*);
	
private slots:
	void onSelectionChanged();
	void onButtonSet();
	void onButtonClear();

signals:
	void selectionChanged();
	void closed();
};

#endif /* DOCKLAYERLIST_H_ */
