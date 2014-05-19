#include "StdAfx.h"
using namespace DuiLib;


class CMainWindow : public WindowImplBase
{
public:
	CMainWindow()
	{
	}
protected:
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) override
	{
		if(_tcscmp(pstrClass, _T("SysCombo")) == 0) return new CSysComboUI;
		return nullptr;
	}
	virtual CDuiString GetSkinFolder() override
	{
		return _T("");
	}
	virtual CDuiString GetSkinFile() override
	{
		return _T("MainWindow.xml");
	}
	virtual LPCTSTR GetWindowClassName(void) const override
	{
		return _T("女孩不哭");
	}

	virtual void InitWindow() override
	{


	}
	virtual void OnFinalMessage( HWND hWnd ) override
	{
		__super::OnFinalMessage(hWnd);
		delete this;
		::PostQuitMessage(0);
	}

	virtual void Notify(TNotifyUI& msg) override
	{
		if(msg.sType == DUI_MSGTYPE_CLICK){
			SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
		}
	}


private:
	CSysComboUI* m_pSysCombo;
	CSysButtonUI* m_pSysButton;
};

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	AllocConsole();
	freopen("CONIN$","r",stdin);
	freopen("CONOUT$","w",stdout);
	freopen("CONOUT$","w",stdout);
	CPaintManagerUI::SetInstance(hInstance);
	//CPaintManagerUI::LoadPlugin(SYSCTRL_DLL_NAME);

	CMainWindow* dlg = new CMainWindow();
	dlg->CreateDuiWindow(nullptr, _T("女孩不哭"), WS_OVERLAPPEDWINDOW|WS_SIZEBOX);
	dlg->CenterWindow();
	dlg->ShowModal();

	CPaintManagerUI::MessageLoop();

	return 0;
}
