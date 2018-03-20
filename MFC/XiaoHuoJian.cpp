// XiaoHuoJian.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "XiaoHuoJian.h"
#include "afxdialogex.h"


// CXiaoHuoJian �Ի���

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


// CXiaoHuoJian ��Ϣ�������


BOOL CXiaoHuoJian::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	RECT Rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &Rc, 0);
	MoveWindow(Rc.right - 300, Rc.bottom - 200, 157, 208, TRUE);
	
	COLORREF maskColor = RGB(255, 255, 255);  // ��Ҫ����Ϊ ͸��ɫ�ʵ���ɫ
	//����ָ�����ڵ����ԡ��ú�������ָ��ƫ������32λ(long)ֵ����Ϊ����Ĵ����ڴ档
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | 0x80000);
	//����DLL
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));
	if (hInst)
	{
		typedef BOOL(WINAPI * MYFUNC)(HWND, COLORREF, BYTE, DWORD);
		MYFUNC fun = NULL;
		//			  ��ʽ���ӵ� DLL
		fun = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if (fun)fun(this->GetSafeHwnd(), maskColor, 255, 1);
		FreeLibrary(hInst);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CXiaoHuoJian::OnPaint()
{
	CRect rect;
	CPaintDC dc(this);
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));  //  �������� Ҫ��ǰ�����õ�͸����־ɫҪһ��
	CBitmap bmp1, bmp2;
	//����С���λͼ
	bmp1.LoadBitmap(IDB_BITMAP2);  //  ����˱�־ɫ��
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

//�򴰿ڷ���Ϣ
void CXiaoHuoJian::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
	
}

//�����Ϣ
void CXiaoHuoJian::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SetTimer(1, 1, NULL);
	m_memory.OnBnClickedButton1();
	CDialogEx::OnLButtonDblClk(nFlags, point);
	
}

//����
void CXiaoHuoJian::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect Rc;
	GetWindowRect(Rc);
	Rc.top = Rc.top - 10;
	Rc.bottom = Rc.bottom - 10;
	MoveWindow(Rc, 1);
	
	if (Rc.top <= 0)
	{
		//�رռ�ʱ��
		KillTimer(1);
		//��ȡ��Ļ��С
		RECT Rc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &Rc, 0);
		MoveWindow(Rc.right - 300, Rc.bottom - 200, 157, 208, TRUE);

	}
	CDialogEx::OnTimer(nIDEvent);
}
