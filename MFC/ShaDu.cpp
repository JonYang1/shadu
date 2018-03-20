// ShaDu.cpp : ʵ���ļ�
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
// CShaDu �Ի���

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


// CShaDu ��Ϣ�������

//ָ������
void CShaDu::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ����֮ǰ�����֮ǰ�������ļ���Ϣ���б������
	m_vecFileDelete.clear();
	m_ctrListBoxFile.ResetContent();
	// ��ȡҪ�������ļ�����Ŀ
	int nCount = m_ctlListBoxFolder.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString test;
		// ��ȡListBox�ؼ��ڵ��ļ���·��
		m_ctlListBoxFolder.GetText(i, test);
		// �����ļ����ڵ������ļ������ļ���
		TraverseFile(test.GetBuffer(), m_strDelete, m_vecFileDelete);
	}
	// ����������Ҫɾ���ļ���ӵ�ListBox����ʾ����
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		m_ctrListBoxFile.AddString(m_vecFileDelete[i]);
		m_ctrListBoxFile.SelectString(i, m_vecFileDelete[i]);
		
		CStringA ff(m_vecFileDelete[i]);//�õ��ļ���·��
		char * h = md5FileValue(ff.GetBuffer());
		//md5FileValue(m_vecFileDelete[i]);
		CString my;
		my = h;
		m_vecMd5.push_back(my);
		//m_vecMd5.push_back();

	}
}


// ѡ��Ҫɾ�����ļ���(����)
void CShaDu::OnGetFolderBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szPath1[MAX_PATH] = {};
	BROWSEINFO bi = {};
	// �ļ���ѡ��Ի����������ھ��
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szPath1;
	// ����˵����Ϣ
	bi.lpszTitle = L"��Ҫ�������ļ���";
	// �Ի�����༭��
	bi.ulFlags = BIF_EDITBOX;
	LPITEMIDLIST pid;
	// ���ر�ʶ�ļ���·���ġ�PIDL���������ҵ��ļ���·��
	pid = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pid, szPath1);
	// ��·�����µ�·����ʾ�༭����
	m_strFolder = szPath1;
	// ��·�����µ�·����ʾListBox��
	UpdateData(FALSE);
	// �����ͷŵ�֮ǰ�ġ�PID��
	CoTaskMemFree(pid);
}
void CShaDu::TraverseFile(
	TCHAR  szPath[MAX_PATH], // Ҫ�������ļ�·��
	const CString &strFilter,// ���˵��ļ���׺
	vector<CString> &vecPath)// ������������ļ�
{
	// ���ļ���ĩβ���\\*�����������ļ�
	CString strPath = szPath;
	strPath += L"\\*";
	WIN32_FIND_DATA fi = {};
	// �ҵ����ļ���Ϣ�ŵ�fi����
	HANDLE hFile = FindFirstFile(strPath, &fi);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		// �ж��Ƿ�ǰĿ¼���ϼ�Ŀ¼
		if (!lstrcmp(fi.cFileName, L".") || !lstrcmp(fi.cFileName, L".."))
		{
			continue;
		}
		// �ж��Ƿ����ļ��У����ǣ��ݹ����
		if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strDIr = szPath;
			strDIr += L"\\";
			strDIr += fi.cFileName;
			TraverseFile(strDIr.GetBuffer(), strFilter, vecPath);
			continue;
		}
		// ��ȡ�ļ���չ���������ж��Ƿ������ļ���׺
		TCHAR *p = PathFindExtension(fi.cFileName);
		// Find���ز��ҵ��Ӵ�p��m_Delete�е����������Ҳ�������-1
		if (-1 == strFilter.Find(p))
		{
			continue;
		}
		// ��������ɸѡ�������ҵ�Ҫɾ�����ļ����ˣ���֮ǰ��·�����������·��
		CString strDIr = szPath;
		strDIr += L"\\";
		strDIr += fi.cFileName;
		// �����·��������
		vecPath.push_back(strDIr);
	} while (FindNextFile(hFile, &fi));

}
BOOL CShaDu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//MD5��ɱ
void CShaDu::OnBnClickedButton2()
{
	//��ȡ�ļ�
	
	CFile file;
	file.Open(L"1.txt",CFile::modeRead, NULL);
	char pbuff[1000] = {0};  //	ע�� "\r\n"
	file.Read(pbuff,size(pbuff));
	file.Close(); //�ر��ļ�
				  //��ʾ�ɹ�������Ϣ
	

	CString m_MD5_Name = (CString)pbuff; //�����ȡ��MD5ֵ

	CString AA = L"5d6e45d606c955e0ecbd03a664586c10";
	//open()
	for (int i = 0; i < m_vecMd5.size(); i++)
	{
		if (m_vecMd5[i] == AA)
		{
			MessageBoxW(L"�ҵ��˲�����");
		}
	}

}

