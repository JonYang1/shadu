#pragma once
#include "afxcmn.h"
#include <vector>
#include "afxwin.h"
using std::vector;


// CShaDu 对话框
typedef struct _MY_PROCESSINFOA
{
	DWORD     dwPid;//PID
	TCHAR     szProcess[MAX_PATH];//名称
	TCHAR     szFullProcess[MAX_PATH];//全路径
	DWORD     dwPriorityClass;//优先级
	DWORD     dwParentProcess;//父PID
	DWORD     dwThreadCount;//线程数
}MY_PROCESSINFOA;
class CShaDu : public CDialogEx
{
	DECLARE_DYNAMIC(CShaDu)

public:
	CShaDu(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShaDu();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHADU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CListCtrl m_Qingli;
	virtual BOOL OnInitDialog();
	// 要删除的文件夹路径
	CString m_strFolder;
	// 打开选择文件夹对话框
	afx_msg void OnGetFolderBtn();
	// 遍历文件
	void TraverseFile(TCHAR szPath[MAX_PATH], const CString& strFilter, vector<CString> &vecPath);

	vector<MY_PROCESSINFOA> m_vecProcess;
	BOOL EnumProcess();//枚举进程信息
	// 保存要遍历的垃圾文件路径
	vector<CString> m_vecFileDelete;
	vector<CString> m_vecMd5;

	// 垃圾文件后缀
	CString m_strDelete;
	CTreeCtrl m_FileTree;
	bool isInThead;
	vector<CString> pFile;//保存文件路径

	afx_msg void OnBnClickedButton2();
	CListBox m_ctrListBoxFile;
	CListBox m_ctlListBoxFolder;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
