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
		return _T("Å®º¢²»¿Þ");
	}

	virtual void InitWindow() override
	{
		m_pSysCombo	= static_cast<CSysComboUI*>(m_PaintManager.FindControl(_T("cbo")));
		m_pSysButton	= static_cast<CSysButtonUI*>(m_PaintManager.FindControl(_T("btn")));
		LPCTSTR a[] = {_T("×Ö·û´®1"),_T("×Ö·û´®2"),_T("×Ö·û´®3"),_T("×Ö·û´®4"),_T("Å®º¢²»¿Þ")};
		for(LPCTSTR s : a){
			m_pSysCombo->AddText(s);
		}
	}
	virtual void OnFinalMessage( HWND hWnd ) override
	{
		__super::OnFinalMessage(hWnd);
		delete this;
		::PostQuitMessage(0);
	}

	virtual void Notify(TNotifyUI& msg) override
	{
		printf("msg: %s\n", msg.sType);
		if(msg.pSender == m_pSysCombo){
			auto pCtrl = static_cast<CSysComboUI*>(msg.pSender);
			if(msg.sType == _T("selchange")){
				CDuiString str = pCtrl->GetItemText(int(msg.wParam));
				::MessageBox(GetHWND(), str, _T(""), MB_OK);
				return;
			}
		}
		if(_tcscmp(msg.pSender->GetClass(), SYSCTRL_BUTTON_UI) == 0
			&& msg.sType == DUI_MSGTYPE_CLICK){
			::MessageBox(GetHWND(), msg.pSender->GetText(),_T(""),MB_OK);
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
	CPaintManagerUI::LoadPlugin(SYSCTRL_DLL_NAME);

	CMainWindow* dlg = new CMainWindow();
	dlg->CreateDuiWindow(nullptr, _T("Å®º¢²»¿Þ"), WS_OVERLAPPEDWINDOW|WS_SIZEBOX);
	dlg->CenterWindow();
	dlg->ShowModal();

	CPaintManagerUI::MessageLoop();

	return 0;
}
