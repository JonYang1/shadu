#pragma once
#include<iostream>
#include <vector>
#include "afxcmn.h"
using namespace std;

// CDiab �Ի���

class CDiab : public CDialogEx
{
	DECLARE_DYNAMIC(CDiab)

public:
	CDiab(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiab();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_bool[6];
	// Ҫɾ�����ļ���·��
	CString m_strFolder;

	// �����ļ�
	void TraverseFile(TCHAR szPath[MAX_PATH], const CString& strFilter, vector<CString> &vecPath);
	DWORD WINAPI ThraedProc(LPVOID lpParam);

	// �����ļ���׺
	CString m_strDelete;
	void HuiSouZ();
	void deleteFile();//ɾ��ָ���ļ�
	void Find(CString lpPath, CString Type);//ϵͳ����
	vector<CString> m_CLEAR_vecPath;
	int m_Number;
	CString m_strsystem;
	CString m_strVS;
	CString m_vspath;
	CBrush m_brs;

	// ����Ҫ�����������ļ�·��
	vector<CString> m_vecFileDelete;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	CListCtrl m_listCtr;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheck5();
};
