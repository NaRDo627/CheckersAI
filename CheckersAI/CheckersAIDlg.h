
// CheckersAIDlg.h : header file
//

#pragma once

class CGridCtrl;
// CCheckersAIDlg dialog
class CCheckersAIDlg : public CDialog
{
// Construction
public:
	CCheckersAIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECKERSAI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	VOID InitCheckersBoard();
	//VOID DrawCheckerBoard();

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// Dlg에서 생성만 해준다.
	CGridCtrl	m_grdCheckersBoard;
	//CCheckerGame	m_CheckerGame;
};
