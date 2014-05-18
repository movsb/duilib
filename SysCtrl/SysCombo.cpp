#include "StdAfx.h"

namespace DuiLib{

class CSysComboWnd : public CWindowWnd
{
public:
	CSysComboWnd();
	void Init(CSysComboUI* pOwner);

	virtual LPCTSTR GetSuperClassName() const override;
	virtual LPCTSTR GetWindowClassName() const override;
	virtual void OnFinalMessage(HWND hWnd) override;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

protected:
	CSysComboUI* m_pOwner;
};

CSysComboWnd::CSysComboWnd()
	: m_pOwner(nullptr)
{

}

void CSysComboWnd::Init(CSysComboUI* pOwner)
{
	m_pOwner = pOwner;
	CPaintManagerUI* p = m_pOwner->GetManager();
	Create(m_pOwner->GetManager()->GetPaintWindow(),nullptr,WS_CHILD|WS_VISIBLE | CBS_DROPDOWNLIST,0);

	HFONT hFont=NULL;
	int iFontIndex=m_pOwner->GetFont();
	if (iFontIndex!=-1)
		hFont=m_pOwner->GetManager()->GetFont(iFontIndex);
	if (hFont==NULL)
		hFont=m_pOwner->GetManager()->GetDefaultFontInfo()->hFont;
	SetWindowFont(*this, hFont, TRUE);
}

LPCTSTR CSysComboWnd::GetSuperClassName() const
{
	return WC_COMBOBOX;
}

LPCTSTR CSysComboWnd::GetWindowClassName() const
{
	return _T("___") WC_COMBOBOX;
}

void CSysComboWnd::OnFinalMessage(HWND hWnd)
{
	m_pOwner->Invalidate();
	m_pOwner->m_pWnd = nullptr;
	delete this;
}

LRESULT CSysComboWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg >= OCM__BASE){
		uMsg -= OCM__BASE;
		TNotifyUI msg;
		msg.pSender = m_pOwner;
		msg.wParam = 0;
		msg.lParam = 0;
		if(uMsg == WM_COMMAND){
			auto code = HIWORD(wParam);
			switch(code)
			{
			case CBN_SELCHANGE:
				msg.sType = _T("selchange");
				msg.wParam = SendMessage(CB_GETCURSEL);
				m_pOwner->GetManager()->SendNotify(msg);
				return 0;
			}
		}
		return 0;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 

CSysComboUI::CSysComboUI()
	: m_pWnd(nullptr)
{
}

CSysComboUI::~CSysComboUI()
{
	if(m_pWnd) ::PostMessage(*m_pWnd,WM_CLOSE,0,0);
}

LPCTSTR CSysComboUI::GetClass() const
{
	return SYSCTRL_COMBO_UI;	
}
LPVOID CSysComboUI::GetInterface(LPCTSTR pstrName)
{
	if(_tcscmp(pstrName, SYSCTRL_COMBO) ==0 ) return this;
	return __super::GetInterface(pstrName);
}

UINT CSysComboUI::GetControlFlags() const
{
	if( !IsEnabled() ) return CControlUI::GetControlFlags();

	return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
}


void CSysComboUI::SetFont(int index)
{
	m_iFont = index;
}

int CSysComboUI::GetFont() const
{
	return m_iFont;
}

void CSysComboUI::SetPos(RECT rc)
{
	__super::SetPos(rc);
	CDuiRect r = GetPos();
	::SetWindowPos(*m_pWnd,0,r.left, r.top, r.GetWidth(), r.GetHeight(), SWP_NOZORDER | ::IsWindowVisible(*m_pWnd)?SWP_SHOWWINDOW:SWP_HIDEWINDOW);
}

SIZE CSysComboUI::EstimateSize(SIZE szAvailable)
{
	if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 8);
	return CControlUI::EstimateSize(szAvailable);
}

void CSysComboUI::DoInit()
{
	m_pWnd = new CSysComboWnd;
	m_pWnd->Init(this);
	SetEnabled(IsEnabled());
	SetVisible(IsVisible());
}

void CSysComboUI::SetEnabled(bool bEnable)
{
	if(m_pWnd) ::EnableWindow(*m_pWnd, bEnable);
	m_bEnabled = bEnable;
}

bool CSysComboUI::IsEnabled() const
{
	return m_bEnabled;
}

void CSysComboUI::SetVisible(bool bVisible /* = true */)
{
	__super::SetVisible(bVisible);
	if(m_pWnd) m_pWnd->ShowWindow(bVisible,false);
}

void CSysComboUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if(_tcscmp(pstrName, _T("font")) == 0)			SetFont(_ttoi(pstrValue));

	else __super::SetAttribute(pstrName, pstrValue);
}
//////////////////////////////////////////////////////////////////////////

void CSysComboUI::AddText(LPCTSTR str)
{
	ComboBox_AddString(*m_pWnd,str);
}

CDuiString CSysComboUI::GetItemText(int i)
{
	int len = ::SendMessage(*m_pWnd,CB_GETLBTEXTLEN,i,0) + 1;
	CDuiString str;
	TCHAR* t = new TCHAR[len];
	memset(t, 0, len);
	::SendMessage(*m_pWnd,CB_GETLBTEXT, i, (LPARAM)t);
	str = t;
	delete[] t;
	return str;
}

}