// CongDingWeiDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "CongDingWeiDlg.h"
#include "afxdialogex.h"


// CongDingWeiDlg �Ի���

IMPLEMENT_DYNAMIC(CongDingWeiDlg, CDialogEx)

CongDingWeiDlg::CongDingWeiDlg(IMAGE_DOS_HEADER* pDosHdr, BYTE* pBuf, IMAGE_FILE_HEADER* pFileHdr, IMAGE_SECTION_HEADER* pScnHdr, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG7, pParent)
{
	this->pDosHdr = pDosHdr;
	this->pFileBuff = pBuf;
	this->pFileHdr = pFileHdr;
	this->pScnHdr = pScnHdr;
}

CongDingWeiDlg::~CongDingWeiDlg()
{
}

void CongDingWeiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_tou);
	DDX_Control(pDX, IDC_LIST2, m_wei);
}


BEGIN_MESSAGE_MAP(CongDingWeiDlg, CDialogEx)
END_MESSAGE_MAP()

// CongDingWeiDlg ��Ϣ�������
DWORD CongDingWeiDlg::RVAToOffset(LPVOID pDos, DWORD dwRva) {
	// �ҵ���Ҫת����RVA���ڵ�����,Ȼ�����ƫ��.
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)pDos;

	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)((DWORD)pDos + pDosHdr->e_lfanew);

	IMAGE_SECTION_HEADER* pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);

	DWORD dwScnCount = pNtHdr->FileHeader.NumberOfSections;

	for (DWORD i = 0; i < dwScnCount; i++) {
		// �ж�RVA�Ƿ���һ��������
		if (dwRva >= pScnHdr[i].VirtualAddress
			&& dwRva < pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData) {

			return dwRva - pScnHdr[i].VirtualAddress + pScnHdr[i].PointerToRawData;
		}
	}
	return -1;
}


BOOL CongDingWeiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString sta_a, sta_b, sta_c, sta_d;
	int count = 1;
	int nCount = 0;
	int nCount2 = 0;
	wchar_t* fu[] = { L"�Ӷ�λ�ĸ���",L"����",L"�����ַ",L"ƫ��" };
	for (int i = 0; i<4; i++)
	{
		m_tou.InsertColumn(i, fu[i], LVCFMT_LEFT, 120);
	}
	m_wei.InsertColumn(0, { L"�����д�ŵ�����" }, LVCFMT_LEFT, 320);
	IMAGE_NT_HEADERS *pNtHdr =(IMAGE_NT_HEADERS*)((DWORD)pDosHdr + pDosHdr->e_lfanew);
	// �õ���չͷ
	IMAGE_OPTIONAL_HEADER* pOptHdr = &pNtHdr->OptionalHeader;
	// �õ�����Ŀ¼��
	IMAGE_DATA_DIRECTORY* pDataDir = pOptHdr->DataDirectory;
	// ��ȡ�ض�λ��
	/*
	typedef struct _IMAGE_BASE_RELOCATION {
	DWORD   VirtualAddress; // �����ʼRVA
	DWORD   SizeOfBlock;    // ������Ĵ�С(����VirtualAddress��SizeOfBlock�����ֶεĴ�С)
	//WORD    TypeOffset[1];
	} IMAGE_BASE_RELOCATION;
	*/
	IMAGE_BASE_RELOCATION* pBaseRel;
	pBaseRel = (IMAGE_BASE_RELOCATION*)(RVAToOffset(pFileBuff, pDataDir[5].VirtualAddress) + (DWORD)pFileBuff);
	CString aa;
	while (pBaseRel->VirtualAddress != 0)
	{
		//DWORD dwCount = (pBaseRel->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);
		DWORD dwCount = pBaseRel->SizeOfBlock;
		sta_a.Format(L"%d", dwCount);
		m_tou.InsertItem(nCount, sta_a); //�Ӷ�λ����
		printf("%08X\n", pBaseRel->VirtualAddress);
		sta_c.Format(L"%08X", pBaseRel->VirtualAddress);//��ַ
		m_tou.SetItemText(nCount, 2, sta_c);
		////�ж�����
		INT nA = pFileHdr->NumberOfSections;// ���α�Ĵ�С
		STDA.Format(L"%x", pScnHdr->Name);
		pScnHdr++;//�ӽṹ���С
		if (pBaseRel->VirtualAddress >= pFileHdr->NumberOfSections)//���޸�
		{
			STDA = CString(pScnHdr->Name);
			m_tou.SetItemText(nCount, 1, STDA);
		}
		sta_b.Format(L"%08X", pBaseRel->VirtualAddress);//��ַ	
		aa = "";
		//TypeOffset ���������(�Լ�����Ľṹ��)
		TypeOffset* pTypeOffset = (TypeOffset*)(pBaseRel + 1);
		for (DWORD i = 0; i < dwCount; ++i)
		{
			if (pTypeOffset[i].Type == IMAGE_REL_BASED_HIGHLOW) {
				// �õ���Ҫ�޸���RVA
				DWORD dwRva = pBaseRel->VirtualAddress + pTypeOffset[i].Offset; 12;  // (1) ��СΪ12Bit���ض�λƫ��
				DWORD dwOffset = RVAToOffset(pFileBuff, dwRva);
				DWORD* pAddress = (DWORD*)(dwOffset + (DWORD)pFileBuff);
				sta_d.Format(L"%06X", pTypeOffset[i].Offset);//��ַ		
				aa += ",";
				aa += +sta_d;	
			}
			
		}
		m_wei.InsertItem(0, aa);
		// �ҵ���һ���ض�λ��
		pBaseRel =(IMAGE_BASE_RELOCATION*)((DWORD)pBaseRel + pBaseRel->SizeOfBlock);
		count++;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



// CongDingWeiDlg ��Ϣ�������
