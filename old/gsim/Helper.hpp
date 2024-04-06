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

#ifndef _HELPER_H_
#define _HELPER_H_

#include <string>
#include <sstream>
#include <QWidget>
using namespace std;

class Helper
{
public:
	static string Int2String(int Value);
	static string Int2String(unsigned int Value);
	static string Int2String(int Value, int Space);
	static string Float2String(float Value);
	static int String2Int(string Value);
	static float String2float(string Value);
private:
	Helper();  // this class cannot be inherited
	~Helper();

};

#endif
