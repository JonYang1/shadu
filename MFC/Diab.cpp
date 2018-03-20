// Diab.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "Diab.h"
#include "afxdialogex.h"
#include <thread>
#include <shellapi.h>
using std::thread;

// CDiab �Ի���

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
	TCHAR  szPath[MAX_PATH], // Ҫ�������ļ�·��
	const CString &strFilter,// ���˵��ļ���׺
	vector<CString> &vecPath)// ������������ļ�
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
		// �����·��������
		vecPath.push_back(strDIr);
	} while (FindNextFile(hFile, &fi));

}


//  ɾ�������߳�
DWORD WINAPI deleteThraedProc(LPVOID lpParam)
{
	CDiab *pDlg = (CDiab*)lpParam;
	pDlg->deleteFile();
	return 0;
}

//ɨ���߳�
DWORD WINAPI ThraedProcA(LPVOID lpParam)
{
	CDiab *pDlg = (CDiab*)lpParam;
	//ϵͳ����
	if (pDlg->m_bool[0])
	{
		pDlg->HuiSouZ();

	}
	//����վ����
	if (pDlg->m_bool[1])
	{
		CString strPath; //·��

		BOOL bSuccess = GetTempPath(MAX_PATH, strPath.GetBuffer(MAX_PATH));

		strPath.ReleaseBuffer();
		if (bSuccess)//�õ�ϵͳ��ʱĿ¼
		{
			pDlg->Find(strPath, L"");
		}
	}
	//���������
	if (pDlg->m_bool[2])
	{
		CString strPath;
		//SHGetSpecialFolderPath ��ȡϵͳָ��ϵͳ·��,
		//CSIDL_INTERNET_CACHE,��ʾ��ǰ�û���Internet Explorer��"Cache"�ļ��У�
		//���磺C:\Documents and Settings\username\Temporary Internet Files
		BOOL bSuccess = SHGetSpecialFolderPath(NULL,
			strPath.GetBuffer(MAX_PATH), CSIDL_INTERNET_CACHE, FALSE);
		strPath.ReleaseBuffer();
		if (bSuccess)
		{  //�õ���ʱĿ¼���������.
			pDlg->Find(strPath, pDlg->m_strsystem);
		}
	}
	//ȫ������
	if (pDlg->m_bool[4])
	{
		pDlg->Find(L"C:", pDlg->m_strsystem);

	}
	//������
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

//ѡ��
void CDiab::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
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
	CreateThread(NULL, 0, ThraedProcA, this, 0, NULL);//������������
	//һ���߳���ִ�в���
	//CreateThread(NULL, 0, ThraedProc, this, 0, NULL);

}


void CDiab::OnBnClickedButton4()
{
	
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
		int a = 0;
		if (LB_ERR == m_listCtr.InsertItem(a, m_strFolder))
		{// ���m_ListBox����û������µ��ļ���·��������ӽ�m_ListBox
			m_listCtr.InsertItem(a, m_strFolder);
		}
		// �ѱ������ݸ��µ��Ի���
		UpdateData(FALSE);
		// �����ͷŵ�֮ǰ�ġ�PID��
		CoTaskMemFree(pid);
		//����

	
}


BOOL CDiab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listCtr.InsertColumn(0, L"�����ļ�·��", LVCFMT_LEFT, 300);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//��ͼ
void CDiab::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
					   //��������ͼ
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(&dc);
	CBitmap bmpBackground1;
	bmpBackground1.LoadBitmap(IDB_BITMAP1);
	BITMAP bitmap1;
	bmpBackground1.GetBitmap(&bitmap1);
	CBitmap* pbmpPri1 = dcMem1.SelectObject(&bmpBackground1);
	dc.BitBlt(0, -34, 1036, 539, &dcMem1, 0, 0, SRCCOPY);
}



//�����ж�ѡ�еİ�ť�����������
void CDiab::OnBnClickedButton2()
{
	CreateThread(NULL, 0, deleteThraedProc, this, 0, NULL);

}
//����վ����
void CDiab::HuiSouZ()
{
	SHQUERYRBINFO RecycleBinInformation;
	ZeroMemory(&RecycleBinInformation, sizeof(RecycleBinInformation));

	RecycleBinInformation.cbSize = sizeof(RecycleBinInformation);


	//Ϊ�մ������еĻ���վ
	SHQueryRecycleBin(NULL, &RecycleBinInformation);

	SHEmptyRecycleBin(NULL, NULL, SHERB_NOSOUND | SHERB_NOPROGRESSUI | SHERB_NOCONFIRMATION);
}


// ɾ���ļ�
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




// ɨ��ָ��·���µ��ļ�������·������vector

void CDiab::Find(CString lpPath, CString Type)
{
	//1.��ʼ��·���ַ���
	CString str = lpPath + L"\\*.*";
	//2.��ȡ��һ���ļ�����Ϣ
	CFileFind finder;
	BOOL bwork = finder.FindFile(str);
	while (bwork)
	{
		bwork = finder.FindNextFile();
		//�ж��Ƿ�Ϊ�ļ��У��Լ�ȥ��.��..
		if ((finder.IsDots() == FALSE) && finder.IsDirectory())
		{
			Find(finder.GetFilePath(), Type);
		}
		else
		{
			//��ȡ�ļ���
			CString name = finder.GetFileName();
			//��ȡ�ļ���С
			ULONGLONG length = finder.GetLength();
			CString leng;
			leng.Format(L"%dkb", length / 1024);
			//��ȡ�ļ���׺��/��չ��
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
					//��ռ����
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


//���޲�
void CDiab::OnBnClickedCheck5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
