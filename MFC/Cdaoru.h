#pragma once
#include "afxcmn.h"


// Cdaoru �Ի���

class Cdaoru : public CDialogEx
{
	DECLARE_DYNAMIC(Cdaoru)

public:
	Cdaoru(IMAGE_DOS_HEADER* pDosHdr, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Cdaoru();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_daoru2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	IMAGE_DOS_HEADER* pDosHdr;
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos,
		DWORD dwRva);

	CListCtrl m_daoru;
	CListCtrl m_mokuaim;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
