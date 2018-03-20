#pragma once
#include <iostream>
#include <vector>
using namespace std;
// CCPU �Ի���

class CCPU : public CDialogEx
{
	DECLARE_DYNAMIC(CCPU)

public:
	CCPU(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCPU();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CPU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int CpuUseage();

	typedef	unsigned char U_CHAR;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	__int64 CompareFileTime(FILETIME time1, FILETIME time2);//cpu
	vector<int> m_CPU;

	static DWORD WINAPI ThraedProc(LPVOID lpParam); //�߳�

	CString m_jis;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnPaint();
	CString m_listaaa;
	afx_msg void OnBnClickedButton3();
};
