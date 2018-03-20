// MuluDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "MuluDlg.h"
#include "afxdialogex.h"


// CMuluDlg 对话框

IMPLEMENT_DYNAMIC(CMuluDlg, CDialogEx)

CMuluDlg::CMuluDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent)
	, m_statica(_T(""))
{

}

CMuluDlg::~CMuluDlg()
{
}

void CMuluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_a, m_statica);
}


BEGIN_MESSAGE_MAP(CMuluDlg, CDialogEx)
END_MESSAGE_MAP()


// CMuluDlg 消息处理程序
