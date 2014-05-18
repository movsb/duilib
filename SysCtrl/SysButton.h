#pragma once

namespace DuiLib{

#define SYSCTRL_BUTTON		_T("SysButton")
#define SYSCTRL_BUTTON_UI	SYSCTRL_BUTTON _T("UI")

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
};

}
