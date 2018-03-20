// MyJingc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "MyJingc.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "DlgThread.h"
#include "DlgModule.h"


// CMyJingc �Ի���

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
// ö�ٽ���
BOOL CMyJingc::EnumProcess()
{
	// ���
	m_vecProcess.clear();
	// ����
	HANDLE hProcessSnap;//���̿��վ��
	HANDLE hProcess;//���̾��
	PROCESSENTRY32 stcPe32 = { 0 };//���̿�����Ϣ
	stcPe32.dwSize = sizeof(PROCESSENTRY32);
	//1.����һ��������صĿ��վ��
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) return FALSE;
	//2ͨ�����̿��վ����ȡ��һ��������Ϣ
	if (!Process32First(hProcessSnap, &stcPe32)) {
		CloseHandle(hProcessSnap);
		return FALSE;
	}
	//3ѭ������������Ϣ
	do {
		MY_PROCESSINFO stcInfo = { 0 };
		//3.1��ȡ������
		wcscpy_s(stcInfo.szProcess, MAX_PATH, stcPe32.szExeFile);
		//3.2��ȡ����·�������ȼ���Ϣ
		//hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, NULL, stcPe32.th32ProcessID);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, NULL, stcPe32.th32ProcessID);
		if (hProcess) {
			// ��ȡ��������·��
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hProcess, 0, stcInfo.szFullProcess, &dwSize);
			//GetProcessImageFileName(hProcess, stcInfo.szFullProcess, MAX_PATH);
			stcInfo.dwPriorityClass = GetPriorityClass(hProcess);//��ȡ�������ȼ�
			CloseHandle(hProcess);
		}
		//3.3��ȡ���̵����������Ϣ
		stcInfo.dwPid = stcPe32.th32ProcessID;
		stcInfo.dwThreadCount = stcPe32.cntThreads;
		stcInfo.dwParentProcess = stcPe32.th32ParentProcessID;
		//3.4����ȡ���������Ϣ���浽������
		m_vecProcess.push_back(stcInfo);
	} while (Process32Next(hProcessSnap, &stcPe32));
	CloseHandle(hProcessSnap);
	return TRUE;
}

// ����list����Ϣ
BOOL CMyJingc::InsertProcessList()
{
	m_ctlListProcess.DeleteAllItems();
	int i = 0;//������
	CString temp;
	// L"��������",L"PID",L"������PID",L"�߳�����",L"���ȼ�",L"��������·��"
	for (MY_PROCESSINFO &stc : m_vecProcess) {
		// ��1��,��������
		m_ctlListProcess.InsertItem(i, stc.szProcess);
		// ��2��,PID
		temp.Format(L"%d", stc.dwPid);
		m_ctlListProcess.SetItemText(i, 1, temp);
		// ��3��,������PID
		temp.Format(L"%d", stc.dwParentProcess);
		m_ctlListProcess.SetItemText(i, 2, temp);
		// ��4��,�߳�����
		temp.Format(L"%d", stc.dwThreadCount);
		m_ctlListProcess.SetItemText(i, 3, temp);
		// ��5��,���ȼ�
		temp.Format(L"%d", stc.dwPriorityClass);
		m_ctlListProcess.SetItemText(i, 4, temp);
		// ��6��,��������·��
		m_ctlListProcess.SetItemText(i, 5, stc.szFullProcess);
		++i;
	}
	return TRUE;
}
//����ԱȨ��
bool CMyJingc::IsRunAsAdmin(BOOL &bFullToken)
{
	// 1. ��ñ����̵�����
	HANDLE hToken = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return false;
	// 2. ��ȡ��������
	TOKEN_ELEVATION_TYPE ElevationType = TokenElevationTypeDefault;
	BOOL			   bIsAdmin = false;
	DWORD                dwSize = 0;
	if (GetTokenInformation(hToken, TokenElevationType, &ElevationType,
		sizeof(TOKEN_ELEVATION_TYPE), &dwSize)) {
		// 2.1 ��������Ա��Ķ�ӦSID
		BYTE adminSID[SECURITY_MAX_SID_SIZE];
		dwSize = sizeof(adminSID);
		CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);
		// 2.2 �жϵ�ǰ���������û���ɫ�Ƿ�Ϊ����Ա
		if (ElevationType == TokenElevationTypeLimited) {
			// a. ��ȡ�������Ƶľ��
			HANDLE hUnfilteredToken = NULL;
			GetTokenInformation(hToken, TokenLinkedToken, (PVOID)&hUnfilteredToken,
				sizeof(HANDLE), &dwSize);
			// b. ������ԭʼ�������Ƿ��������Ա��SID
			if (!CheckTokenMembership(hUnfilteredToken, &adminSID, &bIsAdmin))
				return false;
			CloseHandle(hUnfilteredToken);
		}
		else {
			bIsAdmin = IsUserAnAdmin();
		}
		CloseHandle(hToken);
	}
	// 3. �жϾ����Ȩ��״��
	bFullToken = false;
	switch (ElevationType) {
	case TokenElevationTypeDefault: /* Ĭ�ϵ��û���UAC������ */
		if (bIsAdmin)		  bFullToken = true; // Ĭ���û��й���ԱȨ��
		else                  bFullToken = false;// Ĭ���û����ǹ���Ա��
		break;
	case TokenElevationTypeFull:    /* �Ѿ��ɹ���߽���Ȩ�� */
		if (bIsAdmin)		  bFullToken = true; //��ǰ�Թ���ԱȨ������
		else                  bFullToken = false;//��ǰδ�Թ���ԱȨ������
		break;
	case TokenElevationTypeLimited: /* �����������޵�Ȩ������ */
		if (bIsAdmin)  bFullToken = false;//�û��й���ԱȨ�ޣ�������Ȩ������
		else           bFullToken = false;//�û����ǹ���Ա�飬�ҽ���Ȩ������
	}
	return true;
}

