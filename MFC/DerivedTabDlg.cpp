// DerivedTabDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "DerivedTabDlg.h"
#include "afxdialogex.h"


// CDerivedTabDlg 对话框

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


// CDerivedTabDlg 消息处理程序
//偏移
DWORD CDerivedTabDlg::RVAToOffset(IMAGE_DOS_HEADER* pDos,
	DWORD dwRva)
{
	IMAGE_SECTION_HEADER* pScnHdr;

	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)(pDos->e_lfanew + (DWORD)pDos);

	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);
	DWORD dwNumberOfScn = pNtHdr->FileHeader.NumberOfSections;

	// 1. 遍历所有区段找到所在区段
	for (int i = 0; i < dwNumberOfScn; ++i)
	{
		DWORD dwEndOfSection = pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData;
		// 判断这个RVA是否在一个区段的范围内
		if (dwRva >= pScnHdr[i].VirtualAddress
			&& dwRva < dwEndOfSection)
		{
			// 2. 计算该RVA在区段内的偏移:rva 减去首地址
			DWORD dwOffset = dwRva - pScnHdr[i].VirtualAddress;
			// 3. 将区段内偏移加上区段的文件开始偏移
			return dwOffset + pScnHdr[i].PointerToRawData;
		}
	}
	return -1;
}

BOOL CDerivedTabDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString stac, stab, sta_aa, sta_bbb, sta_cc;

	wchar_t* zifu[] = { L"主模块",L"函数地址",L"导入序号" };
	for (int i = 0; i < 3; ++i)
	{
		m_DerivedTab.InsertColumn(i, zifu[i], 0, 100);

	}
	wchar_t* zifua[] = { L"函数名",L"函数地址",L"导入序号" };
	for (int i = 0; i < 3; ++i)
	{
		m_daochu2.InsertColumn(i, zifua[i], 0, 100);

	}
	int nCount = m_DerivedTab.GetItemCount();//获取对象的编号   
	int nCount2 = m_daochu2.GetItemCount();//获取对象的编号    
										  
	IMAGE_NT_HEADERS* pNtHdr = NULL;
	pNtHdr = (IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pDosHdr);
	// 3. 找到扩展头
	IMAGE_OPTIONAL_HEADER* pOptHdr = NULL;
	pOptHdr = &pNtHdr->OptionalHeader;
	// 4. 找到数据目录表
	IMAGE_DATA_DIRECTORY* pDataDir = NULL;
	pDataDir = pOptHdr->DataDirectory;
	// 5. 找到导出表
	DWORD dwExpRva = pDataDir[0].VirtualAddress;

	// 5.1 得到RVA的文件偏移
	DWORD dwExpOfs = RVAToOffset(pDosHdr, dwExpRva);
	IMAGE_EXPORT_DIRECTORY* pExpTab = NULL;
	pExpTab = (IMAGE_EXPORT_DIRECTORY*)(dwExpOfs + (DWORD)pDosHdr);
	// 1. 解析DLL的名

	DWORD dwNameOfs = RVAToOffset(pDosHdr, pExpTab->Name);
	char* pDllName = (char*)(dwNameOfs + (DWORD)pDosHdr);
	printf("Dll名 : %s\n", pDllName);
	stac = (CString)pDllName;

	m_DerivedTab.InsertItem(0, stac);//插入行
	DWORD dwExpAddrTabOfs = RVAToOffset(pDosHdr, pExpTab->AddressOfFunctions);
	DWORD dwExpNameTabOfs = RVAToOffset(pDosHdr, pExpTab->AddressOfNames);
	DWORD dwExpOrdTabOfs =  RVAToOffset(pDosHdr, pExpTab->AddressOfNameOrdinals);
	// 三张中的地址表
	DWORD* pExpAddr = (DWORD*)(dwExpAddrTabOfs + (DWORD)pDosHdr);
	DWORD* pExpName = (DWORD*)(dwExpNameTabOfs + (DWORD)pDosHdr);
	// 序号表是WORD类型的数组
	DWORD* pExpOrd = (DWORD*)(dwExpOrdTabOfs + (DWORD)pDosHdr);
	// 遍历所有的函数地址
	for (int i = 0; i < pExpTab->NumberOfFunctions; ++i)
	{
		printf("函数地址(RVA):[%08X]\n", pExpAddr[i]);
		sta_aa.Format(L"%x", pExpAddr[i]);
		m_daochu2.InsertItem(1, sta_aa);   //函数名地址
										   // 查找当前遍历到的地址有没有名称
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
			printf("\t函数名:[%s]\n", pFunctionName);
			sta_bbb = (CString)pFunctionName;
			m_daochu2.SetItemText(nCount2, 1, sta_bbb);   //函数名

		}

		if (j < pExpTab->NumberOfNames - 1)
		{
		
			// 找到一个函数名称的Rva
			DWORD dwNameRva = pExpName[j];
			DWORD dwNameOfs = RVAToOffset(pDosHdr, dwNameRva);
			char* pFunctionName = nullptr;
			pFunctionName = (char*)(dwNameOfs + (DWORD)pDosHdr);
			printf("\t函数名:[%s]\n", pFunctionName);
			sta_bbb = (CString)pFunctionName;
			m_daochu2.SetItemText(nCount2, 1, sta_bbb);

		}
		else
		{
	
			if (pExpAddr[i] != 0)
			{
				// 函数的导出序号:
				printf("\t导出序号[%04X]\n", pExpTab->Base + i);
				sta_cc.Format(L"%d", pExpTab->Base + i);
				m_daochu2.SetItemText(nCount2, 2, sta_cc);

			}
		}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
