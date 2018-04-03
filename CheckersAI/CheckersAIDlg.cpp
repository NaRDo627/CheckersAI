
// CheckersAIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckersAI.h"
#include "CheckersAIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define		PLAYER_HUMAN	FALSE
#define		PLAYER_AI		TRUE

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
	ON_WM_CLOSE()
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
	m_pCheckerGame = new CCheckerGame(PLAYER_HUMAN, PLAYER_AI);

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
	m_grdCheckersBoard.SetEventHandler(this);

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

void CCheckersAIDlg::OnGridClick(int a_nRow, int a_nCol)
{
	static BOOL bSelected = FALSE;
	static BOOL bBonusOccured = FALSE;
	static INT nBeforeRow = -1;
	static INT nBeforeCol = -1;
	CCheckerPiece* pPiece = m_pCheckerGame->GetPieceByPos(a_nRow, a_nCol);
	INT nCurrentTurn = m_pCheckerGame->GetPlayerTurn();
	INT nResult = 0;
	BOOL bMoveResult = FALSE;

	if(nBeforeRow >= 0 && nBeforeCol >= 0)
		m_grdCheckersBoard.SetItemFgColour(nBeforeRow, nBeforeCol, RGB(0, 0, 0));
	ReColorCheckerBoard();

	if(!bSelected)
	{
		// 빈칸이라면 그냥 리턴
		if(!pPiece)
			return;

		// 자기 것이 아닌 말이라면 그냥 리턴
		if(pPiece->GetTeam() != nCurrentTurn)
			return;

		// 보너스 턴일 경우: 아까 그 말이 아니면 리턴
		if(bBonusOccured &&
			((nBeforeRow != a_nRow) ||
			(nBeforeCol != a_nCol)))
			return;

		bSelected = TRUE;
		nBeforeRow = a_nRow;
		nBeforeCol = a_nCol;
		m_grdCheckersBoard.SetItemFgColour(nBeforeRow, nBeforeCol, RGB(255, 64, 64));

		// 이동할 수 있는 곳 배경색 변경
		ColorAvalialbePath(nBeforeRow, nBeforeCol);
	}
	else
	{
		// 선택된 상태: 말이 있는 칸 선택했다면 선택 갱신
		if(pPiece)
		{
			// 보너스 턴일 경우: 아까 그 말이 아니면 리턴
			if(bBonusOccured &&
				((nBeforeRow != a_nRow) ||
				(nBeforeCol != a_nCol)))
				return;

			// 자기 것이 아닌 말이라면 선택해제 후 리턴
			if(pPiece->GetTeam() != nCurrentTurn)
			{
				bSelected = FALSE;
				return;
			}
			nBeforeRow = a_nRow;
			nBeforeCol = a_nCol;
			m_grdCheckersBoard.SetItemFgColour(nBeforeRow, nBeforeCol, RGB(255, 64, 64));

			// 이동할 수 있는 곳 배경색 변경
			ColorAvalialbePath(nBeforeRow, nBeforeCol);
			return;
		}

		// 이동 실시
		pPiece = m_pCheckerGame->GetPieceByPos(nBeforeRow, nBeforeCol);
		bMoveResult = m_pCheckerGame->MovePiece(pPiece, a_nRow, a_nCol);
		if(bMoveResult)
		{
			nBeforeRow = a_nRow;
			nBeforeCol = a_nCol;
			// 만약 턴이 바뀌지 않았다면 보너스 턴
			if(m_pCheckerGame->GetPlayerTurn() == nCurrentTurn)
				bBonusOccured = TRUE;
			else
				bBonusOccured = FALSE;
		}

		// 선택 해제
		bSelected = FALSE;
		m_grdCheckersBoard.SetItemFgColour(nBeforeRow, nBeforeCol, RGB(0, 0, 0));
		nResult = m_pCheckerGame->GetGameResult();
		if(nResult == CHECKER_TEAM_RED)
		{
			AfxMessageBox(L"Red WIN!!");
			m_pCheckerGame->InitalizeGame(PLAYER_HUMAN, PLAYER_AI);
		}
		else if(nResult == CHECKER_TEAM_WHITE)
		{
			AfxMessageBox(L"White WIN!!");
			m_pCheckerGame->InitalizeGame(PLAYER_HUMAN, PLAYER_AI);
		}
		else if(nResult == CHECKER_GAME_TIE)
		{
			AfxMessageBox(L"Match is TIE!!");
			m_pCheckerGame->InitalizeGame(PLAYER_HUMAN, PLAYER_AI);
		}
	}

	RefreshCheckerBoard();

	
	if(!bMoveResult)
		return;

	// 만약 넘어간 턴 상대가 AI라면 여기서 AI 로직 실행
	if(m_pCheckerGame->IsCurrentPlayerAI())
		m_pCheckerGame->PlayAITurn();

	RefreshCheckerBoard();

}


