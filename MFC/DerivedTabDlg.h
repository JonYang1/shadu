#pragma once
#include "afxcmn.h"


// CDerivedTabDlg �Ի���

class CDerivedTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDerivedTabDlg)

public:
	CDerivedTabDlg(IMAGE_DOS_HEADER* pDosHdr, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDerivedTabDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_daoru };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_DerivedTab;
	CListCtrl m_daochu2;
	IMAGE_DOS_HEADER* pDosHdr;//DOSͷ�ṹ��
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos,DWORD dwRva);//ƫ��
	virtual BOOL OnInitDialog();
};
