#pragma once
#include "afxcmn.h"


// CDerivedTabDlg 对话框

class CDerivedTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDerivedTabDlg)

public:
	CDerivedTabDlg(IMAGE_DOS_HEADER* pDosHdr, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDerivedTabDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_daoru };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_DerivedTab;
	CListCtrl m_daochu2;
	IMAGE_DOS_HEADER* pDosHdr;//DOS头结构体
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos,DWORD dwRva);//偏移
	virtual BOOL OnInitDialog();
};
