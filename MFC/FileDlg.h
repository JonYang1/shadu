#pragma once


// CFileDlg �Ի���

class CFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(IMAGE_FILE_HEADER* pFileHdr,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_yingxiang };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_e_Machina;
	DWORD m_e_NumberOfSections;
	DWORD m_e_TimeDateStamp;
	DWORD m_e_PointerOfSymbolTable;
	DWORD m_e_NumberOfSymbols;
	DWORD m_e_SizeOfoPtionHeader;
	DWORD m_e_Characteristics;
	virtual BOOL OnInitDialog();
	IMAGE_FILE_HEADER*	 pFileHdr;//�ļ�ͷ
};
