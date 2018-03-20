#pragma once


// COptlonalDlg 对话框

class COptlonalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COptlonalDlg)

public:
	COptlonalDlg(IMAGE_OPTIONAL_HEADER* poptHdr,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COptlonalDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_Magic;
	DWORD m_MajorLinkerVersion;
	DWORD m_MinorLinkerVersion;
	double m_SizeOfCode;
	DWORD m_SizeOfInitData;
	DWORD m_SizeOfUninitData;
	double m_EntryPoint;
	DWORD m_BaseOfCode;
	DWORD m_BaseOfData;
	DWORD m_ImageBase;
	DWORD m_SectionAlignment;
	DWORD m_FileAlignment;
	DWORD MajorOperatingSystemVersion;
	DWORD MinorOperatingSystemVersion;
	DWORD m_MajorImageVersion;
	DWORD m_MinorImageVEersion;
	DWORD m_MajorSubsystemVersion;
	DWORD m_MinorSubsystemVersion;
	DWORD m_Win32VersionValue;
	DWORD m_SizeOfImage;
	DWORD m_SizeOfHeader;
	DWORD m_CheckSum;
	DWORD m_Subsystem;
	DWORD m_DllCharacteristics;
	DWORD m_SizeOfStackReserve;
	DWORD m_SizeOfStackCommit;
	DWORD m_SizeOfheapReserve;
	DWORD m_SizeOfheapCommit;
	DWORD m_LoaderFlags;
	DWORD m_NumberOfRvaAndSize;

	IMAGE_OPTIONAL_HEADER* poptHdr;//扩展头
	virtual BOOL OnInitDialog();
};
