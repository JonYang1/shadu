#pragma once
#include "afxcmn.h"
#include <vector>
#include <TlHelp32.h>
using std::vector;

// CDlgThread �Ի���

class CDlgThread : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThread)

public:
	CDlgThread(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgThread();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JINC_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	BOOL EnumThread(DWORD dwPid);
	BOOL InsertThreadInfo();
	vector<THREADENTRY32> m_vecThread;
	
public:
	DWORD m_dwPid;
	CListCtrl m_ctlListThread;
	virtual BOOL OnInitDialog();
};
