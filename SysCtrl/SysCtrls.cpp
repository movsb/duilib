#include "StdAfx.h"

using namespace DuiLib;

//typedef CControlUI* (*LPCREATECONTROL)(LPCTSTR pstrType);

extern "C" __declspec(dllexport) CControlUI* CreateControl(LPCTSTR pstrType)
{
#define E(n) _tcscmp(pstrType,n)==0
	if(E(SYSCTRL_COMBO))			return new CSysComboUI;
	else if(E(SYSCTRL_BUTTON))		return new CSysButtonUI;

	else return nullptr;
#undef E
}
