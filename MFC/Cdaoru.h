#pragma once
#include "afxcmn.h"


// Cdaoru 对话框

class Cdaoru : public CDialogEx
{
	DECLARE_DYNAMIC(Cdaoru)

public:
	Cdaoru(IMAGE_DOS_HEADER* pDosHdr, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Cdaoru();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_daoru2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	IMAGE_DOS_HEADER* pDosHdr;
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos,
		DWORD dwRva);

	CListCtrl m_daoru;
	CListCtrl m_mokuaim;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