void CCheckersAIDlg::RefreshCheckerBoard()
{
	CCheckerPiece* pPiece = NULL;

	// 체커보드 내용 그대로 그린다.
	for(INT c = 0; c < 8; c++)
		for(INT r = 0; r < 8; r++)
		{
			// 원래 있던 내용 삭제
			m_grdCheckersBoard.SetItemText(r, c, _T(""));

			pPiece = m_pCheckerGame->GetPieceByPos(r, c);
			if(!pPiece)
				continue;

			if(pPiece->GetTeam() == CHECKER_TEAM_RED)
			{
				if(pPiece->IsPromoted())
					m_grdCheckersBoard.SetItemText(r, c, L"King Red");
				else
					m_grdCheckersBoard.SetItemText(r, c, L"Red");
			}
			else
			{
				if(pPiece->IsPromoted())
					m_grdCheckersBoard.SetItemText(r, c, L"King White");
				else
					m_grdCheckersBoard.SetItemText(r, c, L"White");
			}
		}

	m_grdCheckersBoard.Invalidate();
}

void CCheckersAIDlg::ReColorCheckerBoard()
{
	for(INT c = 0; c < 8; c++)
		for(INT r = 0; r < 8; r++)
		{
			if((r + c) % 2 == 1)
				m_grdCheckersBoard.SetItemBkColour(r, c, RGB(64, 64, 255));
		}

	m_grdCheckersBoard.Invalidate();
}

