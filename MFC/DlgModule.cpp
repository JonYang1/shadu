// DlgModule.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "DlgModule.h"
#include "afxdialogex.h"
#include <Psapi.h>


// CDlgModule �Ի���

IMPLEMENT_DYNAMIC(CDlgModule, CDialogEx)

CDlgModule::CDlgModule(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_JINC_DLH_2, pParent)
{

}

CDlgModule::~CDlgModule()
{
}

// ����ģ����Ϣ
BOOL CDlgModule::EnumModule(DWORD dwPid)
{
	m_vecModule.clear();
	//1.1�򿪽���
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
	if (NULL == hProcess) {
		return FALSE;
	}
	
	HMODULE hModules[0x1] = { 0 };//ģ������
	DWORD cbNeed = 0;//ʵ�ʻ�ȡ�Ĵ�С
	EnumProcessModulesEx(hProcess,hModules,sizeof(hModules),&cbNeed,LIST_MODULES_ALL);
						 
	DWORD dwModCount = cbNeed / sizeof(HMODULE);//ģ������
	HMODULE* pModBuf = new HMODULE[dwModCount];//����ģ�����Ļ���
	EnumProcessModulesEx(hProcess,pModBuf,dwModCount * sizeof(HMODULE),&cbNeed,LIST_MODULES_ALL);
	//2.ö�ٽ���ģ����Ϣ					
	MODULEENTRY32 stcMod32 = { sizeof(MODULEENTRY32) };
	MODULEINFO stcModInfo;
	//3.����vec�ռ�
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

// ����ģ����Ϣ
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


// CDlgModule ��Ϣ�������
BOOL CDlgModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ��ʼ��list��
	m_ctlListModule.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[4] = { L"ģ����",L"��ַ",L"ģ���С",L"·��" };
	for (int i = 0; i < 4; ++i)
	{
		m_ctlListModule.InsertColumn(i, szCol[i], 0, 80);
	}
	EnumModule(m_dwPid);
	InsertModuleInfo();
	return TRUE;  
				 
}
