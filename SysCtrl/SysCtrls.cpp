#include "StdAfx.h"

using namespace DuiLib;

//typedef CControlUI* (*LPCREATECONTROL)(LPCTSTR pstrType);

extern "C" __declspec(dllexport) CControlUI* CreateControl(LPCTSTR pstrType)
{
#define E(n) _tcscmp(pstrType,n)==0
	if(E(SYSCTRL_COMBO))			return new CSysComboUI;
	else if(E(SYSCTRL_BUTTON))		return new CSysButtonUI;
	else if(E(SYSCTRL_OPTION))		return new CSysOptionUI;
	else if(E(SYSCTRL_CHECKBOX))	return new CSysCheckBoxUI;
	else if(E(SYSCTRL_EDIT))		return new CSysEditUI;
//	else if(E(SYSCTRL_GROUPBOX))	return new CSysGroupBoxUI;

	else return nullptr;
#undef E
}
