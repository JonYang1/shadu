#pragma once
#include "afxcmn.h"


// CongDingWeiDlg �Ի���

class CongDingWeiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CongDingWeiDlg)

public:
	CongDingWeiDlg(IMAGE_DOS_HEADER* pDosHdr, BYTE* pBuf, IMAGE_FILE_HEADER*	 pFileHdr, IMAGE_SECTION_HEADER* pScnHdr, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CongDingWeiDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_tou;
	CListCtrl m_wei;

	IMAGE_DOS_HEADER* pDosHdr;
	DWORD RVAToOffset(LPVOID pDos, DWORD dwRva);
	typedef struct TypeOffset {  //���4�ֽ�λ
		WORD Offset : 12;  // (1) ��СΪ12Bit���ض�λƫ��
		WORD Type : 4;    // (2) ��СΪ4Bit���ض�λ��Ϣ����ֵ
	}TypeOffset;	    // ����ṹ����A1Pass�ܽ��
	BYTE* pFileBuff;
	IMAGE_FILE_HEADER*	 pFileHdr;
	IMAGE_SECTION_HEADER* pScnHdr;
	CString STDA;
	virtual BOOL OnInitDialog();
	CString m_RVA;
	CString m_size;
	
};
