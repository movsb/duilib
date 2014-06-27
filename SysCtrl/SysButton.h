#pragma once

namespace DuiLib{

#define SYSCTRL_BUTTON		_T("SysButton")
#define SYSCTRL_BUTTON_UI	SYSCTRL_BUTTON _T("UI")

#define SYSCTRL_OPTION		_T("SysOption")
#define SYSCTRL_OPTION_UI	SYSCTRL_OPTION _T("UI")

#define SYSCTRL_CHECKBOX	_T("SysCheck")
#define SYSCTRL_CHECKBOX_UI	SYSCTRL_CHECKBOX _T("UI")

//  #define SYSCTRL_GROUPBOX	_T("SysGroupBox")
//  #define SYSCTRL_GROUPBOX_UI	SYSCTRL_GROUPBOX _T("UI")

class CSysButtonWnd;

class SYSCTRL_API CSysButtonUI : public CControlUI
{
	friend class CSysButtonWnd;
public:
	CSysButtonUI();
	~CSysButtonUI();

	virtual LPCTSTR GetClass() const override;
	static  LPCTSTR GetClassStatic() {return SYSCTRL_BUTTON_UI;}
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;
	virtual UINT GetControlFlags() const override;
	virtual DWORD GetStyle();
	virtual DWORD GetExStyle();

	//Control Text Font
	void SetFont(int index);
	int GetFont() const;

	virtual void SetPos(RECT rc) override;
	virtual void DoEvent(TEventUI& event) override;
	virtual void DoInit() override;
	virtual void SetEnabled(bool bEnable = true) override;
	virtual void SetVisible(bool bVisible = true) override;

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
	void SetText(LPCTSTR pstrText) override;

protected:
	virtual LRESULT CommandHandler(int code);
	virtual LRESULT NotifyHandler(NMHDR* hdr);
protected:
	CSysButtonWnd* m_pWnd;

	int			m_iFont;
	DWORD		m_dwStyle;
};

//////////////////////////////////////////////////////////////////////////
// SysOption
//////////////////////////////////////////////////////////////////////////

class SYSCTRL_API CSysOptionUI : public CSysButtonUI
{
public:
	CSysOptionUI();
	virtual LPCTSTR GetClass() const override;
	static LPCTSTR GetClassStatic() {return SYSCTRL_OPTION_UI;}
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;
	virtual DWORD GetStyle() override;
	virtual UINT GetControlFlags() const override;

	LPCTSTR GetGroup() const;
	void SetGroup(LPCTSTR pStrGroupName = NULL);
	bool IsSelected() const;
	virtual void Selected();

	virtual void DoInit() override;
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

protected:
	virtual LRESULT CommandHandler(int code);

protected:
	CDuiString		m_sGroupName;
	bool			m_bSelected;
};

//////////////////////////////////////////////////////////////////////////
// SysCheckBox
//////////////////////////////////////////////////////////////////////////

class SYSCTRL_API CSysCheckBoxUI : public CSysButtonUI
{
public:
	CSysCheckBoxUI();
	virtual LPCTSTR GetClass() const override;
	static LPCTSTR GetClassStatic() {return SYSCTRL_CHECKBOX_UI;}
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;
	virtual DWORD GetStyle() override;

	bool IsChecked();
	void SetCheck(bool bChecked);

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

protected:
	virtual LRESULT CommandHandler(int code);

protected:
	bool m_bAutoCheck;
	bool m_bInitCheck;
};

}
