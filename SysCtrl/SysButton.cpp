#include "StdAfx.h"

namespace DuiLib{

class CSysButtonWnd : public CWindowWnd
{
public:
	CSysButtonWnd();
	void Init(CSysButtonUI* pOwner);

	virtual LPCTSTR GetSuperClassName() const override;
	virtual LPCTSTR GetWindowClassName() const override;
	virtual void OnFinalMessage(HWND hWnd) override;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

protected:
	CSysButtonUI* m_pOwner;
};

CSysButtonWnd::CSysButtonWnd()
	: m_pOwner(nullptr)
{

}

void CSysButtonWnd::Init(CSysButtonUI* pOwner)
{
	m_pOwner = pOwner;
	Create(m_pOwner->GetManager()->GetPaintWindow(),nullptr,m_pOwner->GetStyle(),m_pOwner->GetExStyle());
	if(GetHWND())
		pOwner->m_pWnd = this;
	else 
		pOwner->m_pWnd = nullptr;
}

LPCTSTR CSysButtonWnd::GetSuperClassName() const
{
	return WC_BUTTON;
}

LPCTSTR CSysButtonWnd::GetWindowClassName() const
{
	return _T("___") WC_BUTTON;
}

void CSysButtonWnd::OnFinalMessage(HWND hWnd)
{
	m_pOwner->m_pWnd = nullptr;
	delete this;
}

LRESULT CSysButtonWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg >= OCM__BASE){
		uMsg -= OCM__BASE;

		if(uMsg == WM_COMMAND){
			return m_pOwner->CommandHandler(GET_WM_COMMAND_CMD(wParam,lParam));
		}
		else if(uMsg == WM_NOTIFY){
			auto hdr = reinterpret_cast<NMHDR*>(lParam);
			if(!hdr) return 0;
			return m_pOwner->NotifyHandler(hdr);
		}
		return 0;
	}

	switch(uMsg)
	{
	case WM_SETFOCUS:
		// 很奇怪, 为什么没有focus虚线框
		m_pOwner->SetFocus();
		goto defproc;
	}

defproc:
	return __super::HandleMessage(uMsg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 

CSysButtonUI::CSysButtonUI()
	: m_pWnd(nullptr)
{
}

CSysButtonUI::~CSysButtonUI()
{
	if(m_pWnd) ::PostMessage(*m_pWnd,WM_CLOSE,0,0);
}

LPCTSTR CSysButtonUI::GetClass() const
{
	return SYSCTRL_BUTTON_UI;
}

LPVOID CSysButtonUI::GetInterface(LPCTSTR pstrName)
{
	if(_tcscmp(pstrName, SYSCTRL_BUTTON) ==0 ) return this;
	else if(_tcscmp(pstrName, _T("HWND"))==0)  return m_pWnd->GetHWND();
	return __super::GetInterface(pstrName);
}

UINT CSysButtonUI::GetControlFlags() const
{
	if( !IsEnabled() ) return CControlUI::GetControlFlags();

	return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
}

void CSysButtonUI::SetFont(int index)
{
	m_iFont = index;

	if(m_pWnd){
		HFONT hFont=NULL;
		int iFontIndex = m_iFont;
		if (iFontIndex!=-1)
			hFont = GetManager()->GetFont(iFontIndex);
		if (hFont == NULL)
			hFont = GetManager()->GetDefaultFontInfo()->hFont;
		SetWindowFont(m_pWnd->GetHWND(), hFont, TRUE);
	}
}

int CSysButtonUI::GetFont() const
{
	return m_iFont;
}

void CSysButtonUI::SetPos(RECT rc)
{
	__super::SetPos(rc);
	CDuiRect r = GetPos();
	if(m_pWnd) ::SetWindowPos(*m_pWnd,0,r.left, r.top, r.GetWidth(), r.GetHeight(), SWP_NOZORDER);
}

void CSysButtonUI::DoEvent(TEventUI& event)
{
	if(event.Type == UIEVENT_SETFOCUS){
		if(::GetFocus()!=*m_pWnd)
			::SetFocus(*m_pWnd);
	}
	return __super::DoEvent(event);
}

void CSysButtonUI::DoInit()
{
	m_pWnd = new CSysButtonWnd;
	m_pWnd->Init(this);
	SetEnabled(IsEnabled());
	SetVisible(IsVisible());
	SetFont(GetFont());
	SetText(GetText());
	::SetWindowPos(*m_pWnd,0,0,0,GetFixedWidth(),GetFixedHeight(),SWP_NOMOVE|SWP_NOZORDER);
}

void CSysButtonUI::SetEnabled(bool bEnable)
{
	if(m_pWnd) ::EnableWindow(*m_pWnd, bEnable);
	m_bEnabled = bEnable;
}

void CSysButtonUI::SetVisible(bool bVisible /* = true */)
{
	__super::SetVisible(bVisible);
	if(m_pWnd) m_pWnd->ShowWindow(bVisible,false);
}

void CSysButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if(_tcscmp(pstrName, _T("font")) == 0)			SetFont(_ttoi(pstrValue));

	else __super::SetAttribute(pstrName, pstrValue);
}


//////////////////////////////////////////////////////////////////////////

void CSysButtonUI::SetText( LPCTSTR pstrText )
{
	__super::SetText(pstrText);
	if(m_pWnd) m_pWnd->SendMessage(WM_SETTEXT,0,LPARAM(pstrText));
}

DWORD CSysButtonUI::GetStyle()
{
	return WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON;
}

