#pragma once
#include "MFCDlg.h"


// CXiaoHuoJian 对话框

class CXiaoHuoJian : public CDialogEx
{
	DECLARE_DYNAMIC(CXiaoHuoJian)

public:
	CXiaoHuoJian(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CXiaoHuoJian();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_xiaohuojian };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();//初始化
	afx_msg void OnPaint();//画
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); //单击
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);//鼠标
	afx_msg void OnTimer(UINT_PTR nIDEvent);//计时器
	CMFCDlg m_memory;//依靠父类窗口

};
