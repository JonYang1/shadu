// CPU.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "CPU.h"
#include "afxdialogex.h"
#include <windows.h>
#include <Psapi.h>





// CCPU 对话框

IMPLEMENT_DYNAMIC(CCPU, CDialogEx)

CCPU::CCPU(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CPU, pParent)
	, m_jis(_T(""))
	, m_listaaa(_T(""))
{

}

CCPU::~CCPU()
{
}

void CCPU::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_jis);
	DDX_Text(pDX, IDC_EDIT2, m_listaaa);
}


BEGIN_MESSAGE_MAP(CCPU, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CCPU::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCPU::OnBnClickedButton2)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON3, &CCPU::OnBnClickedButton3)
END_MESSAGE_MAP()

__int64 CCPU::CompareFileTime(FILETIME time1, FILETIME time2)
{
	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;
	return   (b - a);
}

// CCPU 消息处理程序
int CCPU::CpuUseage()
{
	HANDLE hEvent;

	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime);

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled  
	

		WaitForSingleObject(hEvent, 1000); //等待500毫秒  

		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;
		GetSystemTimes(&idleTime, &kernelTime, &userTime);

		int idle = CompareFileTime(preidleTime, idleTime);
		int kernel = CompareFileTime(prekernelTime, kernelTime);
		int user = CompareFileTime(preuserTime, userTime);

		int cpu = (kernel + user - idle) * 100 / (kernel + user);
		
		//CString str;
		//str.Format(L"CPU使用率：%x %", cpu);
		int doub = cpu;
		m_CPU.push_back(doub);
		//m_jis += str;
	//	//UpdateData(FALSE);
	//	m_jis.Delete(0);
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;

	return 0;
}

BOOL CCPU::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

DWORD WINAPI CCPU::ThraedProc(LPVOID lpParam)
{
	CCPU* pThis = (CCPU*)lpParam;
	pThis->CpuUseage();


	CClientDC dc(pThis); //创建dc
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0)); //创建绘制的画笔
	CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)); //创建绘制时填充的画刷
	dc.SelectObject(&pen); //将画刷和画笔选入设备描述表中
	dc.SelectObject(pbrush);
	int size = pThis->m_CPU.size();
	for (int i = 0; i < size; i++)
	{
		//CPoint pt = m_CPU[i];    //就可以读取第i+1个CPoint点
		dc.MoveTo(i, 0);
		dc.LineTo(i, pThis->m_CPU[i]);
	}
	return true;
}

void CCPU::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);
	CpuUseage();

	CClientDC dc(this); //创建dc
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0)); //创建绘制的画笔
	CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)); //创建绘制时填充的画刷
	dc.SelectObject(&pen); //将画刷和画笔选入设备描述表中
	dc.SelectObject(pbrush);
	int size = m_CPU.size();
	for (int i = 0; i < size; i++)
	{
		//CPoint pt = m_CPU[i];    //就可以读取第i+1个CPoint点
		dc.MoveTo(i, 80);
		dc.LineTo(i, m_CPU[i]);
	}
}

//定时器代码
void CCPU::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(0, 1000, 0);

	
}

//CPU内存优化代码
void CCPU::OnBnClickedButton2()
{
	
	//获取当前的内存状态
	MEMORYSTATUSEX stcMemStatusEx = {0};
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	//2.清理内存
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList,bufSize,&dwNeedSize);
	for (DWORD i=0;i<dwNeedSize/sizeof(DWORD);i++)
	{
		CString aa;

		HANDLE hprocess = OpenProcess(PROCESS_SET_QUOTA,false,dwPIDList[i]);
		SetProcessWorkingSetSize(hprocess, -1, -1);
	

		aa.Format(L"%d", hprocess);
		int s = _ttoi(aa);
		s = s / 1024 ;

		m_jis.Format(L"%d",s);
		
	
	}
	//3.获取清理后的内存状态
	CString neic;

	GlobalMemoryStatusEx(&stcMemStatusEx);
	neic.Format(L"%d ",stcMemStatusEx.dwMemoryLoad);
	m_listaaa += neic;

	DWORDLONG afterCleanUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	
	UpdateData(FALSE);
	m_jis = "";
	m_listaaa = "";
}


void CCPU::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(&dc);
	CBitmap bmpBackground1;
	bmpBackground1.LoadBitmap(IDB_BITMAP3);
	BITMAP bitmap1;
	bmpBackground1.GetBitmap(&bitmap1);
	CBitmap* pbmpPri1 = dcMem1.SelectObject(&bmpBackground1);
	dc.BitBlt(0, -34, 1036, 539, &dcMem1, 0, 0, SRCCOPY);
}


void CCPU::OnBnClickedButton3()
{
	KillTimer(0);
}
