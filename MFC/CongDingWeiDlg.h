#pragma once
#include "afxcmn.h"


// CongDingWeiDlg 对话框

class CongDingWeiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CongDingWeiDlg)

public:
	CongDingWeiDlg(IMAGE_DOS_HEADER* pDosHdr, BYTE* pBuf, IMAGE_FILE_HEADER*	 pFileHdr, IMAGE_SECTION_HEADER* pScnHdr, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CongDingWeiDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_tou;
	CListCtrl m_wei;

	IMAGE_DOS_HEADER* pDosHdr;
	DWORD RVAToOffset(LPVOID pDos, DWORD dwRva);
	typedef struct TypeOffset {  //拆分4字节位
		WORD Offset : 12;  // (1) 大小为12Bit的重定位偏移
		WORD Type : 4;    // (2) 大小为4Bit的重定位信息类型值
	}TypeOffset;	    // 这个结构体是A1Pass总结的
	BYTE* pFileBuff;
	IMAGE_FILE_HEADER*	 pFileHdr;
	IMAGE_SECTION_HEADER* pScnHdr;
	CString STDA;
	virtual BOOL OnInitDialog();
	CString m_RVA;
	CString m_size;
	
};
