#pragma once
#include "afxcmn.h"
#include <vector>
using std::vector;

// CMyJingc �Ի���
typedef struct _MY_PROCESSINFO
{
	DWORD     dwPid;					//PID
	TCHAR     szProcess[MAX_PATH];		//����
	TCHAR     szFullProcess[MAX_PATH]; //ȫ·��
	DWORD     dwPriorityClass;		//���ȼ�
	DWORD     dwParentProcess;		//��PID
	DWORD     dwThreadCount;			//�߳���
}MY_PROCESSINFO;

class CMyJingc : public CDialogEx
{
	DECLARE_DYNAMIC(CMyJingc)

public:
	CMyJingc(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyJingc();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JINC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ���ɵ���Ϣӳ�亯��
	BOOL EnumProcess();
	DECLARE_MESSAGE_MAP()
	BOOL InsertProcessList();
	bool IsRunAsAdmin(BOOL &bFullToken);
	bool EnableDebugPrivilege(BOOL bEnable = TRUE);
public:
	BOOL m_bIsFull;//�ж�Ȩ��
	
	vector<MY_PROCESSINFO> m_vecProcess;
	CListCtrl m_ctlListProcess;
	afx_msg void OnBnClickedQuanXian();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMokuai();
	CBrush m_brBkgr; //beij
	afx_msg void OnProc();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
};
