#pragma once
#include "MFCDlg.h"


// CXiaoHuoJian �Ի���

class CXiaoHuoJian : public CDialogEx
{
	DECLARE_DYNAMIC(CXiaoHuoJian)

public:
	CXiaoHuoJian(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CXiaoHuoJian();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_xiaohuojian };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();//��ʼ��
	afx_msg void OnPaint();//��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); //����
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);//���
	afx_msg void OnTimer(UINT_PTR nIDEvent);//��ʱ��
	CMFCDlg m_memory;//�������ര��

};
