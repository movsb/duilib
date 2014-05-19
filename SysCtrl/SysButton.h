#pragma once

namespace DuiLib{

#define SYSCTRL_BUTTON		_T("SysButton")
#define SYSCTRL_BUTTON_UI	SYSCTRL_BUTTON _T("UI")

#define SYSCTRL_OPTION		_T("SysOption")
#define SYSCTRL_OPTION_UI	SYSCTRL_OPTION _T("UI")

#define SYSCTRL_CHECKBOX	_T("SysCheckBox")
#define SYSCTRL_CHECKBOX_UI	SYSCTRL_CHECKBOX _T("UI")

// #define SYSCTRL_GROUPBOX	_T("SysGroupBox")
// #define SYSCTRL_GROUPBOX_UI	SYSCTRL_GROUPBOX _T("UI")

class CSysButtonWnd;

class SYSCTRL_API CSysButtonUI : public CControlUI
{
	friend class CSysButtonWnd;
public:
	CSysButtonUI();
	~CSysButtonUI();

	virtual LPCTSTR GetClass() const override;
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;
	virtual UINT GetControlFlags() const override;
	virtual DWORD GetStyle();
	virtual DWORD GetExStyle();

	//Control Text Font
	void SetFont(int index);
	int GetFont() const;

	virtual void SetPos(RECT rc) override;
	SIZE EstimateSize(SIZE szAvailable) override;

	virtual void DoEvent(TEventUI& event) override;
	virtual void DoInit() override;
	virtual void SetEnabled(bool bEnable = true) override;
	virtual bool IsEnabled() const override;
	virtual void SetVisible(bool bVisible = true) override;

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

public:

	void SetText(LPCTSTR pstrText) override;

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
	virtual LPCTSTR GetClass() const override;
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;
	virtual UINT GetControlFlags() const override;
	virtual DWORD GetStyle() override;
	virtual DWORD GetExStyle() override;

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
};

//////////////////////////////////////////////////////////////////////////
// SysCheckBox
//////////////////////////////////////////////////////////////////////////

class SYSCTRL_API CSysCheckBoxUI : public CSysButtonUI
{
public:
	virtual LPCTSTR GetClass() const override;
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;
	virtual UINT GetControlFlags() const override;
	virtual DWORD GetStyle() override;
	virtual DWORD GetExStyle() override;

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
};

////////////////////////////////////////////////////////////////////////
//SysGroupBox
////////////////////////////////////////////////////////////////////////

// class SYSCTRL_API CSysGroupBoxUI : public CSysButtonUI
// {
// public:
// 	virtual LPCTSTR GetClass() const override;
// 	virtual LPVOID GetInterface(LPCTSTR pstrName) override;
// 	virtual UINT GetControlFlags() const override;
// 	virtual DWORD GetStyle() override;
// 	virtual DWORD GetExStyle() override;
// 
// 	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
// };

}
