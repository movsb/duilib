#ifndef __UICONTROL_H__
#define __UICONTROL_H__

#pragma once

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//

typedef CControlUI* (CALLBACK* FINDCONTROLPROC)(CControlUI*, LPVOID);

class UILIB_API CControlUI
{
public:
    CControlUI();
    virtual ~CControlUI();

public:
    virtual CDuiString GetName() const;
    virtual void SetName(LPCTSTR pstrName);
    virtual LPCTSTR GetClass() const;
	static LPCTSTR GetClassStatic(){ return _T("Control"); }
    virtual LPVOID GetInterface(LPCTSTR pstrName);
    virtual UINT GetControlFlags() const;

    virtual bool Activate();
	virtual void Notifier(int evt, void* ud=0);
    virtual CPaintManagerUI* GetManager() const;
    virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
    virtual CControlUI* GetParent() const;
    virtual CControlUI*& Owner() { return m_pOwner; }

public:
#define A(c)	friend class C##c##UI;  C##c##UI* To##c##UI() {return (C##c##UI*)this;}
	A(Edit)
	A(List)
	A(Text)

	A(Combo)
	A(Label)
	A(Flash)

	A(Button)
	A(Option)
	A(Slider)

	A(Control)
	A(ActiveX)

	A(ListItem)
	A(Progress)
	A(RichEdit)
	A(CheckBox)
	A(ComboBox)
	A(DateTime)
	A(TreeView)
	A(TreeNode)

	A(Container)
	A(TabLayout)
	A(ScrollBar)

	A(ListHeader)
	A(TileLayout)
	A(WebBrowser)
	
	A(ChildLayout)
	A(ListElement)

	A(DialogLayout)

	A(VerticalLayout)
	A(ListHeaderItem)
	
	A(ListTextElement)

	A(HorizontalLayout)
	A(ListLabelLayout)

	A(ListContainerElement)
#undef A


    // 文本相关
    virtual CDuiString GetText() const;
    virtual void SetText(LPCTSTR pstrText);

    // 图形相关
    DWORD GetBkColor() const;
    void SetBkColor(DWORD dwBackColor);
    DWORD GetBkColor2() const;
    void SetBkColor2(DWORD dwBackColor);
    DWORD GetBkColor3() const;
    void SetBkColor3(DWORD dwBackColor);
    LPCTSTR GetBkImage();
    void SetBkImage(LPCTSTR pStrImage);
	DWORD GetFocusBorderColor() const;
	void SetFocusBorderColor(DWORD dwBorderColor);
    bool IsColorHSL() const;
    void SetColorHSL(bool bColorHSL);
    SIZE GetBorderRound() const;
    void SetBorderRound(SIZE cxyRound);
    bool DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

	//边框相关
	int GetBorderSize() const;
	void SetBorderSize(int nSize);
	DWORD GetBorderColor() const;
	void SetBorderColor(DWORD dwBorderColor);

	void SetBorderSize(RECT rc);
	int GetLeftBorderSize() const;
	void SetLeftBorderSize(int nSize);
	int GetTopBorderSize() const;
	void SetTopBorderSize(int nSize);
	int GetRightBorderSize() const;
	void SetRightBorderSize(int nSize);
	int GetBottomBorderSize() const;
	void SetBottomBorderSize(int nSize);
	int GetBorderStyle() const;
	void SetBorderStyle(int nStyle);

    // 位置相关
    virtual const RECT& GetPos() const;
    virtual void SetPos(RECT rc);
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual int GetX() const;
    virtual int GetY() const;
    virtual RECT GetPadding() const;
    virtual void SetPadding(RECT rcPadding); // 设置外边距，由上层窗口绘制
    virtual SIZE GetFixedXY() const;         // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedXY(SIZE szXY);      // 仅float为true时有效
    virtual int GetFixedWidth() const;       // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedWidth(int cx);      // 预设的参考值
    virtual int GetFixedHeight() const;      // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedHeight(int cy);     // 预设的参考值
    virtual int GetMinWidth() const;
    virtual void SetMinWidth(int cx);
    virtual int GetMaxWidth() const;
    virtual void SetMaxWidth(int cx);
    virtual int GetMinHeight() const;
    virtual void SetMinHeight(int cy);
    virtual int GetMaxHeight() const;
    virtual void SetMaxHeight(int cy);
    virtual void SetRelativePos(SIZE szMove,SIZE szZoom);
    virtual void SetRelativeParentSize(SIZE sz);
    virtual TRelativePosUI GetRelativePos() const;
    virtual bool IsRelativePos() const;

