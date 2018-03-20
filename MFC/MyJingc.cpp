// MyJingc.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "MyJingc.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "DlgThread.h"
#include "DlgModule.h"


// CMyJingc 对话框

IMPLEMENT_DYNAMIC(CMyJingc, CDialogEx)

CMyJingc::CMyJingc(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_JINC, pParent)
{

}

CMyJingc::~CMyJingc()
{
}

void CMyJingc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlListProcess);
}

BEGIN_MESSAGE_MAP(CMyJingc, CDialogEx)
	ON_BN_CLICKED(IDC_QUAN_XIAN, &CMyJingc::OnBnClickedQuanXian)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMyJingc::OnNMRClickList1)
	ON_COMMAND(ID_32771, &CMyJingc::OnMokuai)
	
	ON_COMMAND(ID_32773, &CMyJingc::OnProc)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyJingc::OnBnClickedButton1)
	ON_WM_PAINT()
END_MESSAGE_MAP()
// 枚举进程
BOOL CMyJingc::EnumProcess()
{
	// 清空
	m_vecProcess.clear();
	// 遍历
	HANDLE hProcessSnap;//进程快照句柄
	HANDLE hProcess;//进程句柄
	PROCESSENTRY32 stcPe32 = { 0 };//进程快照信息
	stcPe32.dwSize = sizeof(PROCESSENTRY32);
	//1.创建一个进程相关的快照句柄
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) return FALSE;
	//2通过进程快照句柄获取第一个进程信息
	if (!Process32First(hProcessSnap, &stcPe32)) {
		CloseHandle(hProcessSnap);
		return FALSE;
	}
	//3循环遍历进程信息
	do {
		MY_PROCESSINFO stcInfo = { 0 };
		//3.1获取进程名
		wcscpy_s(stcInfo.szProcess, MAX_PATH, stcPe32.szExeFile);
		//3.2获取完整路径及优先级信息
		//hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, NULL, stcPe32.th32ProcessID);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, NULL, stcPe32.th32ProcessID);
		if (hProcess) {
			// 获取进程完整路径
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hProcess, 0, stcInfo.szFullProcess, &dwSize);
			//GetProcessImageFileName(hProcess, stcInfo.szFullProcess, MAX_PATH);
			stcInfo.dwPriorityClass = GetPriorityClass(hProcess);//获取进程优先级
			CloseHandle(hProcess);
		}
		//3.3获取进程的其他相关信息
		stcInfo.dwPid = stcPe32.th32ProcessID;
		stcInfo.dwThreadCount = stcPe32.cntThreads;
		stcInfo.dwParentProcess = stcPe32.th32ParentProcessID;
		//3.4将获取到的相关信息保存到向量中
		m_vecProcess.push_back(stcInfo);
	} while (Process32Next(hProcessSnap, &stcPe32));
	CloseHandle(hProcessSnap);
	return TRUE;
}