void CCheckersAIDlg::ColorAvalialbePath(INT a_nRow, INT a_nCol)
{
	CCheckerPiece* pPiece = m_pCheckerGame->GetPieceByPos(a_nRow, a_nCol);

	// 가능한 곳: 빨강 -> 왼쪽 위, 오른쪽 위 , 하양 -> 왼쪽 아래, 오른쪽 아래
	// 승격된 상태라면 반대도 가능
	if(pPiece->GetTeam() == CHECKER_TEAM_RED)
	{
		// 왼위 쪽으로 가능한지 검사
		if(a_nRow > 0 && a_nCol > 0)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol - 1) == NULL)
				m_grdCheckersBoard.SetItemBkColour(a_nRow - 1, a_nCol - 1, RGB(255, 64, 64));
		}
		if(a_nRow > 1 && a_nCol > 1)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol - 1) != NULL &&		// 대각선 왼쪽 위로 말이 있고
				(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol - 1)->GetTeam() != pPiece->GetTeam()) && // 그 말의 팀이 같은편이 아니고
				m_pCheckerGame->GetPieceByPos(a_nRow - 2, a_nCol - 2) == NULL)		// 그 너머로 말이 없을때
				m_grdCheckersBoard.SetItemBkColour(a_nRow - 2, a_nCol - 2, RGB(255, 64, 64));
		}

		// 오른위 쪽으로 가능한지 검사
		if(a_nRow > 0 && a_nCol < 7)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol + 1) == NULL)
				m_grdCheckersBoard.SetItemBkColour(a_nRow - 1, a_nCol + 1, RGB(255, 64, 64));
		}
		if(a_nRow > 1 && a_nCol < 6)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol + 1) != NULL &&		// 대각선 오른쪽 위로 말이 있고
				(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol + 1)->GetTeam() != pPiece->GetTeam()) && // 그 말의 팀이 같은편이 아니고
				m_pCheckerGame->GetPieceByPos(a_nRow - 2, a_nCol + 2) == NULL)		// 그 너머로 말이 없을때
				m_grdCheckersBoard.SetItemBkColour(a_nRow - 2, a_nCol + 2, RGB(255, 64, 64));
		}

		// 승격되지 않은 상태라면 리턴
		if(!m_pCheckerGame->GetPieceByPos(a_nRow, a_nCol)->IsPromoted())
		{
			m_grdCheckersBoard.Invalidate();
			return;
		}

		// 승격된 상태라면 대각선 아래쪽도 검사한다.
		// 왼아래 쪽으로 가능한지 검사
		if(a_nRow < 7 && a_nCol > 0)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol - 1) == NULL)
				m_grdCheckersBoard.SetItemBkColour(a_nRow + 1, a_nCol - 1, RGB(255, 64, 64));
		}
		if(a_nRow < 6 && a_nCol > 1)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol - 1) != NULL &&		// 대각선 왼쪽 아래로 말이 있고
				(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol - 1)->GetTeam() != pPiece->GetTeam()) && // 그 말의 팀이 같은편이 아니고
				m_pCheckerGame->GetPieceByPos(a_nRow + 2, a_nCol - 2) == NULL)		// 그 너머로 말이 없을때
				m_grdCheckersBoard.SetItemBkColour(a_nRow + 2, a_nCol - 2, RGB(255, 64, 64));
		}

		// 오른아래 쪽으로 가능한지 검사
		if(a_nRow < 7 && a_nCol < 7)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol + 1) == NULL)
				m_grdCheckersBoard.SetItemBkColour(a_nRow + 1, a_nCol + 1, RGB(255, 64, 64));
		}
		if(a_nRow < 6 && a_nCol < 6)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol + 1) != NULL &&		// 대각선 오른쪽 아래로 말이 있고
				(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol + 1)->GetTeam() != pPiece->GetTeam()) && // 그 말의 팀이 같은편이 아니고
				m_pCheckerGame->GetPieceByPos(a_nRow + 2, a_nCol + 2) == NULL)		// 그 너머로 말이 없을때
				m_grdCheckersBoard.SetItemBkColour(a_nRow + 2, a_nCol + 2, RGB(255, 64, 64));
		}
	}
	else
	{
		// 왼아래 쪽으로 가능한지 검사
		if(a_nRow < 7 && a_nCol > 0)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol - 1) == NULL)
				m_grdCheckersBoard.SetItemBkColour(a_nRow + 1, a_nCol - 1, RGB(255, 64, 64));
		}
		if(a_nRow < 6 && a_nCol > 1)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol - 1) != NULL &&		// 대각선 왼쪽 아래로 말이 있고
				(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol - 1)->GetTeam() != pPiece->GetTeam()) && // 그 말의 팀이 같은편이 아니고
				m_pCheckerGame->GetPieceByPos(a_nRow + 2, a_nCol - 2) == NULL)		// 그 너머로 말이 없을때
				m_grdCheckersBoard.SetItemBkColour(a_nRow + 2, a_nCol - 2, RGB(255, 64, 64));
		}

		// 오른아래 쪽으로 가능한지 검사
		if(a_nRow < 7 && a_nCol < 7)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol + 1) == NULL)
				m_grdCheckersBoard.SetItemBkColour(a_nRow + 1, a_nCol + 1, RGB(255, 64, 64));
		}
		if(a_nRow < 6 && a_nCol < 6)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol + 1) != NULL &&		// 대각선 오른쪽 아래로 말이 있고
				(m_pCheckerGame->GetPieceByPos(a_nRow + 1, a_nCol + 1)->GetTeam() != pPiece->GetTeam()) && // 그 말의 팀이 같은편이 아니고
				m_pCheckerGame->GetPieceByPos(a_nRow + 2, a_nCol + 2) == NULL)		// 그 너머로 말이 없을때
				m_grdCheckersBoard.SetItemBkColour(a_nRow + 2, a_nCol + 2, RGB(255, 64, 64));
		}

		// 승격되지 않은 상태라면 리턴
		if(!m_pCheckerGame->GetPieceByPos(a_nRow, a_nCol)->IsPromoted())
		{
			m_grdCheckersBoard.Invalidate();
			return;
		}

		// 승격된 상태라면 대각선 아래쪽도 검사한다.
		// 왼위 쪽으로 가능한지 검사
		if(a_nRow > 0 && a_nCol > 0)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol - 1) == NULL)
				m_grdCheckersBoard.SetItemBkColour(a_nRow - 1, a_nCol - 1, RGB(255, 64, 64));
		}
		if(a_nRow > 1 && a_nCol > 1)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol - 1) != NULL &&		// 대각선 왼쪽 위로 말이 있고
				(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol - 1)->GetTeam() != pPiece->GetTeam()) && // 그 말의 팀이 같은편이 아니고
				m_pCheckerGame->GetPieceByPos(a_nRow - 2, a_nCol - 2) == NULL)		// 그 너머로 말이 없을때
				m_grdCheckersBoard.SetItemBkColour(a_nRow - 2, a_nCol - 2, RGB(255, 64, 64));
		}

		// 오른위 쪽으로 가능한지 검사
		if(a_nRow > 0 && a_nCol < 7)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol + 1) == NULL)
				m_grdCheckersBoard.SetItemBkColour(a_nRow - 1, a_nCol + 1, RGB(255, 64, 64));
		}
		if(a_nRow > 1 && a_nCol < 6)
		{
			if(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol + 1) != NULL &&		// 대각선 오른쪽 위로 말이 있고
				(m_pCheckerGame->GetPieceByPos(a_nRow - 1, a_nCol + 1)->GetTeam() != pPiece->GetTeam()) && // 그 말의 팀이 같은편이 아니고
				m_pCheckerGame->GetPieceByPos(a_nRow - 2, a_nCol + 2) == NULL)		// 그 너머로 말이 없을때
				m_grdCheckersBoard.SetItemBkColour(a_nRow - 2, a_nCol + 2, RGB(255, 64, 64));
		}
	}

	m_grdCheckersBoard.Invalidate();
	return;
}


void CCheckersAIDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(m_pCheckerGame)
		delete m_pCheckerGame;

	__super::OnClose();
}
