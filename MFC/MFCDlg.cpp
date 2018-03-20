
// MFCDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "MFC.h"
#include "MFCDlg.h"
#include "afxdialogex.h"

#include "MyTab.h"
#include "XiaoHuoJian.h"
#include <winuser.h>
#include <windows.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFCDlg 对话框



CMFCDlg::CMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB1, m_tab);
	
}

BEGIN_MESSAGE_MAP(CMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMFCDlg::OnTcnSelchangeTab1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCDlg::OnBnClickedButton1)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCDlg::OnBnClickedButton2)
	ON_WM_HOTKEY()
END_MESSAGE_MAP()


// CMFCDlg 消息处理程序

BOOL CMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//位图
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP1);
	m_brBkgr.CreatePatternBrush(&bmp);
	bmp.DeleteObject();
	//小火箭
	CXiaoHuoJian* x = new CXiaoHuoJian;
	x->Create(IDD_xiaohuojian, this);
	x->ShowWindow(SW_SHOW);

	m_Kuai = ::RegisterHotKey(this->GetSafeHwnd(), 1, MOD_ALT | MOD_NOREPEAT, 0x42);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
}

//添加背景图片
HBRUSH CMFCDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_brBkgr;

	}
	if (pWnd->GetDlgCtrlID() == IDC_TAB1)//IDC_XXXX你的控件ID
	{
		pDC->SetBkMode(BKMODE_LAST);
	}
	
		
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CMFCDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == MessageBoxExW(NULL, _T("是否关闭窗口?"), _T("提示"), MB_OKCANCEL, NULL))
		// 关闭对话框
		EndDialog(NULL);
}


void CMFCDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}




void CMFCDlg::OnBnClickedButton2()
{
	CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK1);
	m_bool[0] = pBtn->GetCheck();
	if (m_bool[0]==1)
	{
		//sshutdn-------60秒倒计时关机命令 
		system("sshutdn");
		

	}
	pBtn = (CButton *)GetDlgItem(IDC_CHECK2);
	m_bool[1] = pBtn->GetCheck();
	if (m_bool[1] == 1)
	{
		//重启
		system("shutdown -R");

	}
	pBtn = (CButton *)GetDlgItem(IDC_CHECK3);
	m_bool[2] = pBtn->GetCheck();

	if (m_bool[2] == 1)
	{
		//休眠
		system("powercfg - hibernate on");

	}
	

	
}


BOOL CMFCDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMFCDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static bool b = TRUE;
	if (nHotKeyId == 1)
	{
		ShowWindow(b ? SW_HIDE : SW_SHOW);
		b = !b;
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
