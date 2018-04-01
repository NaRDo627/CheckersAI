
// CheckersAIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckersAI.h"
#include "CheckersAIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCheckersAIDlg dialog



CCheckersAIDlg::CCheckersAIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CHECKERSAI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckersAIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_CHECKERBOARD, m_grdCheckersBoard);
}

BEGIN_MESSAGE_MAP(CCheckersAIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CCheckersAIDlg message handlers

BOOL CCheckersAIDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitCheckersBoard();
	//DrawCheckerBoard();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCheckersAIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCheckersAIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCheckersAIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


VOID CCheckersAIDlg::InitCheckersBoard()
{
	m_grdCheckersBoard.SetEditable(FALSE);
	m_grdCheckersBoard.EnableSelection(FALSE);
	m_grdCheckersBoard.EnableScrollBar(SB_BOTH, FALSE);

	// 행/열 갯수 설정
	m_grdCheckersBoard.SetRowCount(8);
	m_grdCheckersBoard.SetColumnCount(8);

	// 열의 넓이, 행 높이 동시 설정
	INT nCnt = 8;		// 전체크기 / 원소크기 = 원소개수
	INT nWidthHeight[8];

	for(INT i = 0; i < nCnt; i++)
		nWidthHeight[i] = 75;
	
	for(INT c = 0; c < nCnt; c++)
	{
		for(INT r = 0; r < nCnt; r++)
		{
			m_grdCheckersBoard.SetColumnWidth(c, nWidthHeight[c]);
			m_grdCheckersBoard.SetRowHeight(r, nWidthHeight[c]);
			m_grdCheckersBoard.SetItemFormat(r, c, DT_CENTER);

			if((r + c) % 2 == 1)
			{
				m_grdCheckersBoard.SetItemBkColour(r, c, RGB(64, 64, 255));
				if(r < 3)
					m_grdCheckersBoard.SetItemText(r, c, L"White");
				else if(r > 4)
					m_grdCheckersBoard.SetItemText(r, c, L"Red");
			}
		}
	}

	m_grdCheckersBoard.Invalidate();
}

/*
VOID CCheckersAIDlg::DrawCheckerBoard()
{
	// 체커보드 내용 그대로 그린다.
	for(INT c = 0; c < 8; c++)
		for(INT r = 0; r < 8; r++)
		{
			// 원래 있던 내용 삭제
			m_grdCheckersBoard.SetItemText(r, c, _T(""));

			if(pPiece->GetTeam() == CHECKER_TEAM_RED)
				m_grdCheckersBoard.SetItemText(r, c, L"Red");
			else
				m_grdCheckersBoard.SetItemText(r, c, L"White");
		}

	m_grdCheckersBoard.Invalidate();
}

*/
/*
BOOL CCheckersAIDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if(wParam == (WPARAM)m_grdCheckersBoard.GetDlgCtrlID())
	{
		*pResult = 1;
		GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;
		//if(GVN_GETDISPINFO == pDispInfo->hdr.code)
		//{
			//TRACE2("Getting Display info for cell %d,%d\n", pDispInfo->item.row, pDispInfo->item.col);
			//pDispInfo->item.strText.Format(_T("Message %d,%d"),
				//pDispInfo->item.row, pDispInfo->item.col);
			//return TRUE;
		//}
		//else if(GVN_ODCACHEHINT == pDispInfo->hdr.code)
		if(GVN_ODCACHEHINT == pDispInfo->hdr.code)
		{
			GV_CACHEHINT *pCacheHint = (GV_CACHEHINT*)pDispInfo;
			TRACE(_T("Cache hint received for cell range %d,%d - %d,%d\n"),
				pCacheHint->range.GetMinRow(),
				pCacheHint->range.GetMinCol(),
				pCacheHint->range.GetMaxRow(),
				pCacheHint->range.GetMaxCol());
		}
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}
*/