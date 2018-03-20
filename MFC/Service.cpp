// Service.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "Service.h"
#include "afxdialogex.h"
#include <winsvc.h>


// CService 对话框

IMPLEMENT_DYNAMIC(CService, CDialogEx)

CService::CService(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVICE, pParent)
{

}

CService::~CService()
{
}
//获取系统服务
void CService::servicea()
{
	//获取服务控制管理器句柄 获取所有的
	SC_HANDLE fanhui=	OpenSCManagerW(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	//1.第一次调用,获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(fanhui,SC_ENUM_PROCESS_INFO,SERVICE_WIN32,SERVICE_STATE_ALL,NULL,0,
						&dwSize,&dwServiceNum,NULL,NULL);

	//2.申请需要的内存,第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerice = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	//3.第二次枚举
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(fanhui,SC_ENUM_PROCESS_INFO,SERVICE_WIN32,SERVICE_STATE_ALL
								,(PBYTE)pEnumSerice,dwSize,&dwSize,&dwServiceNum,NULL,NULL);

	int a = 0;
	CString sta;

	//3.遍历进程

	for (DWORD i=0;i<dwServiceNum;i++)
	{
		//获取基础信息
		//服务名
		pEnumSerice[i].lpServiceName;
		sta.Format(_T("%s"), pEnumSerice[i].lpServiceName);
		m_service.InsertItem(a, sta);

		
		sta.Format(_T("%s"), pEnumSerice[i].lpDisplayName);
		m_service.SetItemText(a,2,sta);
		//服务状态
		pEnumSerice[i].ServiceStatusProcess.dwCurrentState;  //坑
		sta.Format(_T("%d"), pEnumSerice[i].ServiceStatusProcess.dwProcessId);
		m_service.SetItemText(a,1, sta);


		//运行状态
		switch (pEnumSerice[i].ServiceStatusProcess.dwCurrentState)
		{
		case SERVICE_CONTINUE_PENDING:
		case SERVICE_PAUSE_PENDING:
		case SERVICE_PAUSED:
			sta = _T("暂停或挂起");
			m_service.SetItemText(a, 3, sta);
			break;
		case SERVICE_RUNNING:
		case SERVICE_START_PENDING:
			sta = _T("正在运行");
			m_service.SetItemText(a, 3, sta);
			break;
		case SERVICE_STOP_PENDING:
			
			sta = _T("停止运行");
			m_service.SetItemText(a, 3, sta);
			break;
		case SERVICE_STOPPED:
		
			sta = _T("非运行态");
			m_service.SetItemText(a, 3, sta);
			break;
		default:
			break;
		}

	}
}


void CService::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_service);
}


BEGIN_MESSAGE_MAP(CService, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CService::OnNMRClickList1)
END_MESSAGE_MAP()


// CService 消息处理程序


BOOL CService::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_service.InsertColumn(0, L"名称", LVCFMT_LEFT, 140);
	m_service.InsertColumn(1, L"ID", LVCFMT_LEFT, 40);
	m_service.InsertColumn(2, L"详细信息", LVCFMT_LEFT, 240);
	m_service.InsertColumn(3, L"状态", LVCFMT_LEFT, 100);
	servicea();//加载服务
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CService::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	SERVICE_STATUS  ssStatus;
	SC_HANDLE hSCM = OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	
	*pResult = 0;

	// 获取右键弹出的位置
	CPoint	pt;
	GetCursorPos(&pt);
	// 右键弹出窗口
	CMenu mu;
	mu.LoadMenu(IDR_MENU2);
	CMenu * pSub = mu.GetSubMenu(0);
	// 获取用户点击项
	UINT unMenuId = 0;
	unMenuId = TrackPopupMenu(pSub->m_hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,
		pt.x, pt.y, 0, m_hWnd, 0);
	switch (unMenuId) {
	case ID_32774:// 开启服务
	{
		// 开启服务
		CString str;
		for (int i = 0; i < m_service.GetItemCount(); i++)
		{
			if (m_service.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			{
				str.Format(_T("选中了第%d行"), i);
				//	AfxMessageBox(str);
				CString my;
				//获取名称
				my = (CString)m_service.GetItemText(i, 0);
				DWORD dwBytesNeeded;
				//获取服务控制管理器句柄 获取所有的
				if (hSCM == NULL)
				{
					MessageBox(L"权限不足，无法访问");
					CloseServiceHandle(hSCM);
					return;
				}
				SC_HANDLE hService = OpenService(hSCM, my, SERVICE_ALL_ACCESS);

				if (QueryServiceStatusEx(hService,                     // handle to service 
					SC_STATUS_PROCESS_INFO,         // information level
					(LPBYTE)&ssStatus,             // address of structure
					sizeof(SERVICE_STATUS_PROCESS), // size of structure
					&dwBytesNeeded))              // size needed if buffer is too small
				{
					if (ssStatus.dwCurrentState == SERVICE_RUNNING)
					{
						MessageBox(L"服务已经在运行中");
					}
					else if (ssStatus.dwCurrentState == SERVICE_STOPPED)
					{
						StartService(hService, 0, NULL);
						CloseServiceHandle(hService);
						CloseServiceHandle(hSCM);
						MessageBox(L"服务已启动");
						return;
					}
				}
				CloseServiceHandle(hService);
				CloseServiceHandle(hSCM);
				

			}
		}
		break;
	}
	case ID_32775:// 关闭服务
	{
		CString str;
		for (int i = 0; i < m_service.GetItemCount(); i++)
		{
			if (m_service.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			{
				str.Format(_T("选中了第%d行"), i);
				CString my;
				//获取名称
				my = (CString)m_service.GetItemText(i, 0);
				//关闭服务句柄
				SC_HANDLE stService = OpenService(hSCM, my, SERVICE_STOP | GENERIC_EXECUTE);
				ControlService(stService,SERVICE_CONTROL_STOP, &ssStatus);
				CloseServiceHandle(stService); 
				CloseServiceHandle(hSCM);

			}
		}
		break;
	}
	
	default:
		break;
	}
}
