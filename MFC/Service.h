#pragma once
#include "afxcmn.h"


// CService 对话框

class CService : public CDialogEx
{
	DECLARE_DYNAMIC(CService)

public:
	CService(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CService();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVICE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void servicea();//获取系统服务
	CListCtrl m_service;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);

};
