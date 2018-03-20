// PeZhu.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "PeZhu.h"
#include "afxdialogex.h"
#include "PEDosDlg.h"
#include "OptlonalDlg.h"
#include "FileDlg.h"
#include "ResouDlg.h"
#include "MuluDlg.h"
#include "DerivedTabDlg.h"
#include "Cdaoru.h"
#include "CongDingWeiDlg.h"


// CPeZhu 对话框

IMPLEMENT_DYNAMIC(CPeZhu, CDialogEx)

CPeZhu::CPeZhu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CPeZhu::~CPeZhu()
{
}

void CPeZhu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPeZhu, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPeZhu::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPeZhu::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPeZhu::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPeZhu::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPeZhu::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CPeZhu::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CPeZhu::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CPeZhu::OnBnClickedButton8)
END_MESSAGE_MAP()

LPVOID CPeZhu::MyGetProcAddress(HMODULE hMod,
	char* pszFunctionName)
{
	// 1. 找到Dos头
	IMAGE_DOS_HEADER* pDosHdr;
	pDosHdr = (IMAGE_DOS_HEADER*)hMod;
	// 2. 找到Nt头
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
	// 5.1 得到
	IMAGE_EXPORT_DIRECTORY* pExpTab = NULL;
	pExpTab = (IMAGE_EXPORT_DIRECTORY*)(dwExpRva + (DWORD)pDosHdr);
	char* pName = (char*)(pExpTab->Name + (DWORD)pDosHdr);
	DWORD* pExpAddr = (DWORD*)(pExpTab->AddressOfFunctions + (DWORD)hMod);
	DWORD* pExpName = (DWORD*)(pExpTab->AddressOfNames + (DWORD)hMod);
	WORD*  pExpOrd = (WORD*)(pExpTab->AddressOfNameOrdinals + (DWORD)hMod);;
	for (int i = 0; i < pExpTab->NumberOfNames; ++i)
	{
		char* pName = (char*)(pExpName[i] + (DWORD)pDosHdr);
		if (strcmp(pszFunctionName, pName) == 0)
		{
			DWORD dwAddrTabIndex = pExpOrd[i];
			return (LPVOID(pExpAddr[dwAddrTabIndex] + (DWORD)hMod));
		}
	}
	return 0;
}
//************************************
// 方    法:   RVAToOffset
// 函数名称:   RVAToOffset
// 使用权限:   public 
// 返 回 值:   DWORD	rva对应的文件偏移,如果转换失败,返回-1
// 使用说明:   IMAGE_DOS_HEADER * pDos rva所在的pe文件 求偏移位置
// 参    数:   IMAGE_DOS_HEADER * pDos
// 参    数:   DWORD dwRva 待转换的RVA
//************************************
DWORD CPeZhu::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
{
	IMAGE_SECTION_HEADER* pScnHdr;
	IMAGE_NT_HEADERS* pNtHdr = (IMAGE_NT_HEADERS*)(pDos->e_lfanew + (DWORD)pDos);//得到NT头的开始
	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);//区段表.txt
	DWORD dwNumberOfScn = pNtHdr->FileHeader.NumberOfSections;//得到区段的个数
															  // 遍历所有区段找到所在区段
	for (int i = 0; i<dwNumberOfScn; ++i)
	{
		//判断这个RVA是否在一个区段的范围内
		DWORD dwEndOfSection = pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData;
		if (dwRva >= pScnHdr[i].VirtualAddress && dwRva<dwEndOfSection)
		{
			//计算RVA在区段内的偏移:rva减去首地址
			DWORD dwOffset = dwRva - pScnHdr[i].VirtualAddress;
			//  将区段内偏移加上区段的文件开始偏移
			return dwOffset + pScnHdr[i].PointerToRawData;
		}
	}
	return 0;

}
//创建初始化
void CPeZhu::init()
{
	//1.将文件读取到内存  DLL.dll  test.exe
	hFile = CreateFile(L"DLL.dll", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("文件不存在");
	}
	//文件大小
	dwFileSize = GetFileSize(hFile, 0);
	//给他new    一个空间,用来存放读取的内容
	pBuf = new BYTE[dwFileSize];
	ReadFile(hFile, pBuf, dwFileSize, &dwRead, NULL);

 //	byte my[33] = "6d6663313430752e646c6c00c9004372";//特征码
	//byte bidui[33];
	//int a = 0;
 //	//特征码比对
 //	 for (byte i=0;i<dwFileSize;i+=33)
 //	 {
 //		 for (int i=0;i<33;i++)
	//	 {
	//		 
 //			 //取33个字符
 //			 bidui[i] = pBuf[a+i];
	//		 a++;

 //		 }
	//	 for (int i=0;i<33;++i)
	//	 {
	//		 if (bidui[i] == pBuf[a + i])
	//		 {
	//			 MessageBoxW(L"查找到");
	//			 break;
	//		 }
	//		
	//		 
	//	 }
 //		 ////一次比较的大小
 //		 //if ( *my==*bidui) //地址相等
 //		 //{
 //			// MessageBoxW(L"查找到");
 //			// break;
 //		 //}
 //		 pBuf[dwFileSize-33]= pBuf[dwFileSize-33]- bidui[33];
	//	 dwFileSize=dwFileSize - 33;

 //	 }



	//将缓冲区当成DOS头来解析
	pDosHdr = (IMAGE_DOS_HEADER*)pBuf;
	pNtHdr = (IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pBuf); //找到NT头的位置

																   //判断是否是一个有效的PE文件
	if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE || pNtHdr->Signature != IMAGE_NT_SIGNATURE)
	{

		MessageBoxW(L"不是有效的PE文件");


	}
	pFileHdr = &pNtHdr->FileHeader;//文件头的地址
								   //INT A= pFileHdr->NU;
	poptHdr = &pNtHdr->OptionalHeader;//扩展头
									  //判断PE文件是32位还是64位的
	if (poptHdr->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		MessageBoxW(L"32位的PE文件");

	}
	else if (poptHdr->Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		MessageBoxW(L"64位的PE文件");
	}
	dwNtHeaderSize = sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + pFileHdr->SizeOfOptionalHeader;
	pScnHdr = (IMAGE_SECTION_HEADER*)((DWORD)pNtHdr + dwNtHeaderSize);//???
	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);
	//第二个区段
	//加需要加结构体的大小
	dwNtHeaderSizea = sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + pFileHdr->SizeOfOptionalHeader + sizeof(IMAGE_SECTION_HEADER);
	pScnHdB = (IMAGE_SECTION_HEADER*)((DWORD)pNtHdr + dwNtHeaderSizea);

	pDataDirectory = poptHdr->DataDirectory;//资源地址
											// 找到导出表
	DWORD dwExpRva = pDataDirectory[0].VirtualAddress;
	//得到RVA的文件偏移
	DWORD dwExpOfs = RVAToOffset(pDosHdr, dwExpRva);

	//解析地址
	pExpTab = (IMAGE_EXPORT_DIRECTORY*)(dwExpOfs + (DWORD)pDosHdr);
	//解析DLL的名
	dwNameOfs = RVAToOffset(pDosHdr, pExpTab->Name);

	//得到函数名
	char* pDllName = (char*)(dwNameOfs + (DWORD)pDosHdr);


}


