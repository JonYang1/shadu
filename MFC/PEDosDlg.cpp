// PEDosDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "PEDosDlg.h"
#include "afxdialogex.h"


// CPEDosDlg 对话框

IMPLEMENT_DYNAMIC(CPEDosDlg, CDialogEx)

CPEDosDlg::CPEDosDlg(IMAGE_DOS_HEADER* pDosHdr,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DOS, pParent)
	, m_e_magic(0)
	, m_e_cblp(0)
	, m_e_cp(0)
	, m_e_crlc(0)
	, m_e_cparhdr(0)
	, m_e_minalloc(0)
	, m_e_maxalloc(0)
	, my_e_ss(0)
	, my_e_sa(0)
	, my_e_csum(0)
	, my_e_ip(0)
	, my_e_cs(0)
	, my_e_ifarlc(0)
	, my_e_ovno(0)
	, my_e_res(0)
	, my_e_oemid(0)
	, my_e_oeminfo(0)
	, my_e_res2(0)
	, my_e_ifanew(0)
{
	this->pDosHdr = pDosHdr;

}

CPEDosDlg::~CPEDosDlg()
{
}

void CPEDosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_e_magic);
	DDX_Text(pDX, IDC_EDIT2, m_e_cblp);
	DDX_Text(pDX, IDC_EDIT3, m_e_cp);
	DDX_Text(pDX, IDC_EDIT4, m_e_crlc);
	DDX_Text(pDX, IDC_EDIT5, m_e_cparhdr);
	DDX_Text(pDX, IDC_EDIT6, m_e_minalloc);
	DDX_Text(pDX, IDC_EDIT20, m_e_maxalloc);
	DDX_Text(pDX, IDC_EDIT7, my_e_ss);
	DDX_Text(pDX, IDC_EDIT8, my_e_sa);
	DDX_Text(pDX, IDC_EDIT9, my_e_csum);
	DDX_Text(pDX, IDC_EDIT10, my_e_ip);
	DDX_Text(pDX, IDC_EDIT11, my_e_cs);
	DDX_Text(pDX, IDC_EDIT12, my_e_ifarlc);
	DDX_Text(pDX, IDC_EDIT13, my_e_ovno);
	DDX_Text(pDX, IDC_EDIT14, my_e_res);
	DDX_Text(pDX, IDC_EDIT15, my_e_oemid);
	DDX_Text(pDX, IDC_EDIT17, my_e_oeminfo);
	DDX_Text(pDX, IDC_EDIT18, my_e_res2);
	DDX_Text(pDX, IDC_EDIT19, my_e_ifanew);
}


BEGIN_MESSAGE_MAP(CPEDosDlg, CDialogEx)
END_MESSAGE_MAP()


// CPEDosDlg 消息处理程序


BOOL CPEDosDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_e_magic = pDosHdr->e_magic;
	m_e_cblp = pDosHdr->e_cblp;
	m_e_cp = pDosHdr->e_cp;
	m_e_crlc = pDosHdr->e_crlc;
	m_e_cparhdr = pDosHdr->e_cparhdr;
	m_e_maxalloc = pDosHdr->e_maxalloc;
	my_e_ss = pDosHdr->e_ss;
	my_e_sa = pDosHdr->e_sp;
	my_e_csum = pDosHdr->e_csum;
	my_e_ip = pDosHdr->e_ip;
	my_e_cs = pDosHdr->e_cs;
	my_e_ovno = pDosHdr->e_ovno;
	my_e_res = *pDosHdr->e_res;
	my_e_ifarlc = pDosHdr->e_lfarlc;
	my_e_oemid = pDosHdr->e_oemid;
	my_e_oeminfo = pDosHdr->e_oeminfo;
	my_e_res2 = *pDosHdr->e_res2;
	my_e_ifanew = pDosHdr->e_lfanew;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
