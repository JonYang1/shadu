// Diab.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "Diab.h"
#include "afxdialogex.h"
#include <thread>
#include <shellapi.h>
using std::thread;

// CDiab 对话框

IMPLEMENT_DYNAMIC(CDiab, CDialogEx)

CDiab::CDiab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CDiab::~CDiab()
{
}

void CDiab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtr);
}


BEGIN_MESSAGE_MAP(CDiab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiab::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CDiab::OnBnClickedButton4)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON2, &CDiab::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK5, &CDiab::OnBnClickedCheck5)
END_MESSAGE_MAP()



void CDiab::TraverseFile(
	TCHAR  szPath[MAX_PATH], // 要遍历的文件路径
	const CString &strFilter,// 过滤的文件后缀
	vector<CString> &vecPath)// 保存遍历到的文件
{
	CString strPath = szPath;
	strPath += L"\\*";
	WIN32_FIND_DATA fi = {};
	
	HANDLE hFile = FindFirstFile(strPath, &fi);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		if (!lstrcmp(fi.cFileName, L".") || !lstrcmp(fi.cFileName, L".."))
		{
			continue;
		}
		
		if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strDIr = szPath;
			strDIr += L"\\";
			strDIr += fi.cFileName;
			TraverseFile(strDIr.GetBuffer(), strFilter, vecPath);
			continue;
		}
	
		TCHAR *p = PathFindExtension(fi.cFileName);

		if (-1 == strFilter.Find(p))
		{
			continue;
		}
		CString strDIr = szPath;
		strDIr += L"\\";
		strDIr += fi.cFileName;
		// 保存进路径向量里
		vecPath.push_back(strDIr);
	} while (FindNextFile(hFile, &fi));

}


//  删除垃圾线程
DWORD WINAPI deleteThraedProc(LPVOID lpParam)
{
	CDiab *pDlg = (CDiab*)lpParam;
	pDlg->deleteFile();
	return 0;
}

//扫描线程
DWORD WINAPI ThraedProcA(LPVOID lpParam)
{
	CDiab *pDlg = (CDiab*)lpParam;
	//系统垃圾
	if (pDlg->m_bool[0])
	{
		pDlg->HuiSouZ();

	}
	//回收站垃圾
	if (pDlg->m_bool[1])
	{
		CString strPath; //路径

		BOOL bSuccess = GetTempPath(MAX_PATH, strPath.GetBuffer(MAX_PATH));

		strPath.ReleaseBuffer();
		if (bSuccess)//得到系统临时目录
		{
			pDlg->Find(strPath, L"");
		}
	}
	//浏览器垃圾
	if (pDlg->m_bool[2])
	{
		CString strPath;
		//SHGetSpecialFolderPath 获取系统指定系统路径,
		//CSIDL_INTERNET_CACHE,表示当前用户的Internet Explorer的"Cache"文件夹，
		//例如：C:\Documents and Settings\username\Temporary Internet Files
		BOOL bSuccess = SHGetSpecialFolderPath(NULL,
			strPath.GetBuffer(MAX_PATH), CSIDL_INTERNET_CACHE, FALSE);
		strPath.ReleaseBuffer();
		if (bSuccess)
		{  //得到临时目录，并清空它.
			pDlg->Find(strPath, pDlg->m_strsystem);
		}
	}
	//全盘垃圾
	if (pDlg->m_bool[4])
	{
		pDlg->Find(L"C:", pDlg->m_strsystem);

	}
	//带清理
	if (pDlg->m_bool[5])
	{
		//vs
		if (pDlg->m_vspath.GetLength() != 0)
		{
			pDlg->Find(pDlg->m_vspath, pDlg->m_strVS);
		}
	}
	return 0;
}

//选中
void CDiab::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK1);
	m_bool[0] = pBtn->GetCheck();
	pBtn = (CButton *)GetDlgItem(IDC_CHECK2);
	m_bool[1] = pBtn->GetCheck();
	pBtn = (CButton *)GetDlgItem(IDC_CHECK3);
	m_bool[2] = pBtn->GetCheck();
	pBtn = (CButton *)GetDlgItem(IDC_CHECK4);
	m_bool[4] = pBtn->GetCheck();
	if (m_bool[4])
	{
		m_bool[0] = 1;
		m_bool[1] = 1;
		m_bool[2] = 1;

	}
	pBtn = (CButton *)GetDlgItem(IDC_CHECK5);
	m_bool[4] = pBtn->GetCheck();
	pBtn = (CButton *)GetDlgItem(IDC_CHECK6);
	m_bool[5] = pBtn->GetCheck();
	CreateThread(NULL, 0, ThraedProcA, this, 0, NULL);//负责垃圾清理
	//一个线程来执行操作
	//CreateThread(NULL, 0, ThraedProc, this, 0, NULL);

}