//�õ��ļ�·��
void CShaDu::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szPath1[MAX_PATH] = {};
	BROWSEINFO bi = {};
	// �ļ���ѡ��Ի����������ھ��
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szPath1;
	// ����˵����Ϣ
	bi.lpszTitle = L"��Ҫ�������ļ���";
	// �Ի�����༭��
	bi.ulFlags = BIF_EDITBOX;
	LPITEMIDLIST pid;
	// ���ر�ʶ�ļ���·���ġ�PIDL���������ҵ��ļ���·��
	pid = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pid, szPath1);
	// ��·�����µ�·����ʾ�༭����
	m_strFolder = szPath1;
	// ��·�����µ�·����ʾListBox��
	if (LB_ERR == m_ctlListBoxFolder.FindString(0, m_strFolder))
	{// ���m_ListBox����û������µ��ļ���·��������ӽ�m_ListBox
		m_ctlListBoxFolder.AddString(m_strFolder);
	}
	// �ѱ������ݸ��µ��Ի���
	UpdateData(FALSE);
	// �����ͷŵ�֮ǰ�ġ�PID��
	CoTaskMemFree(pid);
}
//ö�ٽ�����Ϣ
BOOL CShaDu::EnumProcess()
{
	// ���
	m_vecProcess.clear();
	// ����
	HANDLE hProcessSnap;//���̿��վ��
	HANDLE hProcess;//���̾��
	PROCESSENTRY32 stcPe32 = { 0 };//���̿�����Ϣ
	stcPe32.dwSize = sizeof(PROCESSENTRY32);
	//1.����һ��������صĿ��վ��
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) return FALSE;
	//2ͨ�����̿��վ����ȡ��һ��������Ϣ
	if (!Process32First(hProcessSnap, &stcPe32)) {
		CloseHandle(hProcessSnap);
		return FALSE;
	}
	//3ѭ������������Ϣ
	do {
		MY_PROCESSINFOA stcInfo = { 0 };
		//3.1��ȡ������
		wcscpy_s(stcInfo.szProcess, MAX_PATH, stcPe32.szExeFile);
		//3.2��ȡ����·�������ȼ���Ϣ
		//hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, NULL, stcPe32.th32ProcessID);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, NULL, stcPe32.th32ProcessID);
		if (hProcess) {
			// ��ȡ��������·��
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hProcess, 0, stcInfo.szFullProcess, &dwSize);
			//GetProcessImageFileName(hProcess, stcInfo.szFullProcess, MAX_PATH);
			stcInfo.dwPriorityClass = GetPriorityClass(hProcess);//��ȡ�������ȼ�
			CloseHandle(hProcess);
		}
		//3.3��ȡ���̵����������Ϣ
		stcInfo.dwPid = stcPe32.th32ProcessID;
		stcInfo.dwThreadCount = stcPe32.cntThreads;
		stcInfo.dwParentProcess = stcPe32.th32ParentProcessID;
		//3.4����ȡ���������Ϣ���浽������
		m_vecProcess.push_back(stcInfo);
	} while (Process32Next(hProcessSnap, &stcPe32));
	CloseHandle(hProcessSnap);
	return TRUE;
}

//��������ɱ
void CShaDu::OnBnClickedButton4()
{
	EnumProcess();
	CString nae = L"test.exe";

	for (MY_PROCESSINFOA &stc : m_vecProcess) {
		// ��1��,��������
		if (nae == stc.szProcess)
		{
			MessageBoxW(L"�������ҵ���");
		}
		
		
	}
}

//ȫ·����ɱ
void CShaDu::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i=0;i<m_vecFileDelete.size();i++)
	{
		int nPos=m_vecFileDelete[i].Find('\\'); //�õ�·��
		CString sSubStr = m_vecFileDelete[i]; 
		while (nPos)
		{
			sSubStr = sSubStr.Mid(nPos + 1, sSubStr.GetLength() - nPos);  //ȡ'\'�ұ��ַ���  
			nPos = sSubStr.Find('\\');   //������'\'������ֵ����-1   
			if (nPos == -1)
			{
				nPos = 0;
				CString chas = L"test.exe";
				if (sSubStr== chas)
				{
					MessageBoxW(L"�Ѿ����ҵ�����");
					
					//ɾ��API
					//BOOL bRet = RemoveDirectoryA(sSubStr);
					/*if (bRet == 0)
					{
						MessageBoxW(L"ɾ��ʧ��");
					}*/
				}
			}
		}
	}
}


void CShaDu::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
