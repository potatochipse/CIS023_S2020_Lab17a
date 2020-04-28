// Module:		Radix.cpp
// Author:		Mark Berrett
// Date:		April 23, 2020
// Purpose:		Implimentation file for Radix
//				node for lined list
//

#include "framework.h"
#include "CIS023_S2020_Lab17a.h"

Radix::Radix(string strName, string strDec, string strHex, string strOct, string strBin)
{
	StringtoTCHAR(&strName, szName);			// store name
	StringtoTCHAR(&strDec, szDec);				// store decimal
	StringtoTCHAR(&strHex, szHex);				// store hex
	StringtoTCHAR(&strOct, szOct);				// store oct
	StringtoTCHAR(&strBin, szBin);				// store bin

}

TCHAR* Radix::GetName()
{
	return szName;
}

TCHAR* Radix::GetDec()
{
	return szDec;
}

TCHAR* Radix::GetHex()
{
	return szHex;
}

TCHAR* Radix::GetOct()
{
	return szOct;
}

TCHAR* Radix::GetBin()
{
	return szBin;
}

