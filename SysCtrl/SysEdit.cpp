#include "stdafx.h"
//#include <iostream>
namespace DuiLib
{
	//////////////////////////////////////////////////////////////////////////
	//
	class CSysEditInit
	{
	public:
		CSysEditInit()
		{
			dwStyle = WS_CHILD | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_NOHIDESEL;
			dwExStyle = 0;
		}

		DWORD			dwStyle;
		DWORD			dwExStyle;
		CDuiString		strTitle;
	};

	class CSysEditWnd : public CWindowWnd
	{
	public:
		CSysEditWnd();
		void Init(CSysEditUI* pOwner,CSysEditInit* pInit);
		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		CSysEditUI* m_pOwner;
		HBRUSH		m_hruBkBrush;
		DWORD		m_dwBkColor;
	};


	CSysEditWnd::CSysEditWnd() 
		: m_pOwner(NULL)
		, m_hruBkBrush(nullptr)
		, m_dwBkColor(0xFFFFFFFF)
	{
	}

	void CSysEditWnd::Init(CSysEditUI* pOwner,CSysEditInit* pInit)
	{
		m_pOwner = pOwner;

		// if style 'multiline' isn't specified, sure to make both scrollbars invisible.
		if(!(pInit->dwStyle & ES_MULTILINE)) pInit->dwStyle &= ~ (WS_VSCROLL | WS_HSCROLL);

		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, pInit->dwStyle, pInit->dwExStyle);
		if(GetHWND()){
			m_pOwner->m_pWindow = this;
		}
		else{
			m_pOwner->m_pWindow = nullptr;
			return;
		}
		
