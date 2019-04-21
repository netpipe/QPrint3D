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

#include "DockLayerList.h"

DockLayerList::DockLayerList(const QString & title, GCodeInterpreter& interpreter, QWidget * parent, Qt::WindowFlags flags) :
	QDockWidget(title, parent, flags),
	interpreter(interpreter)
{
	layerList = new QListWidget(this);
	layerList->setGeometry(0, 60, 100, 500);
	layerList->setAutoScroll(true);
	layerList->setSelectionMode(QAbstractItemView::MultiSelection);
	
	btClear = new QPushButton(tr("Clear"), this);
	btClear->setGeometry(10, 30, 30, 15);
	connect(btClear, SIGNAL(clicked()), this, SLOT(onButtonClear()));

	btSet = new QPushButton(tr("Set"), this);
	btSet->setGeometry(50, 30, 30, 15);
	connect(btSet, SIGNAL(clicked()), this, SLOT(onButtonSet()));
	
	connect(layerList, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChanged()));
}

DockLayerList::~DockLayerList()
{
	
}

void DockLayerList::refresh()
{
	// make shure no event is triggered during update
	disconnect(layerList, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChanged()));
	layerList->clear();
	int maxLayer=0;
	vector<GCodeLine>& gCodeLines = interpreter.getCodeLines();
	for(unsigned int i=0;  i<gCodeLines.size(); i++)
	{
		if(gCodeLines[i].layer>maxLayer)
			maxLayer=gCodeLines[i].layer;
	}
	for(int i=0; i<=maxLayer; i++)
	{
		layerList->addItem(QString::number(i));
		layerList->item(layerList->count()-1)->setSelected(true);
	}
	
	// connect the event again
	connect(layerList, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChanged()));
}

void DockLayerList::resizeEvent(QResizeEvent*)
{
	layerList->resize(width(), height()-70);
	btClear->setGeometry(5, 30, (width()-15)/2, 20);
	btSet->setGeometry(10+(width()-15)/2, 30, (width()-15)/2, 20);
}

void DockLayerList::onSelectionChanged()
{
	vector<GCodeLine>& gCodeLines = interpreter.getCodeLines();
	for(unsigned int i=0; i<gCodeLines.size(); i++)
	{
		int layer=gCodeLines[i].layer;
		if(layer>=layerList->count())
		{
			cerr<<"There is something wrong with the number of layers in the layer view"<<endl;
			cerr<<"The used layer "<<layer<<" is missing..."<<endl;
			cerr<<"This should never happen, if you see this warning there is a problem "
					<<"with the source code."<<endl;
			// TODO: Add it? Ignore it?
			continue;
		}
///		gCodeLines[i].visible=layerList->item(layer)->isSelected();
	}
	emit selectionChanged();
}

void DockLayerList::onButtonSet()
{
	// make shure no event is triggered during update
	disconnect(layerList, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChanged()));
	for(int i=0; i<layerList->count(); i++)
	{
		layerList->item(i)->setSelected(true);
	}
	// connect the event again
	connect(layerList, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChanged()));
	onSelectionChanged();
}

void DockLayerList::onButtonClear()
{
	// make shure no event is triggered during update
	disconnect(layerList, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChanged()));
	for(int i=0; i<layerList->count(); i++)
	{
		layerList->item(i)->setSelected(false);
	}
	// connect the event again
	connect(layerList, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChanged()));
	onSelectionChanged();
}

void DockLayerList::closeEvent(QCloseEvent*)
{
	emit closed();
}