void CDiab::OnBnClickedButton4()
{
	
		TCHAR szPath1[MAX_PATH] = {};
		BROWSEINFO bi = {};
		// 文件夹选择对话框所属窗口句柄
		bi.hwndOwner = m_hWnd;
		bi.pszDisplayName = szPath1;
		// 窗口说明信息
		bi.lpszTitle = L"打开要遍历的文件夹";
		// 对话框带编辑框
		bi.ulFlags = BIF_EDITBOX;
		LPITEMIDLIST pid;
		// 返回标识文件夹路径的“PIDL”，用它找到文件夹路径
		pid = SHBrowseForFolder(&bi);
		SHGetPathFromIDList(pid, szPath1);
		// 把路径更新到路径显示编辑框内
		m_strFolder = szPath1;
		// 把路径更新到路径显示ListBox内
		int a = 0;
		if (LB_ERR == m_listCtr.InsertItem(a, m_strFolder))
		{// 如果m_ListBox里面没有这个新的文件夹路径，再添加进m_ListBox
			m_listCtr.InsertItem(a, m_strFolder);
		}
		// 把变量内容更新到对话框
		UpdateData(FALSE);
		// 用完释放掉之前的“PID”
		CoTaskMemFree(pid);
		//遍历

	
}


BOOL CDiab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listCtr.InsertColumn(0, L"清理文件路径", LVCFMT_LEFT, 300);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//贴图
void CDiab::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
					   //给窗口贴图
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(&dc);
	CBitmap bmpBackground1;
	bmpBackground1.LoadBitmap(IDB_BITMAP1);
	BITMAP bitmap1;
	bmpBackground1.GetBitmap(&bitmap1);
	CBitmap* pbmpPri1 = dcMem1.SelectObject(&bmpBackground1);
	dc.BitBlt(0, -34, 1036, 539, &dcMem1, 0, 0, SRCCOPY);
}



//用来判断选中的按钮并且清除垃圾
void CDiab::OnBnClickedButton2()
{
	CreateThread(NULL, 0, deleteThraedProc, this, 0, NULL);

}
//回收站垃圾
void CDiab::HuiSouZ()
{
	SHQUERYRBINFO RecycleBinInformation;
	ZeroMemory(&RecycleBinInformation, sizeof(RecycleBinInformation));

	RecycleBinInformation.cbSize = sizeof(RecycleBinInformation);


	//为空代表所有的回收站
	SHQueryRecycleBin(NULL, &RecycleBinInformation);

	SHEmptyRecycleBin(NULL, NULL, SHERB_NOSOUND | SHERB_NOPROGRESSUI | SHERB_NOCONFIRMATION);
}


// 删除文件
void CDiab::deleteFile()
{
	for (unsigned int i = 0; i<m_CLEAR_vecPath.size(); i++)
	{
		DeleteFile(m_CLEAR_vecPath[i]);
	}
	m_CLEAR_vecPath.clear();
	m_listCtr.DeleteAllItems();
	m_Number = 0;
}




// 扫描指定路径下的文件，并将路径存入vector

void CDiab::Find(CString lpPath, CString Type)
{
	//1.初始化路径字符串
	CString str = lpPath + L"\\*.*";
	//2.获取第一个文件的信息
	CFileFind finder;
	BOOL bwork = finder.FindFile(str);
	while (bwork)
	{
		bwork = finder.FindNextFile();
		//判断是否为文件夹，以及去掉.或..
		if ((finder.IsDots() == FALSE) && finder.IsDirectory())
		{
			Find(finder.GetFilePath(), Type);
		}
		else
		{
			//获取文件名
			CString name = finder.GetFileName();
			//获取文件大小
			ULONGLONG length = finder.GetLength();
			CString leng;
			leng.Format(L"%dkb", length / 1024);
			//获取文件后缀名/扩展名
			CString Ex = PathFindExtension(name);
			if (Type == L"")
			{
				if (Ex == L"." || Ex == L"..")
				{
					continue;
				}
				m_listCtr.InsertItem(m_Number, NULL);
				m_listCtr.SetItemText(m_Number, 0, name);
				m_listCtr.SetItemText(m_Number, 1, leng);
				m_Number++;
				CString path = finder.GetFilePath();
				m_CLEAR_vecPath.push_back(path);
			}
			else
			{
				if (Type.Find(Ex) != -1)
				{
					if (Ex == L"." || Ex == L"..")
					{
						continue;
					}
					//向空间添加
					m_listCtr.InsertItem(m_Number, NULL);
					m_listCtr.SetItemText(m_Number, 0, name);
					m_listCtr.SetItemText(m_Number, 1, leng);
					m_Number++;
					CString path = finder.GetFilePath();
					m_CLEAR_vecPath.push_back(path);
				}
			}
		}
	}
}


//带修补
void CDiab::OnBnClickedCheck5()
{
	// TODO:  在此添加控件通知处理程序代码
	CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK1);
	pBtn = (CButton *)GetDlgItem(IDC_CHECK5);
	m_bool[4] = pBtn->GetCheck();
	if (m_bool[4])
	{
		m_bool[0] = 1;
		m_bool[1] = 1;
		m_bool[2] = 1;
		
	}
}
