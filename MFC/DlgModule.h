#pragma once
#include "afxcmn.h"
#include <TlHelp32.h>
#include <vector>
#include "afxcmn.h"
using std::vector;

// CDlgModule �Ի���

class CDlgModule : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModule)

public:
	CDlgModule(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgModule();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JINC_DLH_2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwPid;
	// ö��ģ����Ϣ
	BOOL EnumModule(DWORD dwPid);
	BOOL InsertModuleInfo();
	vector<MODULEENTRY32> m_vecModule;
	CListCtrl m_ctlListModule;
	virtual BOOL OnInitDialog();
};
