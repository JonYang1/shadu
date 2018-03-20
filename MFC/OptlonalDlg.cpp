// OptlonalDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "OptlonalDlg.h"
#include "afxdialogex.h"


// COptlonalDlg �Ի���

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


// COptlonalDlg ��Ϣ�������


BOOL COptlonalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Magic = poptHdr->Magic;							//pe�ļ�����(32λ/64λ)
	m_MajorImageVersion = poptHdr->MajorImageVersion;	//����ȥ���汾��
	m_MinorLinkerVersion = poptHdr->MinorLinkerVersion;	//����ȥ�Ӱ汾��
	m_SizeOfCode = poptHdr->SizeOfCode;					//PE�ļ��Ĵ���ε��ֽ���
	m_SizeOfInitData = poptHdr->SizeOfInitializedData;	//��ʼ�����ݴ�С
	m_SizeOfUninitData = poptHdr->SizeOfUninitializedData;//Ϊ��ʼ�����ݵĴ�С
	m_EntryPoint = poptHdr->AddressOfEntryPoint;			//PE�ļ�����ڵ�ַ(oep)
	m_BaseOfCode = poptHdr->BaseOfCode;					//PE�ļ����ݶεĵ�ַ(RVA)
	m_BaseOfData = poptHdr->BaseOfData;					//PE�ļ����ݼ��ص�ַ(RVA)
	m_ImageBase = poptHdr->ImageBase;					//������ػ�ַ,exeһ����0x400000,DLLһ����0x10000000
	m_SectionAlignment = poptHdr->SectionAlignment;		//PE�ļ������ص��ڴ��ǵĶ�������,һ��0x1000
	m_FileAlignment = poptHdr->FileAlignment;			//�ļ���������,һ����0x200,�������ݱ����ڴ�����ʱ��
	MajorOperatingSystemVersion = poptHdr->MajorOperatingSystemVersion;//����ϵͳ���汾��
	MinorOperatingSystemVersion = poptHdr->MinorOperatingSystemVersion;//����ϵͳ�Ӱ汾��
	m_MajorImageVersion = poptHdr->MajorImageVersion;	//���汾��
	m_MinorImageVEersion = poptHdr->MinorImageVersion;	//�Ӱ汾��
	m_MajorSubsystemVersion = poptHdr->MajorSubsystemVersion;//��ϵͳ���汾��
	m_MinorSubsystemVersion = poptHdr->MinorSubsystemVersion;//��ϵͳ������
	m_Win32VersionValue = poptHdr->Win32VersionValue;		//�汾��
	m_SizeOfImage = poptHdr->SizeOfImage;					//PE�ļ������ؽ��ڴ��Ĵ�С
	m_SizeOfHeader = poptHdr->SizeOfHeaders;					//����ͷ���Ĵ�С(����DOSͷ,NTͷ,�ļ�ͷ,��չͷ,���α�)
	m_CheckSum = poptHdr->CheckSum;							//CheckSum
	m_Subsystem = poptHdr->Subsystem;						//��ϵͳ
	m_DllCharacteristics = poptHdr->DllCharacteristics;		//PE�ļ���DLL����
	m_SizeOfStackReserve = poptHdr->SizeOfStackReserve;		//�߳�ջ�ı�����С
	m_SizeOfStackCommit = poptHdr->SizeOfStackCommit;		//SizeOfStackCommit
	m_SizeOfheapReserve = poptHdr->SizeOfHeapReserve;		//SizeOfHeapReserve
	m_SizeOfheapCommit = poptHdr->SizeOfHeapCommit;			//SizeOfHeapCommit
	m_LoaderFlags = poptHdr->LoaderFlags;					//���ر�־
	m_NumberOfRvaAndSize = poptHdr->NumberOfRvaAndSizes;		//����

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
