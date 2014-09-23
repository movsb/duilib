//#ifndef WIN_IMPL_BASE_HPP
//#define WIN_IMPL_BASE_HPP

#include "stdafx.h"

namespace DuiLib
{

//////////////////////////////////////////////////////////////////////////

LPBYTE WindowImplBase::m_lpResourceZIPBuffer=NULL;

DUI_BEGIN_MESSAGE_MAP(WindowImplBase,CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()

void WindowImplBase::OnFinalMessage( HWND hWnd )
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());
}

LRESULT WindowImplBase::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		Close();
		return TRUE;
	}

	return FALSE;
}

CPaintManagerUI* WindowImplBase::GetManager()
{
	return &m_PaintManager;
}

CControlUI* WindowImplBase::FindControl(POINT pt)
{
	return m_PaintManager.FindControl(pt);
}

CControlUI* WindowImplBase::FindControl(LPCTSTR pstrName)
{
	return m_PaintManager.FindControl(pstrName);
}

UINT WindowImplBase::GetClassStyle() const
{
	return CS_DBLCLKS;
}

UILIB_RESOURCETYPE WindowImplBase::GetResourceType() const
{
	return UILIB_FILE;
}

CDuiString WindowImplBase::GetZIPFileName() const
{
	return _T("");
}

LPCTSTR WindowImplBase::GetResourceID() const
{
	return _T("");
}

CControlUI* WindowImplBase::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

LRESULT WindowImplBase::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			bHandled = !!ResponseDefaultKeyEvent(wParam);
			return 0;
		default:
			break;
		}
	}
	return FALSE;
}

LRESULT WindowImplBase::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

#if defined(WIN32) && !defined(UNDER_CE)
LRESULT WindowImplBase::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	return 0;
}

LRESULT WindowImplBase::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_bFrameWnd){
		bHandled = FALSE;
		return 0;
	}

	LPRECT pRect=NULL;

	if ( wParam == TRUE)
	{
		LPNCCALCSIZE_PARAMS pParam = (LPNCCALCSIZE_PARAMS)lParam;
		pRect=&pParam->rgrc[0];
	}
	else
	{
		pRect=(LPRECT)lParam;
	}

	if ( ::IsZoomed(m_hWnd))
	{	// 最大化时，计算当前显示器最适合宽高度
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		CDuiRect rcMonitor = oMonitor.rcMonitor;
		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

		pRect->right = pRect->left + rcWork.GetWidth();
		pRect->bottom = pRect->top + rcWork.GetHeight();
		return WVR_REDRAW;
	}

	return 0;
}

LRESULT WindowImplBase::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (m_bFrameWnd){
		bHandled = FALSE;
		return 0;
	}

	return 0;
}

LRESULT WindowImplBase::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_bFrameWnd){
		bHandled = FALSE;
		return 0;
	}

	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	
	if( !::IsZoomed(*this) )
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top )
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom )
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}

		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
			if( pControl && _tcsicmp(pControl->GetClass(), CButtonUI::GetClassStatic()) != 0 && 
				_tcsicmp(pControl->GetClass(), COptionUI::GetClassStatic()) != 0 &&
				_tcsicmp(pControl->GetClass(), CTextUI::GetClassStatic()) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT WindowImplBase::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;

	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	CDuiRect rcMonitor = oMonitor.rcMonitor;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	// 计算最大化时，正确的原点坐标
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;

	lpMMI->ptMaxTrackSize.x =rcWork.GetWidth();
	lpMMI->ptMaxTrackSize.y =rcWork.GetHeight();

	lpMMI->ptMinTrackSize.x =m_PaintManager.GetMinInfo().cx;
	lpMMI->ptMinTrackSize.y =m_PaintManager.GetMinInfo().cy;

	bHandled = FALSE;
	return 0;
}

#endif

LRESULT WindowImplBase::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_bFrameWnd){
		bHandled = FALSE;
		return 0;
	}

	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
