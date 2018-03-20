#pragma once
#include <iostream>
#include <vector>
using namespace std;
// CCPU 对话框

class CCPU : public CDialogEx
{
	DECLARE_DYNAMIC(CCPU)

public:
	CCPU(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCPU();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CPU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int CpuUseage();

	typedef	unsigned char U_CHAR;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	__int64 CompareFileTime(FILETIME time1, FILETIME time2);//cpu
	vector<int> m_CPU;

	static DWORD WINAPI ThraedProc(LPVOID lpParam); //线程

	CString m_jis;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnPaint();
	CString m_listaaa;
	afx_msg void OnBnClickedButton3();
};
