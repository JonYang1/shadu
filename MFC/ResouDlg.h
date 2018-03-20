#pragma once
#include "afxcmn.h"


// CResouDlg �Ի���

class CResouDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResouDlg)

public:
	CResouDlg(IMAGE_DOS_HEADER* pDosHdr, HANDLE hFile, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResouDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeCtr_k;
	virtual BOOL OnInitDialog();

	IMAGE_DOS_HEADER* pDosHdr;
	void parseResourcesTable(DWORD dwResRootDirAddr,
		IMAGE_RESOURCE_DIRECTORY* pResDir,
		int nDee);

	HTREEITEM hRoot;//�ؼ�
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva);
};