#endif

	if (wParam != m_dwWindowPosState
		&& (wParam==SIZE_MAXIMIZED || wParam==SIZE_RESTORED)
		&& GetManager()->GetRoot())
	{
		CControlUI* btnmax = FindControl(_T("maxbtn"));
		CControlUI* btnrestore = FindControl(_T("restorebtn"));
		if (btnmax && btnrestore){
			if (wParam == SIZE_MAXIMIZED){
				btnmax->SetVisible(false);
				btnrestore->SetVisible(true);
			}
			else{
				btnmax->SetVisible(true);
				btnrestore->SetVisible(false);
			}
		}
	}

	m_dwWindowPosState = wParam;

	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_PaintManager.Init(m_hWnd);
	m_PaintManager.AddPreMessageFilter(this);

	CDialogBuilder builder;
	if (m_PaintManager.GetResourcePath().IsEmpty())
	{	// 允许更灵活的资源路径定义
		CDuiString strResourcePath=m_PaintManager.GetInstancePath();
		strResourcePath+=GetSkinFolder().GetData();
		m_PaintManager.SetResourcePath(strResourcePath.GetData());
	}

	switch(GetResourceType())
	{
	case UILIB_ZIP:
		m_PaintManager.SetResourceZip(GetZIPFileName().GetData(), true);
		break;
	case UILIB_ZIPRESOURCE:
		{
			HRSRC hResource = ::FindResource(m_PaintManager.GetResourceDll(), GetResourceID(), _T("ZIPRES"));
			if( hResource == NULL )
				return 0L;
			DWORD dwSize = 0;
			HGLOBAL hGlobal = ::LoadResource(m_PaintManager.GetResourceDll(), hResource);
			if( hGlobal == NULL ) 
			{
#if defined(WIN32) && !defined(UNDER_CE)
				::FreeResource(hResource);
#endif
				return 0L;
			}
			dwSize = ::SizeofResource(m_PaintManager.GetResourceDll(), hResource);
			if( dwSize == 0 )
				return 0L;
			m_lpResourceZIPBuffer = new BYTE[ dwSize ];
			if (m_lpResourceZIPBuffer != NULL)
			{
				::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
			}
#if defined(WIN32) && !defined(UNDER_CE)
			::FreeResource(hResource);
#endif
			m_PaintManager.SetResourceZip(m_lpResourceZIPBuffer, dwSize);
		}
		break;
	}

	CControlUI* pRoot=NULL;
	if (GetResourceType()==UILIB_RESOURCE)
	{
		STRINGorID xml(_ttoi(GetSkinFile().GetData()));
		pRoot = builder.Create(xml, _T("xml"), this, &m_PaintManager);
	}
	else
		pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, &m_PaintManager);
	ASSERT(pRoot);
	if (pRoot==NULL)
	{
		MessageBox(NULL,_T("加载资源文件失败"),_T("Duilib"),MB_OK|MB_ICONERROR);
		ExitProcess(1);
		return 0;
	}
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);
	m_PaintManager.SetBackgroundTransparent(TRUE);

	InitWindow();
	return 0;
}

LRESULT WindowImplBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
#if defined(WIN32) && !defined(UNDER_CE)
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
#endif
	case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	default:				bHandled = FALSE; break;
	}
	if (bHandled) return lRes;

	lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lRes;

	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
		return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT WindowImplBase::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void WindowImplBase::OnClick(TNotifyUI& msg)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if( sCtrlName == _T("closebtn") )
	{
		Close();
		return; 
	}
	else if( sCtrlName == _T("minbtn"))
	{ 
		::ShowWindow(GetHWND(), SW_MINIMIZE);
		return; 
	}
	else if( sCtrlName == _T("maxbtn"))
	{ 
		::ShowWindow(GetHWND(), SW_MAXIMIZE);
		return; 
	}
	else if( sCtrlName == _T("restorebtn"))
	{ 
		::ShowWindow(GetHWND(), SW_RESTORE);
		return; 
	}
	return;
}

void WindowImplBase::Notify(TNotifyUI& msg)
{
	return (void)CNotifyPump::NotifyPump(msg);
}

}
