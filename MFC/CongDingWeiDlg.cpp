// CongDingWeiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "CongDingWeiDlg.h"
#include "afxdialogex.h"


// CongDingWeiDlg 对话框

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

// CongDingWeiDlg 消息处理程序
DWORD CongDingWeiDlg::RVAToOffset(LPVOID pDos, DWORD dwRva) {
	// 找到需要转换的RVA所在的区段,然后计算偏移.
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)pDos;

	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)((DWORD)pDos + pDosHdr->e_lfanew);

	IMAGE_SECTION_HEADER* pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);

	DWORD dwScnCount = pNtHdr->FileHeader.NumberOfSections;

	for (DWORD i = 0; i < dwScnCount; i++) {
		// 判断RVA是否在一个区段内
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
	wchar_t* fu[] = { L"从定位的个数",L"区段",L"虚拟地址",L"偏移" };
	for (int i = 0; i<4; i++)
	{
		m_tou.InsertColumn(i, fu[i], LVCFMT_LEFT, 120);
	}
	m_wei.InsertColumn(0, { L"数据中存放的内容" }, LVCFMT_LEFT, 320);
	IMAGE_NT_HEADERS *pNtHdr =(IMAGE_NT_HEADERS*)((DWORD)pDosHdr + pDosHdr->e_lfanew);
	// 得到扩展头
	IMAGE_OPTIONAL_HEADER* pOptHdr = &pNtHdr->OptionalHeader;
	// 得到数据目录表
	IMAGE_DATA_DIRECTORY* pDataDir = pOptHdr->DataDirectory;
	// 获取重定位表
	/*
	typedef struct _IMAGE_BASE_RELOCATION {
	DWORD   VirtualAddress; // 块的起始RVA
	DWORD   SizeOfBlock;    // 整个块的大小(包括VirtualAddress和SizeOfBlock两个字段的大小)
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
		m_tou.InsertItem(nCount, sta_a); //从定位个数
		printf("%08X\n", pBaseRel->VirtualAddress);
		sta_c.Format(L"%08X", pBaseRel->VirtualAddress);//地址
		m_tou.SetItemText(nCount, 2, sta_c);
		////判断区段
		INT nA = pFileHdr->NumberOfSections;// 区段表的大小
		STDA.Format(L"%x", pScnHdr->Name);
		pScnHdr++;//加结构体大小
		if (pBaseRel->VirtualAddress >= pFileHdr->NumberOfSections)//待修改
		{
			STDA = CString(pScnHdr->Name);
			m_tou.SetItemText(nCount, 1, STDA);
		}
		sta_b.Format(L"%08X", pBaseRel->VirtualAddress);//地址	
		aa = "";
		//TypeOffset 里面的数据(自己定义的结构体)
		TypeOffset* pTypeOffset = (TypeOffset*)(pBaseRel + 1);
		for (DWORD i = 0; i < dwCount; ++i)
		{
			if (pTypeOffset[i].Type == IMAGE_REL_BASED_HIGHLOW) {
				// 得到需要修复的RVA
				DWORD dwRva = pBaseRel->VirtualAddress + pTypeOffset[i].Offset; 12;  // (1) 大小为12Bit的重定位偏移
				DWORD dwOffset = RVAToOffset(pFileBuff, dwRva);
				DWORD* pAddress = (DWORD*)(dwOffset + (DWORD)pFileBuff);
				sta_d.Format(L"%06X", pTypeOffset[i].Offset);//地址		
				aa += ",";
				aa += +sta_d;	
			}
			
		}
		m_wei.InsertItem(0, aa);
		// 找到下一个重定位块
		pBaseRel =(IMAGE_BASE_RELOCATION*)((DWORD)pBaseRel + pBaseRel->SizeOfBlock);
		count++;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



// CongDingWeiDlg 消息处理程序
