#pragma once


// CMuluDlg �Ի���

class CMuluDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMuluDlg)

public:
	CMuluDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMuluDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	CString m_statica;
};
