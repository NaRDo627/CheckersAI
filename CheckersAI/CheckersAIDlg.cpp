
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
	m_grdCheckersBoard.SetEditable(false);
	m_grdCheckersBoard.SetSingleRowSelection(TRUE);
	m_grdCheckersBoard.SetSingleColSelection(TRUE);
	m_grdCheckersBoard.EnableScrollBar(SB_BOTH, FALSE);

	// 행/열 갯수 설정
	m_grdCheckersBoard.SetRowCount(8);
	m_grdCheckersBoard.SetColumnCount(8);

	// 열의 넓이, 행 높이 동시 설정
	int nCnt = 8;		// 전체크기 / 원소크기 = 원소개수
	int nWidthHeight[8];

	for(int i = 0; i < nCnt; i++)
		nWidthHeight[i] = 75;
	
	for(int i = 0; i < nCnt; i++)
	{
		for(int j = 0; j < nCnt; j++)
		{
			m_grdCheckersBoard.SetColumnWidth(i, nWidthHeight[i]);
			m_grdCheckersBoard.SetRowHeight(j, nWidthHeight[i]);
			m_grdCheckersBoard.SetItemFormat(j, i, DT_CENTER);

			if((i + j) % 2 == 0)
			{
				m_grdCheckersBoard.SetItemBkColour(j, i, RGB(64, 64, 255));
			}
		}
	}
	m_grdCheckersBoard.Invalidate();
}

