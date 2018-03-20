// DerivedTabDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "DerivedTabDlg.h"
#include "afxdialogex.h"


// CDerivedTabDlg �Ի���

IMPLEMENT_DYNAMIC(CDerivedTabDlg, CDialogEx)

CDerivedTabDlg::CDerivedTabDlg(IMAGE_DOS_HEADER* pDosHdr, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_daoru, pParent)
{
	this->pDosHdr = pDosHdr;
}

CDerivedTabDlg::~CDerivedTabDlg()
{
}

void CDerivedTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_DerivedTab);
	DDX_Control(pDX, IDC_LIST2, m_daochu2);
}


BEGIN_MESSAGE_MAP(CDerivedTabDlg, CDialogEx)
END_MESSAGE_MAP()


// CDerivedTabDlg ��Ϣ�������
//ƫ��
DWORD CDerivedTabDlg::RVAToOffset(IMAGE_DOS_HEADER* pDos,
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

BOOL CDerivedTabDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString stac, stab, sta_aa, sta_bbb, sta_cc;

	wchar_t* zifu[] = { L"��ģ��",L"������ַ",L"�������" };
	for (int i = 0; i < 3; ++i)
	{
		m_DerivedTab.InsertColumn(i, zifu[i], 0, 100);

	}
	wchar_t* zifua[] = { L"������",L"������ַ",L"�������" };
	for (int i = 0; i < 3; ++i)
	{
		m_daochu2.InsertColumn(i, zifua[i], 0, 100);

	}
	int nCount = m_DerivedTab.GetItemCount();//��ȡ����ı��   
	int nCount2 = m_daochu2.GetItemCount();//��ȡ����ı��    
										  
	IMAGE_NT_HEADERS* pNtHdr = NULL;
	pNtHdr = (IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pDosHdr);
	// 3. �ҵ���չͷ
	IMAGE_OPTIONAL_HEADER* pOptHdr = NULL;
	pOptHdr = &pNtHdr->OptionalHeader;
	// 4. �ҵ�����Ŀ¼��
	IMAGE_DATA_DIRECTORY* pDataDir = NULL;
	pDataDir = pOptHdr->DataDirectory;
	// 5. �ҵ�������
	DWORD dwExpRva = pDataDir[0].VirtualAddress;

	// 5.1 �õ�RVA���ļ�ƫ��
	DWORD dwExpOfs = RVAToOffset(pDosHdr, dwExpRva);
	IMAGE_EXPORT_DIRECTORY* pExpTab = NULL;
	pExpTab = (IMAGE_EXPORT_DIRECTORY*)(dwExpOfs + (DWORD)pDosHdr);
	// 1. ����DLL����

	DWORD dwNameOfs = RVAToOffset(pDosHdr, pExpTab->Name);
	char* pDllName = (char*)(dwNameOfs + (DWORD)pDosHdr);
	printf("Dll�� : %s\n", pDllName);
	stac = (CString)pDllName;

	m_DerivedTab.InsertItem(0, stac);//������
	DWORD dwExpAddrTabOfs = RVAToOffset(pDosHdr, pExpTab->AddressOfFunctions);
	DWORD dwExpNameTabOfs = RVAToOffset(pDosHdr, pExpTab->AddressOfNames);
	DWORD dwExpOrdTabOfs =  RVAToOffset(pDosHdr, pExpTab->AddressOfNameOrdinals);
	// �����еĵ�ַ��
	DWORD* pExpAddr = (DWORD*)(dwExpAddrTabOfs + (DWORD)pDosHdr);
	DWORD* pExpName = (DWORD*)(dwExpNameTabOfs + (DWORD)pDosHdr);
	// ��ű���WORD���͵�����
	DWORD* pExpOrd = (DWORD*)(dwExpOrdTabOfs + (DWORD)pDosHdr);
	// �������еĺ�����ַ
	for (int i = 0; i < pExpTab->NumberOfFunctions; ++i)
	{
		printf("������ַ(RVA):[%08X]\n", pExpAddr[i]);
		sta_aa.Format(L"%x", pExpAddr[i]);
		m_daochu2.InsertItem(1, sta_aa);   //��������ַ
										   // ���ҵ�ǰ�������ĵ�ַ��û������
		int j = 0;
		for (; j < pExpTab->NumberOfNames; ++j)
		{
			if (i == pExpOrd[j])
			{
				break;
			}

			DWORD dwNameRva = pExpName[j];
			DWORD dwNameOfs = RVAToOffset(pDosHdr, dwNameRva);
			char* pFunctionName = nullptr;
			pFunctionName = (char*)(dwNameOfs + (DWORD)pDosHdr);
			printf("\t������:[%s]\n", pFunctionName);
			sta_bbb = (CString)pFunctionName;
			m_daochu2.SetItemText(nCount2, 1, sta_bbb);   //������

		}

		if (j < pExpTab->NumberOfNames - 1)
		{
		
			// �ҵ�һ���������Ƶ�Rva
			DWORD dwNameRva = pExpName[j];
			DWORD dwNameOfs = RVAToOffset(pDosHdr, dwNameRva);
			char* pFunctionName = nullptr;
			pFunctionName = (char*)(dwNameOfs + (DWORD)pDosHdr);
			printf("\t������:[%s]\n", pFunctionName);
			sta_bbb = (CString)pFunctionName;
			m_daochu2.SetItemText(nCount2, 1, sta_bbb);

		}
		else
		{
	
			if (pExpAddr[i] != 0)
			{
				// �����ĵ������:
				printf("\t�������[%04X]\n", pExpTab->Base + i);
				sta_cc.Format(L"%d", pExpTab->Base + i);
				m_daochu2.SetItemText(nCount2, 2, sta_cc);

			}
		}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
