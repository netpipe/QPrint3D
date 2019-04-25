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

#include "GCodeInterpreter.h"
#include <fstream>
#include <iostream>
#include "Helper.hpp"

GCodeInterpreter::GCodeInterpreter()
{
	
}

GCodeInterpreter::~GCodeInterpreter()
{
	
}

int GCodeInterpreter::open(string fileName)
{
	currentLayer=0;
	lastZ=0;
	ifstream inFile(fileName.c_str(), ios::binary);
	while(!inFile.eof())
	{
		char lineBuffer[1000];
		inFile.getline(lineBuffer, 1000);
		string line=lineBuffer;
		addLine(line);
	}
	refreshMinMax();
	return 0;
}

void GCodeInterpreter::clear()
{
	codeLines.clear();
}

int GCodeInterpreter::addLine(string line)
{
	GCodeLine codeLine;
	codeLine.originalLine=line;
	string clearedLine=line;
	// search for comments and remove them
	size_t pos;
	if((pos=clearedLine.find(";"))!=string::npos)
		clearedLine=clearedLine.substr(0, pos);
	
	// remove leading whitespaces
	while(clearedLine.length() && (clearedLine[0]==' ' || clearedLine[0]=='\t'))
		clearedLine=clearedLine.substr(1, clearedLine.length()-1);
	
	// remove whitespaces at the end
	while(clearedLine.length() && (clearedLine[clearedLine.length()-1]==' ' || clearedLine[clearedLine.length()-1]=='\t'))
		clearedLine=clearedLine.substr(0, clearedLine.length()-1);
	
	// replace tabs with spaces
	replace(clearedLine, "\t", " ");
	
	// remove double spaces
	while(clearedLine.find("  ")!=string::npos)
		replace(clearedLine, "  ", " ");
	
	if(clearedLine.length()<=1) // nothing to execute
	{
		codeLine.interprete=false;
		codeLine.command="";
	}
	else
	{
		codeLine.interprete=true;
		
		vector<string> words;
		explode(words, clearedLine, " ");
		if(words.size()!=0)
		{
			codeLine.hasE=false;
			codeLine.command=words[0];
			for(unsigned int i=1; i<words.size(); i++)  // get each parameter
			{
				if(words[i].length()==0)
					continue;
				GCodeParameter parameter;
				parameter.name=words[i][0];
				string strValue=words[i].substr(1, words[i].length()-1);
				float value=Helper::String2float(strValue);
				parameter.value=value;
				if(parameter.name=='E')
					codeLine.hasE=true;
				if(parameter.name=='Z' && parameter.value!=lastZ)  // maybe the z value changed
				{
					lastZ=parameter.value;
					currentLayer++;
				}
				codeLine.parameters.push_back(parameter);
			}
			codeLine.layer=currentLayer;
		}
	}
	
	codeLine.clearedLine=clearedLine;
	codeLines.push_back(codeLine);
	return 0;
}

void GCodeInterpreter::refreshMinMax()
{
	minX=1000000.0;
	minY=1000000.0;
	minZ=1000000.0;
	maxX=-1000000.0;
	maxY=-1000000.0;
	maxZ=-1000000.0;
	float x=20,y=20,z=5;
	for(unsigned int i=0; i<codeLines.size(); i++)
	{
		bool hasE=false;
		bool hasXYZ=false;
		for(unsigned int j=0; j<codeLines[i].parameters.size(); j++)
		{
			switch(codeLines[i].parameters[j].name)
			{
			case 'X':
				x=codeLines[i].parameters[j].value;
				hasXYZ=true;
				break;
			case 'Y':
				y=codeLines[i].parameters[j].value;
				hasXYZ=true;
				break;
			case 'Z':
				z=codeLines[i].parameters[j].value;
				hasXYZ=true;
				break;
			case 'E':
				hasE=true;
				break;
			}
		}
		if(codeLines[i].command=="G1" && hasE && hasXYZ)
		{
			if(x<minX)
				minX=x;
			if(x>maxX)
				maxX=x;
			if(y<minY)
				minY=y;
			if(y>maxY)
				maxY=y;
			if(z<minZ)
				minZ=z;
			if(z>maxZ)
				maxZ=z;
		}
	}
	//cout<<"Min: "<<minX<<"/"<<minY<<"/"<<minZ<<endl;
	//cout<<"Max: "<<maxX<<"/"<<maxY<<"/"<<maxZ<<endl;
}

void GCodeInterpreter::replace(string& replace, string from, string to)
{
	size_t pos;
	while((pos=replace.find(from))!=string::npos)
		replace.replace(pos, from.length(), to);
}

void GCodeInterpreter::explode(vector<string> &res, string &list, string expression)
{
	if(list.length()<1)
		return;
	size_t posStart=0, posEnd;
	while(1)
	{
		posEnd=list.find(expression, posStart);
		if(posEnd==string::npos)
			break;
		string line=list.substr(posStart, posEnd-posStart);
		if(line.length()>0)
			res.push_back(line);
		posStart=posEnd+expression.length();
	}
	string line=list.substr(posStart, posEnd-posStart);
	if(line.length()>0)
		res.push_back(line);
}

vector<GCodeLine>& GCodeInterpreter::getCodeLines()
{
	return codeLines;
}

float GCodeInterpreter::getMinX() { if(minX!=minX) return 0; else return minX; }
float GCodeInterpreter::getMinY() { if(minY!=minY) return 0; else return minY; }
float GCodeInterpreter::getMinZ() { if(minZ!=minZ) return 0; else return minZ; }
float GCodeInterpreter::getMaxX() { if(maxX!=maxX) return 0; else return maxX; }
float GCodeInterpreter::getMaxY() { if(maxY!=maxY) return 0; else return maxY; }
float GCodeInterpreter::getMaxZ() { if(maxZ!=maxZ) return 0; else return maxZ; }

bool GCodeInterpreter::isOpen()
{
	if(codeLines.size()>0)
		return true;
	else
		return false;
}