    // 鼠标提示
    virtual CDuiString GetToolTip() const;
    virtual void SetToolTip(LPCTSTR pstrText);
	virtual void SetToolTipWidth(int nWidth);
	virtual int	  GetToolTipWidth(void);	// 多行ToolTip单行最长宽度

    // 快捷键
    virtual TCHAR GetShortcut() const;
    virtual void SetShortcut(TCHAR ch);

	//TAB键 应该没必要virtual
	virtual void SetWantTab(bool bWant = true);
	virtual bool IsWantTab() const;

    // 用户属性
    virtual const CDuiString& GetUserData(); // 辅助函数，供用户使用
    virtual void SetUserData(LPCTSTR pstrText); // 辅助函数，供用户使用
    virtual UINT_PTR GetTag() const; // 辅助函数，供用户使用
    virtual void SetTag(UINT_PTR pTag); // 辅助函数，供用户使用

    // 一些重要的属性
    virtual bool IsVisible() const;
    virtual void SetVisible(bool bVisible = true);
    virtual void SetInternVisible(bool bVisible = true); // 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数
    virtual bool IsEnabled() const;
    virtual void SetEnabled(bool bEnable = true);
    virtual bool IsMouseEnabled() const;
    virtual void SetMouseEnabled(bool bEnable = true);
    virtual bool IsKeyboardEnabled() const;
    virtual void SetKeyboardEnabled(bool bEnable = true);
    virtual bool IsFocused() const;
    virtual void SetFocus();
    virtual bool IsFloat() const;
    virtual void SetFloat(bool bFloat = true);

    virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	//鼠标捕捉
	bool SetCapture();
	void ReleaseCapture();

    void Invalidate();
    bool IsUpdateNeeded() const;
    void NeedUpdate();
    void NeedParentUpdate();
    DWORD GetAdjustColor(DWORD dwColor);

    virtual void Init();
    virtual void DoInit();

    virtual void Event(TEventUI& event);
    virtual void DoEvent(TEventUI& event);

    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    CControlUI* ApplyAttributeList(LPCTSTR pstrList);
	bool& UseDefaultAttribute() { return m_bUseDefaultAttr; }

    virtual SIZE EstimateSize(SIZE szAvailable);

    virtual void DoPaint(HDC hDC, const RECT& rcPaint);
    virtual void PaintBkColor(HDC hDC);
    virtual void PaintBkImage(HDC hDC);
    virtual void PaintStatusImage(HDC hDC);
    virtual void PaintText(HDC hDC);
    virtual void PaintBorder(HDC hDC);

    virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);

	//虚拟窗口参数
	void SetVirtualWnd(LPCTSTR pstrValue);
	CDuiString GetVirtualWnd() const;

public:
    CEventSource OnInit;
    CEventSource OnDestroy;
    CEventSource OnSize;
    CEventSource OnEvent;
    CEventSource OnNotify;

protected:
    CPaintManagerUI* m_pManager;
    CControlUI* m_pParent;	// Parent是用来传递Event 的; 除root外, 控件均有Parent
    CControlUI* m_pOwner;	// Owner 是用来传递Notify的; 有没有Owner由是否分组决定
	CDuiString m_sVirtualWnd;
    CDuiString m_sName;
    bool m_bUpdateNeeded;
	bool m_bWantTab;
	bool m_bUseDefaultAttr;
    RECT m_rcItem;
    RECT m_rcPadding;
    SIZE m_cXY;
    SIZE m_cxyFixed;
    SIZE m_cxyMin;
    SIZE m_cxyMax;
    bool m_bVisible;
    bool m_bInternVisible;
    bool m_bEnabled;
    bool m_bMouseEnabled;
	bool m_bKeyboardEnabled ;
    bool m_bFocused;
    bool m_bFloat;
    bool m_bSetPos; // 防止SetPos循环调用
    TRelativePosUI m_tRelativePos;

    CDuiString m_sText;
    CDuiString m_sToolTip;
    TCHAR m_chShortcut;
    CDuiString m_sUserData;
    UINT_PTR m_pTag;

    DWORD m_dwBackColor;
    DWORD m_dwBackColor2;
    DWORD m_dwBackColor3;
    CDuiString m_sBkImage;
	CDuiString m_sForeImage;
    DWORD m_dwBorderColor;
	DWORD m_dwFocusBorderColor;
    bool m_bColorHSL;
    int m_nBorderSize;
	int m_nBorderStyle;
	int m_nTooltipWidth;
    SIZE m_cxyBorderRound;
    RECT m_rcPaint;
	RECT m_rcBorderSize;
};

} // namespace DuiLib

#endif // __UICONTROL_H__
