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

#include "Helper.hpp"

string Helper::Int2String(int Value)
{
	ostringstream Buffer;
	Buffer<<Value;
	return Buffer.str();
}

string Helper::Int2String(unsigned int Value)
{
	ostringstream Buffer;
	Buffer<<Value;
	return Buffer.str();
}

string Helper::Int2String(int Value, int Space)
{
	string Buffer="";
	int CurrPos=10;
	for(int i=0; i<Space-1; i++)
	{
		if(Value<CurrPos)
			Buffer+=" ";
		CurrPos*=10;
	}
	ostringstream Stream;
	Stream<<Value;
	Buffer+=Stream.str();
	return Buffer;
}

string Helper::Float2String(float Value)
{
	ostringstream Buffer;
	Buffer<<Value;
	return Buffer.str();
}

int Helper::String2Int(string Value)
{
	istringstream buffer(Value);
	int returncode;
	buffer >> returncode;
	return returncode;
}

float Helper::String2float(string Value)
{
  istringstream buffer(Value);
  float returncode;
  buffer >> returncode;
  return returncode;
}
