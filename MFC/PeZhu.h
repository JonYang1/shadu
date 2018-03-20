#pragma once


// CPeZhu 对话框

class CPeZhu : public CDialogEx
{
	DECLARE_DYNAMIC(CPeZhu)

public:
	CPeZhu(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPeZhu();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva);

	void init();
	DWORD dwFileSize = 0;
	BYTE* pBuf;
	DWORD dwRead = 0;
	IMAGE_DOS_HEADER* pDosHdr;//DOS头结构体
	IMAGE_NT_HEADERS* pNtHdr;//NT头结构体
	IMAGE_FILE_HEADER*	 pFileHdr;//文件头
	IMAGE_OPTIONAL_HEADER* poptHdr;//扩展头
	DWORD dwNtHeaderSize;
	IMAGE_SECTION_HEADER* pScnHdr = NULL;
	IMAGE_SECTION_HEADER* pScnHdB = NULL;
	DWORD dwNtHeaderSizea;
	IMAGE_DATA_DIRECTORY* pDataDirectory;//资源目录
	HANDLE hFile = INVALID_HANDLE_VALUE;//文件
	IMAGE_EXPORT_DIRECTORY* pExpTab = NULL;
	DWORD* pExpAddr;

	DWORD dwExpAddrTabOfs;
	DWORD dwExpNameTabOfs;
	DWORD dwExpOrdTabOfs;
	DWORD* pExpOrd;
	DWORD dwNameOfs;

	LPVOID MyGetProcAddress(HMODULE hMod, char* pszFunctionName);
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
};
