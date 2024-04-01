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
		return nullptr;
	}
	virtual CDuiString GetSkinFolder() override
	{
		return _T("");
	}
	virtual CDuiString GetSkinFile() override
	{
		return _T("test1.xml");
	}
	virtual LPCTSTR GetWindowClassName(void) const override
	{
		return _T("9078");
	}

	virtual void InitWindow() override
	{

	}
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	virtual void OnFinalMessage( HWND hWnd ) override
	{
		__super::OnFinalMessage(hWnd);
		delete this;
		::PostQuitMessage(0);
	}

	virtual void Notify(TNotifyUI& msg) override
	{
		if (msg.pSender->GetName() == "btn1"){
			if (msg.sType == "click"){
				MessageBox(m_hWnd, "", "", 0);
			}
		}
		__super::Notify(msg);
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
	CPaintManagerUI::StartupGdiPlus();
	CPaintManagerUI::LoadPlugin(SYSCTRL_DLL_NAME);
	CMainWindow* dlg = new CMainWindow();
	dlg->Create(NULL,"fffffffffff",UI_WNDSTYLE_FRAME);
	dlg->CenterWindow();
	dlg->ShowModal();

	CPaintManagerUI::MessageLoop();

	return 0;
}
