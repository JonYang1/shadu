// ResouDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "ResouDlg.h"
#include "afxdialogex.h"


// CResouDlg 对话框

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


// CResouDlg 消息处理程序
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
					("光标") ,		
					("位图") ,		
					("图标") ,		
					("菜单") ,		
					("对话框") ,		
					("字符串表") ,	
					("字体目录") ,	
					("字体") ,		
					("加速键") ,		
					("RC数据") ,		
					("消息表") ,		
					("光标组") ,		
					("") ,			
					("图标组") ,		
					"" ,				
					("版本信息") ,		
					("对话框包含目录") ,	
					"" ,					
					"" ,					
					"" ,					
					"" ,					
					"" ,					
					("HTML") ,			
					("清单文件")			
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

	}/*第二层*/
	else if (nDeep == 2) { 

		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			// 资源目录的第二层, 
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

			// 解析下一层目录
			IMAGE_RESOURCE_DIRECTORY* pNextDir =
				(IMAGE_RESOURCE_DIRECTORY*)(pResDirEntry[i].OffsetToDirectory + dwResRootDirAddr);

			parseResourcesTable(dwResRootDirAddr, pNextDir, nDeep + 1);
		}
	}
	else if (nDeep == 3) {/*第三层*/
		IMAGE_RESOURCE_DATA_ENTRY* pResDataEntry = 0;

		if (pResDirEntry->DataIsDirectory == 0) {

			pResDataEntry =
				(IMAGE_RESOURCE_DATA_ENTRY*)(pResDirEntry->OffsetToData + dwResRootDirAddr);

	
			LPVOID pData = (LPVOID)(pResDataEntry->OffsetToData + (DWORD)GetModuleHandle(NULL));
		}
	}
}

// CResouDlg 消息处理程序
DWORD CResouDlg::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
{
	IMAGE_SECTION_HEADER* pScnHdr;
	IMAGE_NT_HEADERS* pNtHdr = (IMAGE_NT_HEADERS*)(pDos->e_lfanew + (DWORD)pDos);//得到NT头
	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);//区段表
	DWORD dwNumberOfScn = pNtHdr->FileHeader.NumberOfSections;//得到区段的个数
															 
	for (int i = 0; i < dwNumberOfScn; ++i)
	{
		//判断这个RVA是否在一个区段的范围内
		DWORD dwEndOfSection = pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData;
		if (dwRva >= pScnHdr[i].VirtualAddress && dwRva < dwEndOfSection)
		{
			//计算RVA在区段内的偏移:rva减去首地址
			DWORD dwOffset = dwRva - pScnHdr[i].VirtualAddress;
			//  将区段内偏移加上区段的文件开始偏移
			return dwOffset + pScnHdr[i].PointerToRawData;
		}
	}
	return 0;

}

BOOL CResouDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_TreeCtr_k.SetBkColor(RGB(255, 222, 0));

	// 获取Nt头
	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pDosHdr);//??????????????//
																// TODO:  在此添加额外的初始化
	IMAGE_RESOURCE_DIRECTORY* pResRoot = NULL;

	//获取资源根目录的RVA
	DWORD dwResRootRva = pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;

	//偏移转换
	pResRoot = (IMAGE_RESOURCE_DIRECTORY*)(RVAToOffset(pDosHdr, dwResRootRva) + (DWORD)pDosHdr);
	parseResourcesTable((DWORD)pResRoot, pResRoot, 1);
	return TRUE;  // return TRUE unless you set the focus to a control
				 
}
