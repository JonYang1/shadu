#pragma once
#include<iostream>
#include <vector>
#include "afxcmn.h"
using namespace std;

// CDiab 对话框

class CDiab : public CDialogEx
{
	DECLARE_DYNAMIC(CDiab)

public:
	CDiab(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiab();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_bool[6];
	// 要删除的文件夹路径
	CString m_strFolder;

	// 遍历文件
	void TraverseFile(TCHAR szPath[MAX_PATH], const CString& strFilter, vector<CString> &vecPath);
	DWORD WINAPI ThraedProc(LPVOID lpParam);

	// 垃圾文件后缀
	CString m_strDelete;
	void HuiSouZ();
	void deleteFile();//删除指定文件
	void Find(CString lpPath, CString Type);//系统垃圾
	vector<CString> m_CLEAR_vecPath;
	int m_Number;
	CString m_strsystem;
	CString m_strVS;
	CString m_vspath;
	CBrush m_brs;

	// 保存要遍历的垃圾文件路径
	vector<CString> m_vecFileDelete;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	CListCtrl m_listCtr;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheck5();
};
