// ShaDu.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "ShaDu.h"
#include "afxdialogex.h"
#include "Md5.h"
#include <TlHelp32.h>
#include <iosfwd>
#include <iostream>
using namespace std;
// CShaDu 对话框

IMPLEMENT_DYNAMIC(CShaDu, CDialogEx)

CShaDu::CShaDu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHADU, pParent)
	, m_strDelete(_T(".exe"))
{

}

CShaDu::~CShaDu()
{
}

void CShaDu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strDelete);
	DDX_Control(pDX, IDC_LIST2, m_ctrListBoxFile);
	DDX_Control(pDX, IDC_LIST3, m_ctlListBoxFolder);
}


BEGIN_MESSAGE_MAP(CShaDu, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CShaDu::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShaDu::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CShaDu::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CShaDu::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CShaDu::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CShaDu::OnBnClickedButton6)
END_MESSAGE_MAP()


// CShaDu 消息处理程序

//指定病毒
void CShaDu::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 遍历之前先清空之前遍历的文件信息和列表框内容
	m_vecFileDelete.clear();
	m_ctrListBoxFile.ResetContent();
	// 获取要遍历的文件夹数目
	int nCount = m_ctlListBoxFolder.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString test;
		// 获取ListBox控件内的文件夹路径
		m_ctlListBoxFolder.GetText(i, test);
		// 遍历文件夹内的所有文件和子文件夹
		TraverseFile(test.GetBuffer(), m_strDelete, m_vecFileDelete);
	}
	// 将遍历到的要删除文件添加到ListBox内显示出来
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		m_ctrListBoxFile.AddString(m_vecFileDelete[i]);
		m_ctrListBoxFile.SelectString(i, m_vecFileDelete[i]);
		
		CStringA ff(m_vecFileDelete[i]);//得到文件的路径
		char * h = md5FileValue(ff.GetBuffer());
		//md5FileValue(m_vecFileDelete[i]);
		CString my;
		my = h;
		m_vecMd5.push_back(my);
		//m_vecMd5.push_back();

	}
}


// 选择要删除的文件夹(遍历)
void CShaDu::OnGetFolderBtn()
{
	// TODO: 在此添加控件通知处理程序代码
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
	UpdateData(FALSE);
	// 用完释放掉之前的“PID”
	CoTaskMemFree(pid);
}
void CShaDu::TraverseFile(
	TCHAR  szPath[MAX_PATH], // 要遍历的文件路径
	const CString &strFilter,// 过滤的文件后缀
	vector<CString> &vecPath)// 保存遍历到的文件
{
	// 向文件夹末尾添加\\*，遍历所有文件
	CString strPath = szPath;
	strPath += L"\\*";
	WIN32_FIND_DATA fi = {};
	// 找到的文件信息放到fi里面
	HANDLE hFile = FindFirstFile(strPath, &fi);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		// 判断是否当前目录或上级目录
		if (!lstrcmp(fi.cFileName, L".") || !lstrcmp(fi.cFileName, L".."))
		{
			continue;
		}
		// 判断是否是文件夹，若是，递归遍历
		if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strDIr = szPath;
			strDIr += L"\\";
			strDIr += fi.cFileName;
			TraverseFile(strDIr.GetBuffer(), strFilter, vecPath);
			continue;
		}
		// 获取文件扩展名，用于判断是否垃圾文件后缀
		TCHAR *p = PathFindExtension(fi.cFileName);
		// Find返回查找的子串p在m_Delete中的索引，若找不到返回-1
		if (-1 == strFilter.Find(p))
		{
			continue;
		}
		// 经过重重筛选，终于找到要删除的文件名了，和之前的路径构造成完整路径
		CString strDIr = szPath;
		strDIr += L"\\";
		strDIr += fi.cFileName;
		// 保存进路径向量里
		vecPath.push_back(strDIr);
	} while (FindNextFile(hFile, &fi));

}
BOOL CShaDu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//MD5查杀
void CShaDu::OnBnClickedButton2()
{
	//读取文件
	
	CFile file;
	file.Open(L"1.txt",CFile::modeRead, NULL);
	char pbuff[1000] = {0};  //	注意 "\r\n"
	file.Read(pbuff,size(pbuff));
	file.Close(); //关闭文件
				  //显示成功保存信息
	

	CString m_MD5_Name = (CString)pbuff; //保存读取的MD5值

	CString AA = L"5d6e45d606c955e0ecbd03a664586c10";
	//open()
	for (int i = 0; i < m_vecMd5.size(); i++)
	{
		if (m_vecMd5[i] == AA)
		{
			MessageBoxW(L"找到了病毒了");
		}
	}

}

