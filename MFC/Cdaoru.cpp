// Cdaoru.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "Cdaoru.h"
#include "afxdialogex.h"


// Cdaoru �Ի���

IMPLEMENT_DYNAMIC(Cdaoru, CDialogEx)

Cdaoru::Cdaoru(IMAGE_DOS_HEADER* pDosHdr, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_daoru2, pParent)
{
	this->pDosHdr = pDosHdr;
}

Cdaoru::~Cdaoru()
{
}
// Cdaoru ��Ϣ�������
/*!
* \brief : RVAToOffset
* \return: DWORD	rva��Ӧ���ļ�ƫ��,���ת��ʧ��,����-1
* \param : IMAGE_DOS_HEADER * pDos rva���ڵ�pe�ļ�
* \param : DWORD dwRva ��ת����RVA
*/
DWORD Cdaoru::RVAToOffset(IMAGE_DOS_HEADER* pDos,
	DWORD dwRva)
{
	IMAGE_SECTION_HEADER* pScnHdr;

	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)(pDos->e_lfanew + (DWORD)pDos);

	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);
	DWORD dwNumberOfScn = pNtHdr->FileHeader.NumberOfSections;

	// 1. �������������ҵ���������
	for (int i = 0; i < dwNumberOfScn; ++i)
	{
		DWORD dwEndOfSection = pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData;
		// �ж����RVA�Ƿ���һ�����εķ�Χ��
		if (dwRva >= pScnHdr[i].VirtualAddress
			&& dwRva < dwEndOfSection)
		{
			// 2. �����RVA�������ڵ�ƫ��:rva ��ȥ�׵�ַ
			DWORD dwOffset = dwRva - pScnHdr[i].VirtualAddress;
			// 3. ��������ƫ�Ƽ������ε��ļ���ʼƫ��
			return dwOffset + pScnHdr[i].PointerToRawData;
		}
	}
	return -1;
}
void Cdaoru::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_daoru);
	DDX_Control(pDX, IDC_LIST2, m_mokuaim);
}


BEGIN_MESSAGE_MAP(Cdaoru, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &Cdaoru::OnNMClickList1)
END_MESSAGE_MAP()


// Cdaoru ��Ϣ�������


BOOL Cdaoru::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString sta_aa, sta_bb, sta_cc;
	CString stb_aa, stb_bb, stb_cc;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	wchar_t* zifu[] = { L"ģ����",L"�������",L"��ַ:",L"�����DLL" };
	for (int i = 0; i < 4; ++i)
	{
		m_daoru.InsertColumn(i, zifu[i], 0, 100);
	}
	wchar_t* zifua[] = { L"�������",L"��������",L"��ַ:",L"�����DLL" };
	for (int i = 0; i < 4; ++i)
	{
		m_mokuaim.InsertColumn(i, zifua[i], 0, 100);
	}
	int nCount = m_daoru.GetItemCount();//��ȡ����ı��  
	int nCount2 = m_mokuaim.GetItemCount();
	IMAGE_NT_HEADERS* pNtHdr = NULL;
	pNtHdr = (IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pDosHdr);
	// 3. �ҵ���չͷ
	IMAGE_OPTIONAL_HEADER* pOptHdr = NULL;
	pOptHdr = &pNtHdr->OptionalHeader;

	// 4. �ҵ�����Ŀ¼��
	IMAGE_DATA_DIRECTORY* pDataDir = NULL;
	pDataDir = pOptHdr->DataDirectory;
	// 5. �õ�������RVA
	DWORD dwImpRva = pDataDir[1].VirtualAddress;
	IMAGE_IMPORT_DESCRIPTOR* pImpArray;
	pImpArray = (IMAGE_IMPORT_DESCRIPTOR*)(RVAToOffset(pDosHdr, dwImpRva) + (DWORD)pDosHdr);
	while (pImpArray->Name != 0)
	{
		// Dll������
		DWORD dwNameOfs = RVAToOffset(pDosHdr, pImpArray->Name);
		char* pDllName = (char*)(dwNameOfs + (DWORD)pDosHdr);
		printf("DLL: [%s]\n", pDllName);
		sta_aa = (CString)pDllName;
		m_daoru.InsertItem(nCount, sta_aa);										 
		pImpArray->OriginalFirstThunk;
		pImpArray->FirstThunk;
		DWORD INTOfs = RVAToOffset(pDosHdr, pImpArray->FirstThunk);
		DWORD IATOfs = RVAToOffset(pDosHdr, pImpArray->FirstThunk);
		IMAGE_THUNK_DATA* pInt = NULL;
		IMAGE_THUNK_DATA* pIat = NULL;
		/*
		typedef struct _IMAGE_THUNK_DATA32 {
		union {
		DWORD ForwarderString;      // PBYTE
		DWORD Function;             // PDWORD
		DWORD Ordinal;
		DWORD AddressOfData;        // PIMAGE_IMPORT_BY_NAME
		} u1;
		} IMAGE_THUNK_DATA32;
		*/
		pInt = (IMAGE_THUNK_DATA*)(INTOfs + (DWORD)pDosHdr);
		pIat = (IMAGE_THUNK_DATA*)(IATOfs + (DWORD)pDosHdr);
		while (pInt->u1.Function != 0)
		{
			if (IMAGE_SNAP_BY_ORDINAL32(pInt->u1.Function))
			{
				sta_aa.Format(L"%d", pInt->u1.Ordinal & 0xFFFF);
				m_daoru.SetItemText(nCount, 1, sta_aa);//������
			}
			else
			{
				IMAGE_IMPORT_BY_NAME* pImpName;
				DWORD dwImpNameOfs = RVAToOffset(pDosHdr, pInt->u1.Function);
				pImpName = (IMAGE_IMPORT_BY_NAME*)
					(dwImpNameOfs + (DWORD)pDosHdr);
				stb_aa.Format(L"%d", pImpName->Hint);
				stb_bb = (CString)pImpName->Name;
				m_mokuaim.InsertItem(0, stb_aa);
				m_mokuaim.SetItemText(nCount2, 1, stb_bb);
			}
			++pInt;
		}
		++pImpArray;
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//��굥����ȡ��ֵ
void Cdaoru::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
