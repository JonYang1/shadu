// ResouDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "ResouDlg.h"
#include "afxdialogex.h"


// CResouDlg �Ի���

IMPLEMENT_DYNAMIC(CResouDlg, CDialogEx)

CResouDlg::CResouDlg(IMAGE_DOS_HEADER* pDosHdr, HANDLE hFile, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{
	this->pDosHdr = pDosHdr;
	this->hFile = hFile;
}

CResouDlg::~CResouDlg()
{
}

void CResouDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtr_k);
}


BEGIN_MESSAGE_MAP(CResouDlg, CDialogEx)
END_MESSAGE_MAP()


// CResouDlg ��Ϣ�������
void CResouDlg::parseResourcesTable(DWORD dwResRootDirAddr,IMAGE_RESOURCE_DIRECTORY* pResDir,int nDeep )
{
	DWORD dwEntryCount = pResDir->NumberOfIdEntries + pResDir->NumberOfNamedEntries;
	
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pResDirEntry;
	pResDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pResDir + 1);

	WCHAR buff[512] = { 0 };

	if (nDeep == 1)
	{ 		 
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			
			if (pResDirEntry[i].NameIsString == 1) {
				
				IMAGE_RESOURCE_DIR_STRING_U* pTypeName;
				
				pTypeName =
					(IMAGE_RESOURCE_DIR_STRING_U*)
					(pResDirEntry[i].NameOffset + dwResRootDirAddr);


				wcsncpy_s(buff, pTypeName->NameString, pTypeName->Length);
				wprintf(L"\"%s\"\n", buff);
				CString str = buff;
				HTREEITEM hRoot = m_TreeCtr_k.InsertItem(str);
			}
			else {
				
				char *szType[] =
				{
					("") ,			
					("���") ,		
					("λͼ") ,		
					("ͼ��") ,		
					("�˵�") ,		
					("�Ի���") ,		
					("�ַ�����") ,	
					("����Ŀ¼") ,	
					("����") ,		
					("���ټ�") ,		
					("RC����") ,		
					("��Ϣ��") ,		
					("�����") ,		
					("") ,			
					("ͼ����") ,		
					"" ,				
					("�汾��Ϣ") ,		
					("�Ի������Ŀ¼") ,	
					"" ,					
					"" ,					
					"" ,					
					"" ,					
					"" ,					
					("HTML") ,			
					("�嵥�ļ�")			
				};

				if (pResDirEntry[i].Id >= 1 && pResDirEntry[i].Id <= 24) {

					printf("%s\n", szType[pResDirEntry[i].Id]);
					CString aa = (CString)szType[pResDirEntry[i].Id];
					HTREEITEM hRoot = m_TreeCtr_k.InsertItem(aa);


				}
				else {
					printf("%d\n", pResDirEntry[i].Id);
					CString a;
					a.Format(L"%d\n", pResDirEntry[i].Id);
					HTREEITEM hChild = m_TreeCtr_k.InsertItem(a, hRoot, TVI_LAST);
				}
			}

			
			IMAGE_RESOURCE_DIRECTORY* pNextDir =
				(IMAGE_RESOURCE_DIRECTORY*)(pResDirEntry[i].OffsetToDirectory + dwResRootDirAddr);

			parseResourcesTable(dwResRootDirAddr, pNextDir, nDeep + 1);
		}

	}/*�ڶ���*/
	else if (nDeep == 2) { 

		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			// ��ԴĿ¼�ĵڶ���, 
			if (pResDirEntry[i].NameIsString) {
				
				IMAGE_RESOURCE_DIR_STRING_U* pTypeName;
				pTypeName =
					(IMAGE_RESOURCE_DIR_STRING_U*)
					(pResDirEntry[i].NameOffset + dwResRootDirAddr);

				wcsncpy_s(buff, pTypeName->NameString, pTypeName->Length);
				wprintf(L" |- \"%s\"\n", buff);
				HTREEITEM hChild = m_TreeCtr_k.InsertItem(buff, hRoot);
			}
			else {
				printf(" |- %d\n", pResDirEntry[i].Id);
				CString M_BIANL;
				M_BIANL.Format(L" --- %d\n", pResDirEntry[i].Id);
				HTREEITEM hChild = m_TreeCtr_k.InsertItem(M_BIANL, hRoot);
			}

			// ������һ��Ŀ¼
			IMAGE_RESOURCE_DIRECTORY* pNextDir =
				(IMAGE_RESOURCE_DIRECTORY*)(pResDirEntry[i].OffsetToDirectory + dwResRootDirAddr);

			parseResourcesTable(dwResRootDirAddr, pNextDir, nDeep + 1);
		}
	}
	else if (nDeep == 3) {/*������*/
		IMAGE_RESOURCE_DATA_ENTRY* pResDataEntry = 0;

		if (pResDirEntry->DataIsDirectory == 0) {

			pResDataEntry =
				(IMAGE_RESOURCE_DATA_ENTRY*)(pResDirEntry->OffsetToData + dwResRootDirAddr);

	
			LPVOID pData = (LPVOID)(pResDataEntry->OffsetToData + (DWORD)GetModuleHandle(NULL));
		}
	}
}

// CResouDlg ��Ϣ�������
DWORD CResouDlg::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
{
	IMAGE_SECTION_HEADER* pScnHdr;
	IMAGE_NT_HEADERS* pNtHdr = (IMAGE_NT_HEADERS*)(pDos->e_lfanew + (DWORD)pDos);//�õ�NTͷ
	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);//���α�
	DWORD dwNumberOfScn = pNtHdr->FileHeader.NumberOfSections;//�õ����εĸ���
															 
	for (int i = 0; i < dwNumberOfScn; ++i)
	{
		//�ж����RVA�Ƿ���һ�����εķ�Χ��
		DWORD dwEndOfSection = pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData;
		if (dwRva >= pScnHdr[i].VirtualAddress && dwRva < dwEndOfSection)
		{
			//����RVA�������ڵ�ƫ��:rva��ȥ�׵�ַ
			DWORD dwOffset = dwRva - pScnHdr[i].VirtualAddress;
			//  ��������ƫ�Ƽ������ε��ļ���ʼƫ��
			return dwOffset + pScnHdr[i].PointerToRawData;
		}
	}
	return 0;

}

BOOL CResouDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_TreeCtr_k.SetBkColor(RGB(255, 222, 0));

	// ��ȡNtͷ
	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pDosHdr);//??????????????//
																// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	IMAGE_RESOURCE_DIRECTORY* pResRoot = NULL;

	//��ȡ��Դ��Ŀ¼��RVA
	DWORD dwResRootRva = pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;

	//ƫ��ת��
	pResRoot = (IMAGE_RESOURCE_DIRECTORY*)(RVAToOffset(pDosHdr, dwResRootRva) + (DWORD)pDosHdr);
	parseResourcesTable((DWORD)pResRoot, pResRoot, 1);
	return TRUE;  // return TRUE unless you set the focus to a control
				 
}
