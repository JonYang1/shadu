// MyTab.cpp : 实现文件
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



// CMyTab 消息处理程序





void CMyTab::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	//添加接环按钮
	
	InsertItem(0, L"垃圾清理");
	InsertItem(1, L"任务管理器");
	InsertItem(2, L"杀毒");
	InsertItem(3, L"cpu");
	
	InsertItem(4, L"PE文件");
	//创建点坐标(范围)
	InsertItem(5, L"系统服务");
	
	CRect rt;
	GetClientRect(rt);
	rt.DeflateRect(0, 20, 0, 0);
	//创建显示控件
	m_dlgB.Create(IDD_DIALOG2, this);
	MyJingc.Create(IDD_JINC, this);
	cpu.Create(IDD_CPU, this);
	shadu.Create(IDD_SHADU, this);
	service.Create(IDD_SERVICE, this);

	pezhu.Create(IDD_DIALOG1, this);
	//垃圾清理
	m_dlgB.MoveWindow(rt);
	m_dlgB.ShowWindow(SW_SHOW);
	m_pDlg[0] = &m_dlgB;
	//cpu
	cpu.MoveWindow(rt);
	m_pDlg[3] = &cpu;
	//杀毒
	shadu.MoveWindow(rt);
	m_pDlg[2] = &shadu;
	//进程
	MyJingc.MoveWindow(rt);
	m_pDlg[1] = &MyJingc;
	//服务
	service.MoveWindow(rt);
	m_pDlg[5] = &service;
	//PE文件
	pezhu.MoveWindow(rt);
	m_pDlg[4] = &pezhu;
	//设置默认为第一个
	
	//SetCurSel(0);
	CTabCtrl::PreSubclassWindow();
}


void CMyTab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
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
