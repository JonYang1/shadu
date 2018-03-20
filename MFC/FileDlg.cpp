// FileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "FileDlg.h"
#include "afxdialogex.h"


// CFileDlg 对话框

IMPLEMENT_DYNAMIC(CFileDlg, CDialogEx)

CFileDlg::CFileDlg(IMAGE_FILE_HEADER*	 pFileHdr,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_yingxiang, pParent)
	, m_e_Machina(0)
	, m_e_NumberOfSections(0)
	, m_e_TimeDateStamp(0)
	, m_e_PointerOfSymbolTable(0)
	, m_e_NumberOfSymbols(0)
	, m_e_SizeOfoPtionHeader(0)
	, m_e_Characteristics(0)
{
	this->pFileHdr = pFileHdr;
}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1,  m_e_Machina);
	DDX_Text(pDX, IDC_EDIT2,  m_e_NumberOfSections);
	DDX_Text(pDX, IDC_EDIT31, m_e_TimeDateStamp);
	DDX_Text(pDX, IDC_EDIT45, m_e_PointerOfSymbolTable);
	DDX_Text(pDX, IDC_EDIT16, m_e_NumberOfSymbols);
	DDX_Text(pDX, IDC_EDIT46, m_e_SizeOfoPtionHeader);
	DDX_Text(pDX, IDC_EDIT47, m_e_Characteristics);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileDlg 消息处理程序


BOOL CFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_e_Machina = pFileHdr->Machine;
	m_e_NumberOfSections = pFileHdr->NumberOfSections;
	m_e_TimeDateStamp = pFileHdr->TimeDateStamp;
	m_e_PointerOfSymbolTable = pFileHdr->PointerToSymbolTable;
	m_e_NumberOfSymbols = pFileHdr->NumberOfSymbols;
	m_e_SizeOfoPtionHeader = pFileHdr->SizeOfOptionalHeader;
	m_e_Characteristics = pFileHdr->Characteristics;
	UpdateData(FALSE);
	return TRUE;  
				 
}
