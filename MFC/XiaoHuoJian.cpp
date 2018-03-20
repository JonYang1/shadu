// XiaoHuoJian.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "XiaoHuoJian.h"
#include "afxdialogex.h"


// CXiaoHuoJian 对话框

IMPLEMENT_DYNAMIC(CXiaoHuoJian, CDialogEx)

CXiaoHuoJian::CXiaoHuoJian(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_xiaohuojian, pParent)
{

}

CXiaoHuoJian::~CXiaoHuoJian()
{
}

void CXiaoHuoJian::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CXiaoHuoJian, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CXiaoHuoJian 消息处理程序


BOOL CXiaoHuoJian::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	RECT Rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &Rc, 0);
	MoveWindow(Rc.right - 300, Rc.bottom - 200, 157, 208, TRUE);
	
	COLORREF maskColor = RGB(255, 255, 255);  // 将要设置为 透明色彩的颜色
	//更改指定窗口的属性。该函数还将指定偏移量的32位(long)值设置为额外的窗口内存。
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | 0x80000);
	//加载DLL
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));
	if (hInst)
	{
		typedef BOOL(WINAPI * MYFUNC)(HWND, COLORREF, BYTE, DWORD);
		MYFUNC fun = NULL;
		//			  显式链接到 DLL
		fun = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if (fun)fun(this->GetSafeHwnd(), maskColor, 255, 1);
		FreeLibrary(hInst);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CXiaoHuoJian::OnPaint()
{
	CRect rect;
	CPaintDC dc(this);
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));  //  这里填充的 要和前面设置的透明标志色要一样
	CBitmap bmp1, bmp2;
	//加载小火箭位图
	bmp1.LoadBitmap(IDB_BITMAP2);  //  填充了标志色的
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pbmpOld;
	pbmpOld = memDC.SelectObject(&bmp1);
	dc.BitBlt(0, 0, 208, 208, &memDC, 0, 0, SRCPAINT);  //(DWORD)0x00EE0086 /* dest = source OR dest           */

	memDC.SelectObject(&bmp2);
	dc.BitBlt(0, 0, 208, 208, &memDC, 0, 0, SRCAND);	// (DWORD)0x00EE0086 /* dest = source OR dest           */

	memDC.SelectObject(pbmpOld);
	memDC.DeleteDC();
}

//向窗口发消息
void CXiaoHuoJian::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
	
}

//鼠标消息
void CXiaoHuoJian::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetTimer(1, 1, NULL);
	m_memory.OnBnClickedButton1();
	CDialogEx::OnLButtonDblClk(nFlags, point);
	
}

//动画
void CXiaoHuoJian::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect Rc;
	GetWindowRect(Rc);
	Rc.top = Rc.top - 10;
	Rc.bottom = Rc.bottom - 10;
	MoveWindow(Rc, 1);
	
	if (Rc.top <= 0)
	{
		//关闭计时器
		KillTimer(1);
		//获取屏幕大小
		RECT Rc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &Rc, 0);
		MoveWindow(Rc.right - 300, Rc.bottom - 200, 157, 208, TRUE);

	}
	CDialogEx::OnTimer(nIDEvent);
}
