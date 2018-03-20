#pragma once
#include "afxcmn.h"
#include <vector>
#include "afxwin.h"
using std::vector;


// CShaDu �Ի���
typedef struct _MY_PROCESSINFOA
{
	DWORD     dwPid;//PID
	TCHAR     szProcess[MAX_PATH];//����
	TCHAR     szFullProcess[MAX_PATH];//ȫ·��
	DWORD     dwPriorityClass;//���ȼ�
	DWORD     dwParentProcess;//��PID
	DWORD     dwThreadCount;//�߳���
}MY_PROCESSINFOA;
class CShaDu : public CDialogEx
{
	DECLARE_DYNAMIC(CShaDu)

public:
	CShaDu(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShaDu();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHADU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CListCtrl m_Qingli;
	virtual BOOL OnInitDialog();
	// Ҫɾ�����ļ���·��
	CString m_strFolder;
	// ��ѡ���ļ��жԻ���
	afx_msg void OnGetFolderBtn();
	// �����ļ�
	void TraverseFile(TCHAR szPath[MAX_PATH], const CString& strFilter, vector<CString> &vecPath);

	vector<MY_PROCESSINFOA> m_vecProcess;
	BOOL EnumProcess();//ö�ٽ�����Ϣ
	// ����Ҫ�����������ļ�·��
	vector<CString> m_vecFileDelete;
	vector<CString> m_vecMd5;

	// �����ļ���׺
	CString m_strDelete;
	CTreeCtrl m_FileTree;
	bool isInThead;
	vector<CString> pFile;//�����ļ�·��

	afx_msg void OnBnClickedButton2();
	CListBox m_ctrListBoxFile;
	CListBox m_ctlListBoxFolder;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
