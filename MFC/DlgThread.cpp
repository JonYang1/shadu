// DlgThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "DlgThread.h"
#include "afxdialogex.h"


// CDlgThread �Ի���

IMPLEMENT_DYNAMIC(CDlgThread, CDialogEx)

CDlgThread::CDlgThread(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_JINC_DLG, pParent)
{

}

CDlgThread::~CDlgThread()
{
}
// �����߳�
BOOL CDlgThread::EnumThread(DWORD dwPid)
{
	m_vecThread.clear();
	//1.�����߳̿���
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnap == INVALID_HANDLE_VALUE) return 0;
	//2ͨ���߳̿��վ����ȡ��һ���߳���Ϣ
	THREADENTRY32 stcThread = { sizeof(THREADENTRY32) };
	if (!Thread32First(hSnap, &stcThread)) {
		CloseHandle(hSnap);
		return 0;
	}
	//3ѭ�������߳���Ϣ
	do {
		if (stcThread.th32OwnerProcessID != dwPid) {
			continue;
		}
		m_vecThread.push_back(stcThread);
	} while (Thread32Next(hSnap, &stcThread));
	CloseHandle(hSnap);
	return 0;
}

// �����߳���Ϣ
BOOL CDlgThread::InsertThreadInfo()
{
	//List�ؼ�����
	CString str;
	int i = 0;
	for (THREADENTRY32 &thread : m_vecThread) {
		//�߳�ID
		str.Format(L"%d", thread.th32ThreadID);
		m_ctlListThread.InsertItem(i, str);
		//����ID
		str.Format(L"%d", thread.th32OwnerProcessID);
		m_ctlListThread.SetItemText(i, 1, str);
		//���ȼ�
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


// CDlgThread ��Ϣ�������
BOOL CDlgThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ctlListThread.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	TCHAR* szCol[3] = { L"�߳�ID",L"����ID",L"���ȼ�" };
	for (int i = 0; i < 3; ++i)
	{
		m_ctlListThread.InsertColumn(i, szCol[i], 0, 80);
	}
	// �����߳���Ϣ
	EnumThread(m_dwPid);
	InsertThreadInfo();
	return TRUE;  
				
}
