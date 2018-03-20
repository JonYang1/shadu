// Cdaoru.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "Cdaoru.h"
#include "afxdialogex.h"


// Cdaoru 对话框

IMPLEMENT_DYNAMIC(Cdaoru, CDialogEx)

Cdaoru::Cdaoru(IMAGE_DOS_HEADER* pDosHdr, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_daoru2, pParent)
{
	this->pDosHdr = pDosHdr;
}

Cdaoru::~Cdaoru()
{
}
// Cdaoru 消息处理程序
/*!
* \brief : RVAToOffset
* \return: DWORD	rva对应的文件偏移,如果转换失败,返回-1
* \param : IMAGE_DOS_HEADER * pDos rva所在的pe文件
* \param : DWORD dwRva 待转换的RVA
*/
DWORD Cdaoru::RVAToOffset(IMAGE_DOS_HEADER* pDos,
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
void Cdaoru::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_daoru);
	DDX_Control(pDX, IDC_LIST2, m_mokuaim);
}


BEGIN_MESSAGE_MAP(Cdaoru, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &Cdaoru::OnNMClickList1)
END_MESSAGE_MAP()


// Cdaoru 消息处理程序


BOOL Cdaoru::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString sta_aa, sta_bb, sta_cc;
	CString stb_aa, stb_bb, stb_cc;
	// TODO:  在此添加额外的初始化
	wchar_t* zifu[] = { L"模块名",L"函数序号",L"地址:",L"导入的DLL" };
	for (int i = 0; i < 4; ++i)
	{
		m_daoru.InsertColumn(i, zifu[i], 0, 100);
	}
	wchar_t* zifua[] = { L"函数序号",L"函数名称",L"地址:",L"导入的DLL" };
	for (int i = 0; i < 4; ++i)
	{
		m_mokuaim.InsertColumn(i, zifua[i], 0, 100);
	}
	int nCount = m_daoru.GetItemCount();//获取对象的编号  
	int nCount2 = m_mokuaim.GetItemCount();
	IMAGE_NT_HEADERS* pNtHdr = NULL;
	pNtHdr = (IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pDosHdr);
	// 3. 找到扩展头
	IMAGE_OPTIONAL_HEADER* pOptHdr = NULL;
	pOptHdr = &pNtHdr->OptionalHeader;

	// 4. 找到数据目录表
	IMAGE_DATA_DIRECTORY* pDataDir = NULL;
	pDataDir = pOptHdr->DataDirectory;
	// 5. 得到导入表的RVA
	DWORD dwImpRva = pDataDir[1].VirtualAddress;
	IMAGE_IMPORT_DESCRIPTOR* pImpArray;
	pImpArray = (IMAGE_IMPORT_DESCRIPTOR*)(RVAToOffset(pDosHdr, dwImpRva) + (DWORD)pDosHdr);
	while (pImpArray->Name != 0)
	{
		// Dll的名称
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
				m_daoru.SetItemText(nCount, 1, sta_aa);//插入行
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
				  // 异常: OCX 属性页应返回 FALSE
}

//鼠标单击获取的值
void Cdaoru::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