// 插入list表信息
BOOL CMyJingc::InsertProcessList()
{
	m_ctlListProcess.DeleteAllItems();
	int i = 0;//索引用
	CString temp;
	// L"进程名称",L"PID",L"父进程PID",L"线程数量",L"优先级",L"进程所在路径"
	for (MY_PROCESSINFO &stc : m_vecProcess) {
		// 第1列,进程名称
		m_ctlListProcess.InsertItem(i, stc.szProcess);
		// 第2列,PID
		temp.Format(L"%d", stc.dwPid);
		m_ctlListProcess.SetItemText(i, 1, temp);
		// 第3列,父进程PID
		temp.Format(L"%d", stc.dwParentProcess);
		m_ctlListProcess.SetItemText(i, 2, temp);
		// 第4列,线程数量
		temp.Format(L"%d", stc.dwThreadCount);
		m_ctlListProcess.SetItemText(i, 3, temp);
		// 第5列,优先级
		temp.Format(L"%d", stc.dwPriorityClass);
		m_ctlListProcess.SetItemText(i, 4, temp);
		// 第6列,进程所在路径
		m_ctlListProcess.SetItemText(i, 5, stc.szFullProcess);
		++i;
	}
	return TRUE;
}
//管理员权限
bool CMyJingc::IsRunAsAdmin(BOOL &bFullToken)
{
	// 1. 获得本进程的令牌
	HANDLE hToken = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return false;
	// 2. 获取提升类型
	TOKEN_ELEVATION_TYPE ElevationType = TokenElevationTypeDefault;
	BOOL			   bIsAdmin = false;
	DWORD                dwSize = 0;
	if (GetTokenInformation(hToken, TokenElevationType, &ElevationType,
		sizeof(TOKEN_ELEVATION_TYPE), &dwSize)) {
		// 2.1 创建管理员组的对应SID
		BYTE adminSID[SECURITY_MAX_SID_SIZE];
		dwSize = sizeof(adminSID);
		CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);
		// 2.2 判断当前进程运行用户角色是否为管理员
		if (ElevationType == TokenElevationTypeLimited) {
			// a. 获取连接令牌的句柄
			HANDLE hUnfilteredToken = NULL;
			GetTokenInformation(hToken, TokenLinkedToken, (PVOID)&hUnfilteredToken,
				sizeof(HANDLE), &dwSize);
			// b. 检查这个原始的令牌是否包含管理员的SID
			if (!CheckTokenMembership(hUnfilteredToken, &adminSID, &bIsAdmin))
				return false;
			CloseHandle(hUnfilteredToken);
		}
		else {
			bIsAdmin = IsUserAnAdmin();
		}
		CloseHandle(hToken);
	}
	// 3. 判断具体的权限状况
	bFullToken = false;
	switch (ElevationType) {
	case TokenElevationTypeDefault: /* 默认的用户或UAC被禁用 */
		if (bIsAdmin)		  bFullToken = true; // 默认用户有管理员权限
		else                  bFullToken = false;// 默认用户不是管理员组
		break;
	case TokenElevationTypeFull:    /* 已经成功提高进程权限 */
		if (bIsAdmin)		  bFullToken = true; //当前以管理员权限运行
		else                  bFullToken = false;//当前未以管理员权限运行
		break;
	case TokenElevationTypeLimited: /* 进程在以有限的权限运行 */
		if (bIsAdmin)  bFullToken = false;//用户有管理员权限，但进程权限有限
		else           bFullToken = false;//用户不是管理员组，且进程权限有限
	}
	return true;
}

//打开进程
bool CMyJingc::EnableDebugPrivilege(BOOL bEnable /*= TRUE*/)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	// 打开本进程令牌
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		if (!LookupPrivilegeValue(
			NULL,            // lookup privilege on local system
			SE_DEBUG_NAME,   // privilege to lookup 
			&luid))        // receives LUID of privilege
		{
			printf("LookupPrivilegeValue error: %u\n", GetLastError());
			CloseHandle(hToken);
			return FALSE;
		}

		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		// Enable the privilege or disable all privileges.
		if (!AdjustTokenPrivileges(
			hToken,
			FALSE,
			&tp,
			sizeof(TOKEN_PRIVILEGES),
			(PTOKEN_PRIVILEGES)NULL,
			(PDWORD)NULL)) {
			printf("AdjustTokenPrivileges error: %u\n", GetLastError());
			CloseHandle(hToken);
			return FALSE;
		}

		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
			printf("The token does not have the specified privilege. \n");
			CloseHandle(hToken);
			return FALSE;
		}
		CloseHandle(hToken);
		return TRUE;
	}
	return FALSE;
}

//提升按钮
void CMyJingc::OnBnClickedQuanXian()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bIsFull) {
		// 1. 隐藏当前窗口
		ShowWindow(SW_HIDE);
		// 2. 获取当前程序路径
		WCHAR szApplication[MAX_PATH] = { 0 };
		DWORD cchLength = _countof(szApplication);
		QueryFullProcessImageName(GetCurrentProcess(), 0,
			szApplication, &cchLength);
		// 3. 以管理员权限重新打开进程
		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
		sei.lpVerb = L"runas";      // 请求提升权限
		sei.lpFile = szApplication; // 可执行文件路径
		sei.lpParameters = NULL;          // 不需要参数
		sei.nShow = SW_SHOWNORMAL; // 正常显示窗口
		if (ShellExecuteEx(&sei))
			ExitProcess(0);//ExitProcess(1);
		else
			ShowWindow(SW_SHOWNORMAL);
	}
}

