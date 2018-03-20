#pragma once


// CFileDlg 对话框

class CFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(IMAGE_FILE_HEADER* pFileHdr,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_yingxiang };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	IMAGE_FILE_HEADER*	 pFileHdr;//文件头
};