DWORD CSysButtonUI::GetExStyle()
{
	return 0;
}

LRESULT CSysButtonUI::CommandHandler( int code )
{
	if(GetManager() && code == BN_CLICKED){
		GetManager()->SendNotify(this, DUI_MSGTYPE_CLICK);
		return 0;
	}
	return 0;
}

LRESULT CSysButtonUI::NotifyHandler( NMHDR* hdr )
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////

CSysOptionUI::CSysOptionUI()
	: m_bSelected(false)
{

}


LPCTSTR CSysOptionUI::GetClass() const
{
	return SYSCTRL_OPTION_UI;
}

LPVOID CSysOptionUI::GetInterface(LPCTSTR pstrName)
{
	if(_tcscmp(pstrName, SYSCTRL_OPTION) ==0 ) return this;
	return __super::GetInterface(pstrName);
}

DWORD CSysOptionUI::GetStyle()
{
	// 不能再此使用AutoRadioButton, 因为和系统的处理机制不一样
	return WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON;
}

UINT CSysOptionUI::GetControlFlags() const 
{
	if(m_pWnd){
		return IsSelected()?UIFLAG_TABSTOP:0;
	}
	return __super::GetControlFlags();
}

void CSysOptionUI::DoInit()
{
	__super::DoInit();
	if(m_bSelected)
		Selected();
}


void CSysOptionUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	if(_tcscmp(pstrName, _T("group")) == 0) SetGroup(pstrValue);
	else if(_tcscmp(pstrName, _T("selected")) == 0) m_bSelected = _tcscmp(pstrValue, _T("true"))==0;
	else return __super::SetAttribute(pstrName, pstrValue);
}

LRESULT CSysOptionUI::CommandHandler( int code )
{
	if(GetManager() && code == BN_CLICKED){
		Selected();
	}
	return 0;
}

LPCTSTR CSysOptionUI::GetGroup() const
{
	return m_sGroupName;
}

void CSysOptionUI::SetGroup( LPCTSTR pStrGroupName /*= NULL*/ )
{
	if( pStrGroupName == NULL ) {
		if( m_sGroupName.IsEmpty() ) return;
		m_sGroupName.Empty();
	}
	else {
		if( m_sGroupName == pStrGroupName ) return;
		if (!m_sGroupName.IsEmpty() && m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
		m_sGroupName = pStrGroupName;
	}

	if( !m_sGroupName.IsEmpty() ) {
		if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
	}
	else {
		if (m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
	}

}

bool CSysOptionUI::IsSelected() const
{
	if(m_pWnd){
		return !!::SendMessage(*m_pWnd, BM_GETCHECK, 0, 0);
	}
	else
		return m_bSelected;
}

void CSysOptionUI::Selected()
{
	if(!m_pWnd){
		m_bSelected = true;
		return;
	}

	if(IsSelected()) return;

	::SendMessage(*m_pWnd, BM_SETCHECK, BST_CHECKED, 0);

	if( m_pManager != NULL ) {
		if( !m_sGroupName.IsEmpty() ) {
			if( true ) {
				CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
				for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
					auto pControl = static_cast<CSysOptionUI*>(aOptionGroup->GetAt(i));
					if( pControl != this && pControl->m_pWnd) {
						::SendMessage(pControl->m_pWnd->GetHWND(), BM_SETCHECK, BST_UNCHECKED, 0);
					}
				}
				m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
			}
		}
		else {
			m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
		}
	}
}

//////////////////////////////////////////////////////////////////////////


CSysCheckBoxUI::CSysCheckBoxUI()
	: m_bAutoCheck(true)
	, m_bInitCheck(false)
{

}

LPCTSTR CSysCheckBoxUI::GetClass() const
{
	return SYSCTRL_CHECKBOX_UI;
}

LPVOID CSysCheckBoxUI::GetInterface(LPCTSTR pstrName)
{
	if(_tcscmp(pstrName, SYSCTRL_CHECKBOX) ==0 ) return this;
	return __super::GetInterface(pstrName);
}

DWORD CSysCheckBoxUI::GetStyle()
{
	return WS_CHILD|WS_VISIBLE|BS_CHECKBOX|(m_bAutoCheck?BS_AUTOCHECKBOX:BS_CHECKBOX);
}


bool CSysCheckBoxUI::IsChecked()
{
	if(m_pWnd){
		return ::SendMessage(*m_pWnd,BM_GETCHECK,0,0)==BST_CHECKED;
	}
	else
		return m_bInitCheck;
}

void CSysCheckBoxUI::SetCheck( bool bChecked )
{
	if(m_pWnd){
		::SendMessage(*m_pWnd, BM_SETCHECK, bChecked?BST_CHECKED:BST_UNCHECKED,0);
	}
	else{
		m_bInitCheck = bChecked;
	}
}

void CSysCheckBoxUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	if(_tcscmp(pstrName, _T("autocheck")) == 0) m_bAutoCheck = !_tcscmp(pstrValue, _T("true"));
	else if(_tcscmp(pstrName, _T("checked")) == 0) m_bInitCheck = !_tcscmp(pstrValue, _T("true"));
	else return __super::SetAttribute(pstrName, pstrValue);
}

LRESULT CSysCheckBoxUI::CommandHandler( int code )
{
	if(GetManager() && code == BN_CLICKED){
		GetManager()->SendNotify(this, DUI_MSGTYPE_CLICK, WPARAM(IsChecked()));
		return 0;
	}
	return 0;
}

}
