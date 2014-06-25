#ifndef __UISYSEDIT_H__
#define __UISYSEDIT_H__

#pragma once

/*
控件:
	系统控件 Edit

未实现:
	密码字符
	限制长度
*/

namespace DuiLib
{
#define SYSCTRL_EDIT		_T("SysEdit")
#define SYSCTRL_EDIT_UI		SYSCTRL_EDIT _T("UI")

	class CSysEditWnd;
	class CSysEditInit;

	class SYSCTRL_API CSysEditUI : public CControlUI
	{
		friend class CSysEditWnd;
	public:
		CSysEditUI();
		~CSysEditUI();

		LPCTSTR GetClass() const override;
		LPVOID GetInterface(LPCTSTR pstrName) override;
		UINT GetControlFlags() const override;
		HWND GetHWND() const;

		void SetEnabled(bool bEnable = true) override;

		virtual void SetText(LPCTSTR pstrText) override;
		virtual CDuiString GetText() const override;

		void SetPos(RECT rc);
		void SetVisible(bool bVisible = true) override;
		void SetInternVisible(bool bVisible = true) override;

		void SetFont(int iFont);
		int GetFont() const;

		void SetTextColor(DWORD dwColor);
		DWORD GetTextColor() const;

		void SetStyle(DWORD dwMask, DWORD dwStyle);
		DWORD GetStyle();

		void DoInit() override;
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoEvent(TEventUI& event) override;

		int GetLength();
		bool CanUndo() const;
		bool GetModify() const;
		void SetModify(bool bModified=true);
		void SetReadOnly(bool bReadOnly=true);
		bool GetReadOnly();
		void SelAll();
		void DeSelect();
		void SetSel(int start, int end);	//start can be less than end. start is included and end is excluded.
		void ReplaceSel(LPCTSTR str,bool bUndoable=false);
		void Copy();
		void Cut();
		void Paste();
		void Delete();
		void Undo();
		void PlaceCursor(int pos=-1);	// when -1, ends the cursor

	protected:
		virtual bool ContextMenuHandler(const CPoint& pt); // return value: true - handled, false - default menu

	protected:
		CSysEditInit*		m_pInit;
		CSysEditWnd*		m_pWindow;
		int					m_iFont;
		DWORD				m_dwTextColor;
		bool				m_bUseDefMenu;
	};
}
#endif // __UISYSEDIT_H__
