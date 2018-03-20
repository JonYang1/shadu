#pragma once


// CMuluDlg 对话框

class CMuluDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMuluDlg)

public:
	CMuluDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMuluDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString m_statica;
};
