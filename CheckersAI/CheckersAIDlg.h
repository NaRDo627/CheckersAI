
// CheckersAIDlg.h : header file
//

#pragma once

//class CGridCtrl;
// CCheckersAIDlg dialog
class CCheckersAIDlg : public CDialog,
						public CGridEventHandler
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
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	// Checkers 게임 관련 함수
	VOID InitCheckersBoard();
	VOID RefreshCheckerBoard();
	VOID ReColorCheckerBoard();
	VOID ColorAvalialbePath(INT a_nRow, INT a_nCol);
	virtual void OnGridClick(int a_nRow, int a_nCol);



private:
	CGridCtrl	m_grdCheckersBoard;
	CCheckerGame*	m_pCheckerGame;
public:
	afx_msg void OnClose();
};