		m_pOwner->SetStyle(ES_WANTRETURN, pInit->dwStyle&ES_WANTRETURN? ES_WANTRETURN : 0);
		SendMessage(EM_LIMITTEXT,0);
	}

	LPCTSTR CSysEditWnd::GetWindowClassName() const
	{
		return _T("___") WC_EDIT;
	}

	LPCTSTR CSysEditWnd::GetSuperClassName() const
	{
		return WC_EDIT;
	}

	void CSysEditWnd::OnFinalMessage(HWND /*hWnd*/)
	{
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CSysEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(uMsg >= OCM__BASE){
			auto msg = uMsg - OCM__BASE;

			if(msg == WM_CTLCOLOREDIT || msg==WM_CTLCOLORSTATIC){
				auto cr = m_pOwner->GetBkColor();
				if(cr != m_dwBkColor){
					m_dwBkColor = cr;
					if(m_hruBkBrush){
						::DeleteObject(m_hruBkBrush);
						m_hruBkBrush = nullptr;
					}
				}

				COLORREF dwBkCr = RGB(GetRValue(m_dwBkColor), GetGValue(m_dwBkColor), GetBValue(m_dwBkColor));
				if(!m_hruBkBrush){
					m_hruBkBrush = ::CreateSolidBrush(dwBkCr);
				}
				::SetBkColor(HDC(wParam), dwBkCr);

				DWORD dwCr = m_pOwner->m_dwTextColor & 0x00FFFFFF;
				dwCr = RGB(GetBValue(dwCr), GetGValue(dwCr), GetRValue(dwCr));
				::SetTextColor((HDC)wParam, dwCr);

				return (LRESULT)m_hruBkBrush;
			}

			else if(msg == WM_COMMAND){
				int cmd = GET_WM_COMMAND_CMD(wParam, lParam);
				if(cmd == EN_CHANGE){
					m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
					return 0;
				}
			}

			return 0;
		}

		else if(uMsg == WM_SETFOCUS){
			m_pOwner->SetFocus();
		}
		else if(uMsg == WM_DESTROY){
			if(m_hruBkBrush){
				::DeleteObject(m_hruBkBrush);
				m_hruBkBrush = nullptr;
			}
			m_pOwner->m_pWindow = nullptr;
			return 0;
		}
		else if(uMsg == WM_CONTEXTMENU){
			POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
			auto rv = m_pOwner->ContextMenuHandler(pt);
			if(!rv) goto defproc;
			return 0;
		}
defproc:
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CSysEditUI::CSysEditUI() 
		: m_pWindow(nullptr)
		, m_pInit(nullptr)
		, m_iFont(-1)
		, m_bUseDefMenu(true)
	{
		m_pInit = new CSysEditInit;
		m_dwBackColor = 0xFFFFFFFF;
		m_dwTextColor = 0x00000000;
		m_bMenuUsed = true;
		m_bWantTab = true;
	}


	CSysEditUI::~CSysEditUI()
	{
		if(m_pWindow){
			if(::IsWindow(*m_pWindow)){
				::DestroyWindow(*m_pWindow);
			}
		}
	}


	LPCTSTR CSysEditUI::GetClass() const
	{
		return SYSCTRL_EDIT_UI;
	}

	LPVOID CSysEditUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, SYSCTRL_EDIT) == 0 ) return static_cast<CSysEditUI*>(this);
		else if(_tcscmp(pstrName, _T("HWND"))==0) return m_pWindow->GetHWND();
		else return __super::GetInterface(pstrName);
	}

	UINT CSysEditUI::GetControlFlags() const
	{
		if( !IsEnabled() ) return CControlUI::GetControlFlags();
		return UIFLAG_TABSTOP;
	}

	HWND CSysEditUI::GetHWND() const
	{
		return m_pWindow->GetHWND();
	}

	void CSysEditUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if(m_pWindow){
			::EnableWindow(*m_pWindow,m_bEnabled);
		}
	}

	void CSysEditUI::SetText(LPCTSTR pstrText)
	{
		if(m_pWindow){
			Edit_SetText(*m_pWindow, pstrText);
		}
		else{
			m_pInit->strTitle = pstrText;
		}
	}

	CDuiString CSysEditUI::GetText() const
	{
		CDuiString str;
		if(m_pWindow){
			int len = ::GetWindowTextLength(*m_pWindow);
			TCHAR* pText = new TCHAR[len+1];
			pText[0] = pText[len+1-1] = _T('\0');
			::GetWindowText(*m_pWindow, pText, len+1);
			str = pText;
			delete[] pText;
		}
		else{
			if(m_pInit){
				str = m_pInit->strTitle;
			}
		}
		return str;
	}

	void CSysEditUI::SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);
		if(m_pWindow) {
			CDuiRect rcPos = GetPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL,
				rcPos.left, rcPos.top, 
				rcPos.GetWidth(), rcPos.GetHeight(),
				SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	void CSysEditUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if(m_pWindow){
			::ShowWindow(*m_pWindow, IsVisible()?SW_SHOW:SW_HIDE);
		}
	}

	void CSysEditUI::SetInternVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if(m_pWindow){
			::ShowWindow(*m_pWindow, IsVisible()?SW_SHOW:SW_HIDE);
		}
	}

	void CSysEditUI::SetFont( int iFont )
	{
		m_iFont = iFont;
		if(m_pWindow){
			HFONT hFont=NULL;
			int iFontIndex = m_iFont;
			if (iFontIndex!=-1)
				hFont = GetManager()->GetFont(iFontIndex);
			if (hFont == NULL)
				hFont = GetManager()->GetDefaultFontInfo()->hFont;
			SetWindowFont(m_pWindow->GetHWND(), hFont, TRUE);
		}
	}

	int CSysEditUI::GetFont() const
	{
		return m_iFont;
	}

	void CSysEditUI::SetTextColor( DWORD dwColor )
	{
		m_dwTextColor = dwColor;
	}

	DWORD CSysEditUI::GetTextColor() const
	{
		return m_dwTextColor;
	}

	void CSysEditUI::SetStyle(DWORD dwMask, DWORD dwStyle)
	{
		if(m_pWindow){
			DWORD dw = GetStyle();
			dw &= ~dwMask;
			dw |= dwStyle & dwMask;
			::SetWindowLongPtr(*m_pWindow, GWL_STYLE, dw);
		}
		else{
			m_pInit->dwStyle &= ~dwMask;
			m_pInit->dwStyle |= dwStyle & dwMask;
		}
	}

	DWORD CSysEditUI::GetStyle()
	{
		if(m_pWindow)
			return ::GetWindowLongPtr(*m_pWindow,GWL_STYLE);
		else 
			return m_pInit->dwStyle;
	}

	void CSysEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if(_tcscmp(pstrName, _T("font")) == 0)				SetFont(_ttoi(pstrValue));
		else if(_tcscmp(pstrName, _T("vscroll")) == 0)		SetStyle(WS_VSCROLL,		!_tcscmp(pstrValue,_T("true"))?WS_VSCROLL:0);
		else if(_tcscmp(pstrName, _T("hscroll")) == 0)		SetStyle(WS_HSCROLL,		!_tcscmp(pstrValue,_T("true"))?WS_HSCROLL:0);
		else if(_tcscmp(pstrName, _T("readonly")) == 0)		SetStyle(ES_READONLY,		!_tcscmp(pstrValue,_T("true"))?ES_READONLY:0);
		else if(_tcscmp(pstrName, _T("multiline")) == 0)	SetStyle(ES_MULTILINE,		!_tcscmp(pstrValue,_T("true"))?ES_MULTILINE:0);
		else if(_tcscmp(pstrName, _T("wantreturn")) == 0)	SetStyle(ES_WANTRETURN,		!_tcscmp(pstrValue,_T("true"))?ES_WANTRETURN:0);
		else if(_tcscmp(pstrName, _T("nohidesel")) == 0)	SetStyle(ES_NOHIDESEL,		!_tcscmp(pstrValue,_T("true"))?ES_NOHIDESEL:0);
		else if(_tcscmp(pstrName, _T("textcolor")) == 0){
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextColor(clrColor);
		}
		else if(_tcscmp(pstrName, _T("defmenu")) == 0)		m_bUseDefMenu = !_tcscmp(pstrValue, _T("true"));
		else return __super::SetAttribute(pstrName, pstrValue);
	}

	void CSysEditUI::DoInit()
	{
		m_pWindow = new CSysEditWnd;
		m_pWindow->Init(this, m_pInit);
		SetFont(GetFont());
		SetText(m_pInit->strTitle);
		SetVisible(IsVisible());
		SetEnabled(IsEnabled());
		SetPos(GetPos());
		delete m_pInit;
		m_pInit = nullptr;
	}

	void CSysEditUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else __super::DoEvent(event);
			return;
		}

		if(event.Type == UIEVENT_SETFOCUS){
			if(::GetFocus() != *m_pWindow)
				::SetFocus(*m_pWindow);
		}

		__super::DoEvent(event);
	}

	int CSysEditUI::GetLength()
	{
		int len = 0;
		if(m_pWindow){
			len = ::GetWindowTextLength(m_pWindow->GetHWND());
		}
		return len;
	}

	bool CSysEditUI::CanUndo() const
	{
		if(m_pWindow)
			return !!::SendMessage(*m_pWindow,EM_CANUNDO,0,0);
		else 
			return false;
	}

	bool CSysEditUI::GetModify() const
	{
		if(m_pWindow)
			return !!::SendMessage(*m_pWindow, EM_GETMODIFY,0,0);
		else
			return false;
	}

	void CSysEditUI::SetModify(bool bModified)
	{
		if(m_pWindow)
			::SendMessage(*m_pWindow, EM_SETMODIFY, bModified, 0);
	}

	void CSysEditUI::SetReadOnly(bool bReadOnly)
	{
		if(m_pWindow)
			::SendMessage(*m_pWindow, EM_SETREADONLY, bReadOnly, 0);
		else
			SetStyle(ES_READONLY, bReadOnly?ES_READONLY:0);
	}

	bool CSysEditUI::GetReadOnly()
	{
		//if(m_pWindow)
			return !!(GetStyle() & ES_READONLY);
	}

	void CSysEditUI::SelAll()
	{
		if(m_pWindow){
			::SendMessage(*m_pWindow, EM_SETSEL, 0, -1);
		}
	}

	void CSysEditUI::DeSelect()
	{
		if(m_pWindow)
			::SendMessage(*m_pWindow, EM_SETSEL, -1, 0);
	}

	void CSysEditUI::SetSel(int start, int end)
	{
		if(m_pWindow){
			::SendMessage(*m_pWindow, EM_SETSEL, start, end);
		}
	}

	void CSysEditUI::ReplaceSel(LPCTSTR str, bool bUndoable/*=false*/ )
	{
		if(m_pWindow)
			::SendMessage(*m_pWindow, EM_REPLACESEL, bUndoable, LPARAM(str));
	}

	void CSysEditUI::Copy()
	{
		if(m_pWindow)
			::SendMessage(*m_pWindow, WM_COPY, 0, 0);
	}

	void CSysEditUI::Cut()
	{
		if(m_pWindow)
			::SendMessage(*m_pWindow, WM_CUT,0, 0);
	}

	void CSysEditUI::Paste()
	{
		if(m_pWindow)
			::SendMessage(*m_pWindow, WM_PASTE, 0, 0);
	}

	void CSysEditUI::Delete()
	{
		if(m_pWindow)
			::SendMessage(*m_pWindow, WM_CLEAR, 0, 0);
	}

	void CSysEditUI::Undo()
	{
		if(m_pWindow)
			::SendMessage(*m_pWindow, WM_UNDO, 0, 0);
	}

	void CSysEditUI::PlaceCursor( int pos/*=-1*/ )
	{
		if(m_pWindow){
			if(pos >= GetLength())
				pos = GetLength()-1;
			if(pos == -1)
				pos = GetLength();
			SetSel(pos, pos);
		}
	}

	bool CSysEditUI::ContextMenuHandler(const CPoint& pt)
	{
		if(!IsContextMenuUsed())
			return true;
		if(m_bUseDefMenu)
			return false;
		// send to wnd
		GetManager()->SendNotify(this, DUI_MSGTYPE_MENU);
		return true;
	}

}
