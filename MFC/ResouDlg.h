#pragma once
#include "afxcmn.h"


// CResouDlg 对话框

class CResouDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResouDlg)

public:
	CResouDlg(IMAGE_DOS_HEADER* pDosHdr, HANDLE hFile, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResouDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeCtr_k;
	virtual BOOL OnInitDialog();

	IMAGE_DOS_HEADER* pDosHdr;
	void parseResourcesTable(DWORD dwResRootDirAddr,
		IMAGE_RESOURCE_DIRECTORY* pResDir,
		int nDee);

	HTREEITEM hRoot;//控件
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva);
};
