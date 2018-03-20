#pragma once
#include "afxcmn.h"
#include <TlHelp32.h>
#include <vector>
#include "afxcmn.h"
using std::vector;

// CDlgModule 对话框

class CDlgModule : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModule)

public:
	CDlgModule(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModule();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JINC_DLH_2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwPid;
	// 枚举模块信息
	BOOL EnumModule(DWORD dwPid);
	BOOL InsertModuleInfo();
	vector<MODULEENTRY32> m_vecModule;
	CListCtrl m_ctlListModule;
	virtual BOOL OnInitDialog();
};
