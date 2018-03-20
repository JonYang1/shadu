// MyTab.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "MyTab.h"


// CMyTab

IMPLEMENT_DYNAMIC(CMyTab, CTabCtrl)

CMyTab::CMyTab()
{

}

CMyTab::~CMyTab()
{
}


BEGIN_MESSAGE_MAP(CMyTab, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTab::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTab ��Ϣ�������





void CMyTab::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	//��ӽӻ���ť
	
	InsertItem(0, L"��������");
	InsertItem(1, L"���������");
	InsertItem(2, L"ɱ��");
	InsertItem(3, L"cpu");
	
	InsertItem(4, L"PE�ļ�");
	//����������(��Χ)
	InsertItem(5, L"ϵͳ����");
	
	CRect rt;
	GetClientRect(rt);
	rt.DeflateRect(0, 20, 0, 0);
	//������ʾ�ؼ�
	m_dlgB.Create(IDD_DIALOG2, this);
	MyJingc.Create(IDD_JINC, this);
	cpu.Create(IDD_CPU, this);
	shadu.Create(IDD_SHADU, this);
	service.Create(IDD_SERVICE, this);

	pezhu.Create(IDD_DIALOG1, this);
	//��������
	m_dlgB.MoveWindow(rt);
	m_dlgB.ShowWindow(SW_SHOW);
	m_pDlg[0] = &m_dlgB;
	//cpu
	cpu.MoveWindow(rt);
	m_pDlg[3] = &cpu;
	//ɱ��
	shadu.MoveWindow(rt);
	m_pDlg[2] = &shadu;
	//����
	MyJingc.MoveWindow(rt);
	m_pDlg[1] = &MyJingc;
	//����
	service.MoveWindow(rt);
	m_pDlg[5] = &service;
	//PE�ļ�
	pezhu.MoveWindow(rt);
	m_pDlg[4] = &pezhu;
	//����Ĭ��Ϊ��һ��
	
	//SetCurSel(0);
	CTabCtrl::PreSubclassWindow();
}


void CMyTab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CRect rt;
	GetClientRect(rt);
	rt.DeflateRect(0, 20, 0, 0);

	int nSel = GetCurSel();
	for (int i = 0; i < 6; ++i)
	{
		if (i == nSel)
		{
			m_pDlg[i]->MoveWindow(&rt);
			m_pDlg[i]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pDlg[i]->ShowWindow(SW_HIDE);
		}
	}

}
