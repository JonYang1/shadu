// OptlonalDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "OptlonalDlg.h"
#include "afxdialogex.h"


// COptlonalDlg 对话框

IMPLEMENT_DYNAMIC(COptlonalDlg, CDialogEx)

COptlonalDlg::COptlonalDlg(IMAGE_OPTIONAL_HEADER* poptHdr,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, m_Magic(0)
	, m_MajorLinkerVersion(0)
	, m_MinorLinkerVersion(0)
	, m_SizeOfCode(0)
	, m_SizeOfInitData(0)
	, m_SizeOfUninitData(0)
	, m_EntryPoint(0)
	, m_BaseOfCode(0)
	, m_BaseOfData(0)
	, m_ImageBase(0)
	, m_SectionAlignment(0)
	, m_FileAlignment(0)
	, MajorOperatingSystemVersion(0)
	, MinorOperatingSystemVersion(0)
	, m_MajorImageVersion(0)
	, m_MinorImageVEersion(0)
	, m_MajorSubsystemVersion(0)
	, m_MinorSubsystemVersion(0)
	, m_Win32VersionValue(0)
	, m_SizeOfImage(0)
	, m_SizeOfHeader(0)
	, m_CheckSum(0)
	, m_Subsystem(0)
	, m_DllCharacteristics(0)
	, m_SizeOfStackReserve(0)
	, m_SizeOfStackCommit(0)
	, m_SizeOfheapReserve(0)
	, m_SizeOfheapCommit(0)
	, m_LoaderFlags(0)
	, m_NumberOfRvaAndSize(0)
{
	this->poptHdr = poptHdr;

}

COptlonalDlg::~COptlonalDlg()
{
}

void COptlonalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Magic);
	DDX_Text(pDX, IDC_EDIT2, m_MajorLinkerVersion);
	DDX_Text(pDX, IDC_EDIT16, m_MinorLinkerVersion);
	DDX_Text(pDX, IDC_EDIT21, m_SizeOfCode);
	DDX_Text(pDX, IDC_EDIT22, m_SizeOfInitData);
	DDX_Text(pDX, IDC_EDIT23, m_SizeOfUninitData);
	DDX_Text(pDX, IDC_EDIT24, m_EntryPoint);
	DDX_Text(pDX, IDC_EDIT25, m_BaseOfCode);
	DDX_Text(pDX, IDC_EDIT6, m_BaseOfData);
	DDX_Text(pDX, IDC_EDIT7, m_ImageBase);
	DDX_Text(pDX, IDC_EDIT26, m_SectionAlignment);
	DDX_Text(pDX, IDC_EDIT27, m_FileAlignment);
	DDX_Text(pDX, IDC_EDIT28, MajorOperatingSystemVersion);
	DDX_Text(pDX, IDC_EDIT29, MinorOperatingSystemVersion);
	DDX_Text(pDX, IDC_EDIT30, m_MajorImageVersion);
	DDX_Text(pDX, IDC_EDIT14, m_MinorImageVEersion);
	DDX_Text(pDX, IDC_EDIT15, m_MajorSubsystemVersion);
	DDX_Text(pDX, IDC_EDIT32, m_MinorSubsystemVersion);
	DDX_Text(pDX, IDC_EDIT33, m_Win32VersionValue);
	DDX_Text(pDX, IDC_EDIT34, m_SizeOfImage);
	DDX_Text(pDX, IDC_EDIT35, m_SizeOfHeader);
	DDX_Text(pDX, IDC_EDIT36, m_CheckSum);
	DDX_Text(pDX, IDC_EDIT37, m_Subsystem);
	DDX_Text(pDX, IDC_EDIT38, m_DllCharacteristics);
	DDX_Text(pDX, IDC_EDIT39, m_SizeOfStackReserve);
	DDX_Text(pDX, IDC_EDIT40, m_SizeOfStackCommit);
	DDX_Text(pDX, IDC_EDIT41, m_SizeOfheapReserve);
	DDX_Text(pDX, IDC_EDIT42, m_SizeOfheapCommit);
	DDX_Text(pDX, IDC_EDIT43, m_LoaderFlags);
	DDX_Text(pDX, IDC_EDIT44, m_NumberOfRvaAndSize);
}


BEGIN_MESSAGE_MAP(COptlonalDlg, CDialogEx)
END_MESSAGE_MAP()


// COptlonalDlg 消息处理程序


BOOL COptlonalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Magic = poptHdr->Magic;							//pe文件类型(32位/64位)
	m_MajorImageVersion = poptHdr->MajorImageVersion;	//链接去主版本号
	m_MinorLinkerVersion = poptHdr->MinorLinkerVersion;	//链接去子版本号
	m_SizeOfCode = poptHdr->SizeOfCode;					//PE文件的代码段的字节数
	m_SizeOfInitData = poptHdr->SizeOfInitializedData;	//初始化数据大小
	m_SizeOfUninitData = poptHdr->SizeOfUninitializedData;//为初始化数据的大小
	m_EntryPoint = poptHdr->AddressOfEntryPoint;			//PE文件的入口地址(oep)
	m_BaseOfCode = poptHdr->BaseOfCode;					//PE文件数据段的地址(RVA)
	m_BaseOfData = poptHdr->BaseOfData;					//PE文件数据加载地址(RVA)
	m_ImageBase = poptHdr->ImageBase;					//建议加载基址,exe一般在0x400000,DLL一般在0x10000000
	m_SectionAlignment = poptHdr->SectionAlignment;		//PE文件被加载到内存是的对齐力度,一般0x1000
	m_FileAlignment = poptHdr->FileAlignment;			//文件对齐力度,一般在0x200,区段数据保存在磁盘上时候
	MajorOperatingSystemVersion = poptHdr->MajorOperatingSystemVersion;//操作系统主版本号
	MinorOperatingSystemVersion = poptHdr->MinorOperatingSystemVersion;//操作系统子版本号
	m_MajorImageVersion = poptHdr->MajorImageVersion;	//主版本号
	m_MinorImageVEersion = poptHdr->MinorImageVersion;	//子版本号
	m_MajorSubsystemVersion = poptHdr->MajorSubsystemVersion;//主系统主版本号
	m_MinorSubsystemVersion = poptHdr->MinorSubsystemVersion;//子系统汉本号
	m_Win32VersionValue = poptHdr->Win32VersionValue;		//版本号
	m_SizeOfImage = poptHdr->SizeOfImage;					//PE文件被加载进内存后的大小
	m_SizeOfHeader = poptHdr->SizeOfHeaders;					//整个头部的大小(包括DOS头,NT头,文件头,扩展头,区段表)
	m_CheckSum = poptHdr->CheckSum;							//CheckSum
	m_Subsystem = poptHdr->Subsystem;						//子系统
	m_DllCharacteristics = poptHdr->DllCharacteristics;		//PE文件的DLL属性
	m_SizeOfStackReserve = poptHdr->SizeOfStackReserve;		//线程栈的保留大小
	m_SizeOfStackCommit = poptHdr->SizeOfStackCommit;		//SizeOfStackCommit
	m_SizeOfheapReserve = poptHdr->SizeOfHeapReserve;		//SizeOfHeapReserve
	m_SizeOfheapCommit = poptHdr->SizeOfHeapCommit;			//SizeOfHeapCommit
	m_LoaderFlags = poptHdr->LoaderFlags;					//加载标志
	m_NumberOfRvaAndSize = poptHdr->NumberOfRvaAndSizes;		//无用

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
