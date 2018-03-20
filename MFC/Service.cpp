// Service.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "Service.h"
#include "afxdialogex.h"
#include <winsvc.h>


// CService �Ի���

IMPLEMENT_DYNAMIC(CService, CDialogEx)

CService::CService(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVICE, pParent)
{

}

CService::~CService()
{
}
//��ȡϵͳ����
void CService::servicea()
{
	//��ȡ������ƹ�������� ��ȡ���е�
	SC_HANDLE fanhui=	OpenSCManagerW(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	//1.��һ�ε���,��ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(fanhui,SC_ENUM_PROCESS_INFO,SERVICE_WIN32,SERVICE_STATE_ALL,NULL,0,
						&dwSize,&dwServiceNum,NULL,NULL);

	//2.������Ҫ���ڴ�,�ڶ��ε���
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerice = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	//3.�ڶ���ö��
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(fanhui,SC_ENUM_PROCESS_INFO,SERVICE_WIN32,SERVICE_STATE_ALL
								,(PBYTE)pEnumSerice,dwSize,&dwSize,&dwServiceNum,NULL,NULL);

	int a = 0;
	CString sta;

	//3.��������

	for (DWORD i=0;i<dwServiceNum;i++)
	{
		//��ȡ������Ϣ
		//������
		pEnumSerice[i].lpServiceName;
		sta.Format(_T("%s"), pEnumSerice[i].lpServiceName);
		m_service.InsertItem(a, sta);

		
		sta.Format(_T("%s"), pEnumSerice[i].lpDisplayName);
		m_service.SetItemText(a,2,sta);
		//����״̬
		pEnumSerice[i].ServiceStatusProcess.dwCurrentState;  //��
		sta.Format(_T("%d"), pEnumSerice[i].ServiceStatusProcess.dwProcessId);
		m_service.SetItemText(a,1, sta);


		//����״̬
		switch (pEnumSerice[i].ServiceStatusProcess.dwCurrentState)
		{
		case SERVICE_CONTINUE_PENDING:
		case SERVICE_PAUSE_PENDING:
		case SERVICE_PAUSED:
			sta = _T("��ͣ�����");
			m_service.SetItemText(a, 3, sta);
			break;
		case SERVICE_RUNNING:
		case SERVICE_START_PENDING:
			sta = _T("��������");
			m_service.SetItemText(a, 3, sta);
			break;
		case SERVICE_STOP_PENDING:
			
			sta = _T("ֹͣ����");
			m_service.SetItemText(a, 3, sta);
			break;
		case SERVICE_STOPPED:
		
			sta = _T("������̬");
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


// CService ��Ϣ�������


BOOL CService::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_service.InsertColumn(0, L"����", LVCFMT_LEFT, 140);
	m_service.InsertColumn(1, L"ID", LVCFMT_LEFT, 40);
	m_service.InsertColumn(2, L"��ϸ��Ϣ", LVCFMT_LEFT, 240);
	m_service.InsertColumn(3, L"״̬", LVCFMT_LEFT, 100);
	servicea();//���ط���
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CService::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SERVICE_STATUS  ssStatus;
	SC_HANDLE hSCM = OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	
	*pResult = 0;

	// ��ȡ�Ҽ�������λ��
	CPoint	pt;
	GetCursorPos(&pt);
	// �Ҽ���������
	CMenu mu;
	mu.LoadMenu(IDR_MENU2);
	CMenu * pSub = mu.GetSubMenu(0);
	// ��ȡ�û������
	UINT unMenuId = 0;
	unMenuId = TrackPopupMenu(pSub->m_hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,
		pt.x, pt.y, 0, m_hWnd, 0);
	switch (unMenuId) {
	case ID_32774:// ��������
	{
		// ��������
		CString str;
		for (int i = 0; i < m_service.GetItemCount(); i++)
		{
			if (m_service.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			{
				str.Format(_T("ѡ���˵�%d��"), i);
				//	AfxMessageBox(str);
				CString my;
				//��ȡ����
				my = (CString)m_service.GetItemText(i, 0);
				DWORD dwBytesNeeded;
				//��ȡ������ƹ�������� ��ȡ���е�
				if (hSCM == NULL)
				{
					MessageBox(L"Ȩ�޲��㣬�޷�����");
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
						MessageBox(L"�����Ѿ���������");
					}
					else if (ssStatus.dwCurrentState == SERVICE_STOPPED)
					{
						StartService(hService, 0, NULL);
						CloseServiceHandle(hService);
						CloseServiceHandle(hSCM);
						MessageBox(L"����������");
						return;
					}
				}
				CloseServiceHandle(hService);
				CloseServiceHandle(hSCM);
				

			}
		}
		break;
	}
	case ID_32775:// �رշ���
	{
		CString str;
		for (int i = 0; i < m_service.GetItemCount(); i++)
		{
			if (m_service.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			{
				str.Format(_T("ѡ���˵�%d��"), i);
				CString my;
				//��ȡ����
				my = (CString)m_service.GetItemText(i, 0);
				//�رշ�����
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
