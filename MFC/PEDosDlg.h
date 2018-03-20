#pragma once


// CPEDosDlg 对话框

class CPEDosDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPEDosDlg)

public:
	CPEDosDlg(IMAGE_DOS_HEADER* pDosHdr,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPEDosDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_e_magic;
	int m_e_cblp;
	int m_e_cp;
	int m_e_crlc;
	int m_e_cparhdr;
	int m_e_minalloc;
	int m_e_maxalloc;
	int my_e_ss;
	int my_e_sa;
	int my_e_csum;
	int my_e_ip;
	int my_e_cs;
	int my_e_ifarlc;
	int my_e_ovno;
	int my_e_res;
	int my_e_oemid;
	int my_e_oeminfo;
	int my_e_res2;
	int my_e_ifanew;



	IMAGE_DOS_HEADER* pDosHdr;
	virtual BOOL OnInitDialog();
};
