// DlgModule.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "DlgModule.h"
#include "afxdialogex.h"
#include <Psapi.h>


// CDlgModule 对话框

IMPLEMENT_DYNAMIC(CDlgModule, CDialogEx)

CDlgModule::CDlgModule(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_JINC_DLH_2, pParent)
{

}

CDlgModule::~CDlgModule()
{
}

// 遍历模块信息
BOOL CDlgModule::EnumModule(DWORD dwPid)
{
	m_vecModule.clear();
	//1.1打开进程
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
	if (NULL == hProcess) {
		return FALSE;
	}
	
	HMODULE hModules[0x1] = { 0 };//模块数组
	DWORD cbNeed = 0;//实际获取的大小
	EnumProcessModulesEx(hProcess,hModules,sizeof(hModules),&cbNeed,LIST_MODULES_ALL);
						 
	DWORD dwModCount = cbNeed / sizeof(HMODULE);//模块数量
	HMODULE* pModBuf = new HMODULE[dwModCount];//保存模块句柄的缓存
	EnumProcessModulesEx(hProcess,pModBuf,dwModCount * sizeof(HMODULE),&cbNeed,LIST_MODULES_ALL);
	//2.枚举进程模块信息					
	MODULEENTRY32 stcMod32 = { sizeof(MODULEENTRY32) };
	MODULEINFO stcModInfo;
	//3.分配vec空间
	for (UINT i = 0; i < dwModCount; i++) {
		GetModuleBaseName(hProcess, pModBuf[i], stcMod32.szModule, MAX_PATH);
		GetModuleFileNameEx(hProcess, pModBuf[i], stcMod32.szExePath, MAX_PATH);
		GetModuleInformation(hProcess, pModBuf[i], &stcModInfo, sizeof(MODULEINFO));
		stcMod32.hModule = pModBuf[i];
		stcMod32.modBaseAddr = (PBYTE)stcModInfo.lpBaseOfDll;
		stcMod32.modBaseSize = stcModInfo.SizeOfImage;
		stcMod32.th32ProcessID = dwPid;
		m_vecModule.push_back(stcMod32);
	}
	CloseHandle(hProcess);
	delete[] pModBuf;
	return TRUE;
}

// 插入模块信息
BOOL CDlgModule::InsertModuleInfo()
{
	m_ctlListModule.DeleteAllItems();
	CString str;
	int i = 0;
	for (MODULEENTRY32 &module : m_vecModule) {

		m_ctlListModule.InsertItem(i, module.szModule);

		str.Format(L"%08X", module.modBaseAddr);
		m_ctlListModule.SetItemText(i, 1, str);
		str.Format(L"%d", module.modBaseSize);
		m_ctlListModule.SetItemText(i, 2, str);

		m_ctlListModule.SetItemText(i, 3, module.szExePath);
		i++;
	}
	return TRUE;
}



void CDlgModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlListModule);
}


BEGIN_MESSAGE_MAP(CDlgModule, CDialogEx)
END_MESSAGE_MAP()


// CDlgModule 消息处理程序
BOOL CDlgModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 初始化list表
	m_ctlListModule.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[4] = { L"模块名",L"基址",L"模块大小",L"路径" };
	for (int i = 0; i < 4; ++i)
	{
		m_ctlListModule.InsertColumn(i, szCol[i], 0, 80);
	}
	EnumModule(m_dwPid);
	InsertModuleInfo();
	return TRUE;  
				 
}