BOOL CMyJingc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 00 提权
	m_bIsFull = false;
	if (IsRunAsAdmin(m_bIsFull)) {
		if (!m_bIsFull) {
			// 非管理员，添加UAC标记
			Button_SetElevationRequiredState(::GetDlgItem(m_hWnd, IDC_QUAN_XIAN), TRUE);
		}
		else {
			// 管理员权限开启Debug权限，因为默认Debug权限是关闭的
			EnableDebugPrivilege();
		}
	}
	// 01 初始化控件信息
	m_ctlListProcess.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[6] = { L"进程名称", L"PID", L"父进程PID", L"线程数量", L"优先级", L"进程所在路径" };
	for (int i = 0; i < 6; ++i)
	{
		m_ctlListProcess.InsertColumn(i, szCol[i], 0, 100);
	}
	EnumProcess();
	InsertProcessList();
	m_ctlListProcess.SetBkColor(CLR_NONE);

	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP1);

	m_brBkgr.CreatePatternBrush(&bmp);
	bmp.DeleteObject();





	LONG lStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);
	// 设置透明度
	SetLayeredWindowAttributes(0, 10, LWA_ALPHA);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//右键事件
void CMyJingc::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
	// 获取右键弹出的位置
	CPoint	pt;
	GetCursorPos(&pt);
	// 右键弹出窗口
	CMenu mu;
	mu.LoadMenu(IDR_MENU1);
	CMenu * pSub = mu.GetSubMenu(0);
	// 获取用户点击项
	UINT unMenuId = 0;
	unMenuId = TrackPopupMenu(pSub->m_hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,
		pt.x, pt.y, 0, m_hWnd, 0);
	switch (unMenuId) {
	case ID_32771:// 遍历模块
	{
		CString strPid = m_ctlListProcess.GetItemText(pNMItemActivate->iItem, 1);
		WCHAR *pEnd = L"";
		DWORD dwPid = wcstol(strPid.GetBuffer(), &pEnd, 10);
		// 把PID的值传给模块窗口类对象
		CDlgModule dlg;
		dlg.m_dwPid = dwPid;
		dlg.DoModal();
		break;
	}
	case ID_32772:// 遍历线程
	{
		// 获取要遍历线程进程PID
		CString strPid = m_ctlListProcess.GetItemText(pNMItemActivate->iItem, 1);
		WCHAR *pEnd = L"";
		DWORD dwPid = wcstol(strPid.GetBuffer(), &pEnd, 10);
		// 把PID的值传给线程窗口类对象
		CDlgThread dlg;
		dlg.m_dwPid = dwPid;
		dlg.DoModal();
		break;
	}
	case ID_32773:// 遍历线程
	{
		// 结束进程
		CString str;
		for (int i = 0; i < m_ctlListProcess.GetItemCount(); i++)
		{
			if (m_ctlListProcess.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			{
				str.Format(_T("选中了第%d行"), i);
			//	AfxMessageBox(str);
				CString my;
				//获取名称
				my =(CString)m_ctlListProcess.GetItemText(i, 1);
				int A = _ttoi(my);
				//结束进程
				HANDLE hprocess = OpenProcess(PROCESS_TERMINATE,FALSE,A);
				TerminateProcess(hprocess,1);
				CloseHandle(hprocess);


			}
		}

		

		break;
	}
	default:
		break;
	}
}

void CMyJingc::OnMokuai()
{
	// TODO: 在此添加命令处理程序代码
	// 获取要遍历模块的进程PID
	
}

void CMyJingc::OnProc()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	for (int i = 0; i < m_ctlListProcess.GetItemCount(); i++)
	{
		if (m_ctlListProcess.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			str.Format(_T("选中了第%d行"), i);
			AfxMessageBox(str);
		}
	}
}

//背景
HBRUSH CMyJingc::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_brBkgr;

	}
	if (pWnd->GetDlgCtrlID() == IDC_QUAN_XIAN)
	{
		pDC->SetTextColor(RGB(100, 0, 0));   //设置字体颜色
		pDC->SetBkMode(HOLLOW_BRUSH); //设置字体背景为透明
									 // TODO: Return a different brush if the default is not desired
		return (HBRUSH)::GetStockObject(BLACK_BRUSH);   // 设置背景色
	}
	else
	{
		return hbr;
	}
	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

}
//背景
void CMyJingc::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	LONG lStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);
	// 设置透明度
	SetLayeredWindowAttributes(0, 120, LWA_ALPHA);
}
//背景
void CMyJingc::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(&dc);
	CBitmap bmpBackground1;
	bmpBackground1.LoadBitmap(IDB_BITMAP1);
	BITMAP bitmap1;
	bmpBackground1.GetBitmap(&bitmap1);
	CBitmap* pbmpPri1 = dcMem1.SelectObject(&bmpBackground1);
	dc.BitBlt(0, -34, 1036, 539, &dcMem1, 0, 0, SRCCOPY);
}
