// Module:		Radix.h
// Author:		Mark Berrett
// Date:		April 23, 2020
// Purpose:		Header file for Radix.cpp
//				node for lined list
//

#pragma once
class Radix
{
public:
	Radix(string, string, string, string, string);			// constructor

	// accessors
	TCHAR* GetName();
	TCHAR* GetDec();
	TCHAR* GetHex();
	TCHAR* GetOct();
	TCHAR* GetBin();


private:
	TCHAR szName[TCHAR_SIZE];								// name of number, i.e. twenty-one
	TCHAR szDec[TCHAR_SIZE];								// decimal value, i.e. 21
	TCHAR szHex[TCHAR_SIZE];								// hexadecimal value, i.e. 15
	TCHAR szOct[TCHAR_SIZE];								// octal value, i.e. 25
	TCHAR szBin[TCHAR_SIZE];								// binary value, i.e. 0001 0101

};