//得到文件路径
void CShaDu::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
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
	if (LB_ERR == m_ctlListBoxFolder.FindString(0, m_strFolder))
	{// 如果m_ListBox里面没有这个新的文件夹路径，再添加进m_ListBox
		m_ctlListBoxFolder.AddString(m_strFolder);
	}
	// 把变量内容更新到对话框
	UpdateData(FALSE);
	// 用完释放掉之前的“PID”
	CoTaskMemFree(pid);
}
//枚举进程信息
BOOL CShaDu::EnumProcess()
{
	// 清空
	m_vecProcess.clear();
	// 遍历
	HANDLE hProcessSnap;//进程快照句柄
	HANDLE hProcess;//进程句柄
	PROCESSENTRY32 stcPe32 = { 0 };//进程快照信息
	stcPe32.dwSize = sizeof(PROCESSENTRY32);
	//1.创建一个进程相关的快照句柄
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) return FALSE;
	//2通过进程快照句柄获取第一个进程信息
	if (!Process32First(hProcessSnap, &stcPe32)) {
		CloseHandle(hProcessSnap);
		return FALSE;
	}
	//3循环遍历进程信息
	do {
		MY_PROCESSINFOA stcInfo = { 0 };
		//3.1获取进程名
		wcscpy_s(stcInfo.szProcess, MAX_PATH, stcPe32.szExeFile);
		//3.2获取完整路径及优先级信息
		//hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, NULL, stcPe32.th32ProcessID);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, NULL, stcPe32.th32ProcessID);
		if (hProcess) {
			// 获取进程完整路径
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hProcess, 0, stcInfo.szFullProcess, &dwSize);
			//GetProcessImageFileName(hProcess, stcInfo.szFullProcess, MAX_PATH);
			stcInfo.dwPriorityClass = GetPriorityClass(hProcess);//获取进程优先级
			CloseHandle(hProcess);
		}
		//3.3获取进程的其他相关信息
		stcInfo.dwPid = stcPe32.th32ProcessID;
		stcInfo.dwThreadCount = stcPe32.cntThreads;
		stcInfo.dwParentProcess = stcPe32.th32ParentProcessID;
		//3.4将获取到的相关信息保存到向量中
		m_vecProcess.push_back(stcInfo);
	} while (Process32Next(hProcessSnap, &stcPe32));
	CloseHandle(hProcessSnap);
	return TRUE;
}

//白名单查杀
void CShaDu::OnBnClickedButton4()
{
	EnumProcess();
	CString nae = L"test.exe";

	for (MY_PROCESSINFOA &stc : m_vecProcess) {
		// 第1列,进程名称
		if (nae == stc.szProcess)
		{
			MessageBoxW(L"白名单找到了");
		}
		
		
	}
}

//全路径查杀
void CShaDu::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<m_vecFileDelete.size();i++)
	{
		int nPos=m_vecFileDelete[i].Find('\\'); //拿到路径
		CString sSubStr = m_vecFileDelete[i]; 
		while (nPos)
		{
			sSubStr = sSubStr.Mid(nPos + 1, sSubStr.GetLength() - nPos);  //取'\'右边字符串  
			nPos = sSubStr.Find('\\');   //不包含'\'，函数值返回-1   
			if (nPos == -1)
			{
				nPos = 0;
				CString chas = L"test.exe";
				if (sSubStr== chas)
				{
					MessageBoxW(L"已经查找到病毒");
					
					//删除API
					//BOOL bRet = RemoveDirectoryA(sSubStr);
					/*if (bRet == 0)
					{
						MessageBoxW(L"删除失败");
					}*/
				}
			}
		}
	}
}


void CShaDu::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
}