//DOS头
void CPeZhu::OnBnClickedButton1()
{
	//1.传入头文件
	CPEDosDlg dlgPE(pDosHdr);
	dlgPE.DoModal();
}


BOOL CPeZhu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	init();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//NT头
void CPeZhu::OnBnClickedButton2()
{
	CFileDlg dlgPE(pFileHdr);
	dlgPE.DoModal();
	
}

//可选头
void CPeZhu::OnBnClickedButton3()
{
	// 扩展头信息
	COptlonalDlg COptDlg(poptHdr);															//加载导入表,导出表,资源表...
	COptDlg.DoModal();
	
}

//资源表
void CPeZhu::OnBnClickedButton4()
{
	CString stda;
	// resources代码
	CResouDlg resoudlg(pDosHdr, hFile);
	resoudlg.DoModal();
	// TODO: 在此添加控件通知处理程序代码
}

//区段表
void CPeZhu::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//区段头信息
	CString STDA;
	CMuluDlg resoudlg;
	INT nA = pFileHdr->NumberOfSections;// 区段表的大小
	for (int i = 0; i < nA; ++i)
	{
		STDA = CString(pScnHdr->Name);
		STDA += "\n";
		resoudlg.m_statica += STDA;
		pScnHdr++;//加结构体大小

	}
	resoudlg.DoModal();
}

//导入表
void CPeZhu::OnBnClickedButton7()
{
	Cdaoru daoru(pDosHdr);
	daoru.DoModal();

}

//到出表
void CPeZhu::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CDerivedTabDlg dlg(pDosHdr);
	dlg.DoModal();
}


void CPeZhu::OnBnClickedButton8()
{
	CongDingWeiDlg DLG(pDosHdr, pBuf, pFileHdr, pScnHdr);
	DLG.DoModal();
}
