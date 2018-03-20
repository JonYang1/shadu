#pragma once
#include "afxcmn.h"
#include <vector>
using std::vector;

// CMyJingc 对话框
typedef struct _MY_PROCESSINFO
{
	DWORD     dwPid;					//PID
	TCHAR     szProcess[MAX_PATH];		//名称
	TCHAR     szFullProcess[MAX_PATH]; //全路径
	DWORD     dwPriorityClass;		//优先级
	DWORD     dwParentProcess;		//父PID
	DWORD     dwThreadCount;			//线程数
}MY_PROCESSINFO;

class CMyJingc : public CDialogEx
{
	DECLARE_DYNAMIC(CMyJingc)

public:
	CMyJingc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyJingc();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JINC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 生成的消息映射函数
	BOOL EnumProcess();
	DECLARE_MESSAGE_MAP()
	BOOL InsertProcessList();
	bool IsRunAsAdmin(BOOL &bFullToken);
	bool EnableDebugPrivilege(BOOL bEnable = TRUE);
public:
	BOOL m_bIsFull;//判断权限
	
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
