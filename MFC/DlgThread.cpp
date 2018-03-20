// DlgThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "DlgThread.h"
#include "afxdialogex.h"


// CDlgThread 对话框

IMPLEMENT_DYNAMIC(CDlgThread, CDialogEx)

CDlgThread::CDlgThread(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_JINC_DLG, pParent)
{

}

CDlgThread::~CDlgThread()
{
}
// 遍历线程
BOOL CDlgThread::EnumThread(DWORD dwPid)
{
	m_vecThread.clear();
	//1.创建线程快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnap == INVALID_HANDLE_VALUE) return 0;
	//2通过线程快照句柄获取第一个线程信息
	THREADENTRY32 stcThread = { sizeof(THREADENTRY32) };
	if (!Thread32First(hSnap, &stcThread)) {
		CloseHandle(hSnap);
		return 0;
	}
	//3循环遍历线程信息
	do {
		if (stcThread.th32OwnerProcessID != dwPid) {
			continue;
		}
		m_vecThread.push_back(stcThread);
	} while (Thread32Next(hSnap, &stcThread));
	CloseHandle(hSnap);
	return 0;
}

// 插入线程信息
BOOL CDlgThread::InsertThreadInfo()
{
	//List控件处理
	CString str;
	int i = 0;
	for (THREADENTRY32 &thread : m_vecThread) {
		//线程ID
		str.Format(L"%d", thread.th32ThreadID);
		m_ctlListThread.InsertItem(i, str);
		//进程ID
		str.Format(L"%d", thread.th32OwnerProcessID);
		m_ctlListThread.SetItemText(i, 1, str);
		//优先级
		str.Format(L"%d", thread.tpBasePri);
		m_ctlListThread.SetItemText(i, 2, str);
	}
	return TRUE;
}
void CDlgThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlListThread);
}


BEGIN_MESSAGE_MAP(CDlgThread, CDialogEx)
END_MESSAGE_MAP()


// CDlgThread 消息处理程序
BOOL CDlgThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_ctlListThread.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	TCHAR* szCol[3] = { L"线程ID",L"进程ID",L"优先级" };
	for (int i = 0; i < 3; ++i)
	{
		m_ctlListThread.InsertColumn(i, szCol[i], 0, 80);
	}
	// 遍历线程信息
	EnumThread(m_dwPid);
	InsertThreadInfo();
	return TRUE;  
				
}
