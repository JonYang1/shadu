#pragma once

#include "Diab.h"
#include "CPU.h"
#include "ShaDu.h"
#include "MyJingc.h"
#include "Service.h"
#include "PeZhu.h"


// CMyTab

class CMyTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTab)

public:
	CMyTab();
	virtual ~CMyTab();
	//¿‡–≈œ¢
	CMyJingc MyJingc;
	CDiab	  m_dlgB;
	CCPU		  cpu;
	CShaDu     shadu;
	CService   service;
	CPeZhu     pezhu;
	CDialogEx* m_pDlg[6];
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	virtual void PreSubclassWindow();
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