//�򿪽���
bool CMyJingc::EnableDebugPrivilege(BOOL bEnable /*= TRUE*/)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	// �򿪱���������
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

//������ť
void CMyJingc::OnBnClickedQuanXian()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bIsFull) {
		// 1. ���ص�ǰ����
		ShowWindow(SW_HIDE);
		// 2. ��ȡ��ǰ����·��
		WCHAR szApplication[MAX_PATH] = { 0 };
		DWORD cchLength = _countof(szApplication);
		QueryFullProcessImageName(GetCurrentProcess(), 0,
			szApplication, &cchLength);
		// 3. �Թ���ԱȨ�����´򿪽���
		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
		sei.lpVerb = L"runas";      // ��������Ȩ��
		sei.lpFile = szApplication; // ��ִ���ļ�·��
		sei.lpParameters = NULL;          // ����Ҫ����
		sei.nShow = SW_SHOWNORMAL; // ������ʾ����
		if (ShellExecuteEx(&sei))
			ExitProcess(0);//ExitProcess(1);
		else
			ShowWindow(SW_SHOWNORMAL);
	}
}

BOOL CMyJingc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// 00 ��Ȩ
	m_bIsFull = false;
	if (IsRunAsAdmin(m_bIsFull)) {
		if (!m_bIsFull) {
			// �ǹ���Ա�����UAC���
			Button_SetElevationRequiredState(::GetDlgItem(m_hWnd, IDC_QUAN_XIAN), TRUE);
		}
		else {
			// ����ԱȨ�޿���DebugȨ�ޣ���ΪĬ��DebugȨ���ǹرյ�
			EnableDebugPrivilege();
		}
	}
	// 01 ��ʼ���ؼ���Ϣ
	m_ctlListProcess.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[6] = { L"��������", L"PID", L"������PID", L"�߳�����", L"���ȼ�", L"��������·��" };
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
	// ����͸����
	SetLayeredWindowAttributes(0, 10, LWA_ALPHA);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//�Ҽ��¼�
void CMyJingc::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
	// ��ȡ�Ҽ�������λ��
	CPoint	pt;
	GetCursorPos(&pt);
	// �Ҽ���������
	CMenu mu;
	mu.LoadMenu(IDR_MENU1);
	CMenu * pSub = mu.GetSubMenu(0);
	// ��ȡ�û������
	UINT unMenuId = 0;
	unMenuId = TrackPopupMenu(pSub->m_hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,
		pt.x, pt.y, 0, m_hWnd, 0);
	switch (unMenuId) {
	case ID_32771:// ����ģ��
	{
		CString strPid = m_ctlListProcess.GetItemText(pNMItemActivate->iItem, 1);
		WCHAR *pEnd = L"";
		DWORD dwPid = wcstol(strPid.GetBuffer(), &pEnd, 10);
		// ��PID��ֵ����ģ�鴰�������
		CDlgModule dlg;
		dlg.m_dwPid = dwPid;
		dlg.DoModal();
		break;
	}
	case ID_32772:// �����߳�
	{
		// ��ȡҪ�����߳̽���PID
		CString strPid = m_ctlListProcess.GetItemText(pNMItemActivate->iItem, 1);
		WCHAR *pEnd = L"";
		DWORD dwPid = wcstol(strPid.GetBuffer(), &pEnd, 10);
		// ��PID��ֵ�����̴߳��������
		CDlgThread dlg;
		dlg.m_dwPid = dwPid;
		dlg.DoModal();
		break;
	}
	case ID_32773:// �����߳�
	{
		// ��������
		CString str;
		for (int i = 0; i < m_ctlListProcess.GetItemCount(); i++)
		{
			if (m_ctlListProcess.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			{
				str.Format(_T("ѡ���˵�%d��"), i);
			//	AfxMessageBox(str);
				CString my;
				//��ȡ����
				my =(CString)m_ctlListProcess.GetItemText(i, 1);
				int A = _ttoi(my);
				//��������
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
	// TODO: �ڴ���������������
	// ��ȡҪ����ģ��Ľ���PID
	
}

void CMyJingc::OnProc()
{
	// TODO: �ڴ���������������
	CString str;
	for (int i = 0; i < m_ctlListProcess.GetItemCount(); i++)
	{
		if (m_ctlListProcess.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			str.Format(_T("ѡ���˵�%d��"), i);
			AfxMessageBox(str);
		}
	}
}

//����
HBRUSH CMyJingc::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_brBkgr;

	}
	if (pWnd->GetDlgCtrlID() == IDC_QUAN_XIAN)
	{
		pDC->SetTextColor(RGB(100, 0, 0));   //����������ɫ
		pDC->SetBkMode(HOLLOW_BRUSH); //�������屳��Ϊ͸��
									 // TODO: Return a different brush if the default is not desired
		return (HBRUSH)::GetStockObject(BLACK_BRUSH);   // ���ñ���ɫ
	}
	else
	{
		return hbr;
	}
	
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

}
//����
void CMyJingc::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LONG lStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);
	// ����͸����
	SetLayeredWindowAttributes(0, 120, LWA_ALPHA);
}
//����
void CMyJingc::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(&dc);
	CBitmap bmpBackground1;
	bmpBackground1.LoadBitmap(IDB_BITMAP1);
	BITMAP bitmap1;
	bmpBackground1.GetBitmap(&bitmap1);
	CBitmap* pbmpPri1 = dcMem1.SelectObject(&bmpBackground1);
	dc.BitBlt(0, -34, 1036, 539, &dcMem1, 0, 0, SRCCOPY);
}
