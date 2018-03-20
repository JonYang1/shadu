// PeZhu.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC.h"
#include "PeZhu.h"
#include "afxdialogex.h"
#include "PEDosDlg.h"
#include "OptlonalDlg.h"
#include "FileDlg.h"
#include "ResouDlg.h"
#include "MuluDlg.h"
#include "DerivedTabDlg.h"
#include "Cdaoru.h"
#include "CongDingWeiDlg.h"


// CPeZhu �Ի���

IMPLEMENT_DYNAMIC(CPeZhu, CDialogEx)

CPeZhu::CPeZhu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CPeZhu::~CPeZhu()
{
}

void CPeZhu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPeZhu, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPeZhu::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPeZhu::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPeZhu::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPeZhu::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPeZhu::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CPeZhu::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CPeZhu::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CPeZhu::OnBnClickedButton8)
END_MESSAGE_MAP()

LPVOID CPeZhu::MyGetProcAddress(HMODULE hMod,
	char* pszFunctionName)
{
	// 1. �ҵ�Dosͷ
	IMAGE_DOS_HEADER* pDosHdr;
	pDosHdr = (IMAGE_DOS_HEADER*)hMod;
	// 2. �ҵ�Ntͷ
	IMAGE_NT_HEADERS* pNtHdr = NULL;
	pNtHdr = (IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pDosHdr);
	// 3. �ҵ���չͷ
	IMAGE_OPTIONAL_HEADER* pOptHdr = NULL;
	pOptHdr = &pNtHdr->OptionalHeader;
	// 4. �ҵ�����Ŀ¼��
	IMAGE_DATA_DIRECTORY* pDataDir = NULL;
	pDataDir = pOptHdr->DataDirectory;
	// 5. �ҵ�������
	DWORD dwExpRva = pDataDir[0].VirtualAddress;
	// 5.1 �õ�
	IMAGE_EXPORT_DIRECTORY* pExpTab = NULL;
	pExpTab = (IMAGE_EXPORT_DIRECTORY*)(dwExpRva + (DWORD)pDosHdr);
	char* pName = (char*)(pExpTab->Name + (DWORD)pDosHdr);
	DWORD* pExpAddr = (DWORD*)(pExpTab->AddressOfFunctions + (DWORD)hMod);
	DWORD* pExpName = (DWORD*)(pExpTab->AddressOfNames + (DWORD)hMod);
	WORD*  pExpOrd = (WORD*)(pExpTab->AddressOfNameOrdinals + (DWORD)hMod);;
	for (int i = 0; i < pExpTab->NumberOfNames; ++i)
	{
		char* pName = (char*)(pExpName[i] + (DWORD)pDosHdr);
		if (strcmp(pszFunctionName, pName) == 0)
		{
			DWORD dwAddrTabIndex = pExpOrd[i];
			return (LPVOID(pExpAddr[dwAddrTabIndex] + (DWORD)hMod));
		}
	}
	return 0;
}
//************************************
// ��    ��:   RVAToOffset
// ��������:   RVAToOffset
// ʹ��Ȩ��:   public 
// �� �� ֵ:   DWORD	rva��Ӧ���ļ�ƫ��,���ת��ʧ��,����-1
// ʹ��˵��:   IMAGE_DOS_HEADER * pDos rva���ڵ�pe�ļ� ��ƫ��λ��
// ��    ��:   IMAGE_DOS_HEADER * pDos
// ��    ��:   DWORD dwRva ��ת����RVA
//************************************
DWORD CPeZhu::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
{
	IMAGE_SECTION_HEADER* pScnHdr;
	IMAGE_NT_HEADERS* pNtHdr = (IMAGE_NT_HEADERS*)(pDos->e_lfanew + (DWORD)pDos);//�õ�NTͷ�Ŀ�ʼ
	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);//���α�.txt
	DWORD dwNumberOfScn = pNtHdr->FileHeader.NumberOfSections;//�õ����εĸ���
															  // �������������ҵ���������
	for (int i = 0; i<dwNumberOfScn; ++i)
	{
		//�ж����RVA�Ƿ���һ�����εķ�Χ��
		DWORD dwEndOfSection = pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData;
		if (dwRva >= pScnHdr[i].VirtualAddress && dwRva<dwEndOfSection)
		{
			//����RVA�������ڵ�ƫ��:rva��ȥ�׵�ַ
			DWORD dwOffset = dwRva - pScnHdr[i].VirtualAddress;
			//  ��������ƫ�Ƽ������ε��ļ���ʼƫ��
			return dwOffset + pScnHdr[i].PointerToRawData;
		}
	}
	return 0;

}
//������ʼ��
void CPeZhu::init()
{
	//1.���ļ���ȡ���ڴ�  DLL.dll  test.exe
	hFile = CreateFile(L"DLL.dll", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("�ļ�������");
	}
	//�ļ���С
	dwFileSize = GetFileSize(hFile, 0);
	//����new    һ���ռ�,������Ŷ�ȡ������
	pBuf = new BYTE[dwFileSize];
	ReadFile(hFile, pBuf, dwFileSize, &dwRead, NULL);

 //	byte my[33] = "6d6663313430752e646c6c00c9004372";//������
	//byte bidui[33];
	//int a = 0;
 //	//������ȶ�
 //	 for (byte i=0;i<dwFileSize;i+=33)
 //	 {
 //		 for (int i=0;i<33;i++)
	//	 {
	//		 
 //			 //ȡ33���ַ�
 //			 bidui[i] = pBuf[a+i];
	//		 a++;

 //		 }
	//	 for (int i=0;i<33;++i)
	//	 {
	//		 if (bidui[i] == pBuf[a + i])
	//		 {
	//			 MessageBoxW(L"���ҵ�");
	//			 break;
	//		 }
	//		
	//		 
	//	 }
 //		 ////һ�αȽϵĴ�С
 //		 //if ( *my==*bidui) //��ַ���
 //		 //{
 //			// MessageBoxW(L"���ҵ�");
 //			// break;
 //		 //}
 //		 pBuf[dwFileSize-33]= pBuf[dwFileSize-33]- bidui[33];
	//	 dwFileSize=dwFileSize - 33;

 //	 }



	//������������DOSͷ������
	pDosHdr = (IMAGE_DOS_HEADER*)pBuf;
	pNtHdr = (IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pBuf); //�ҵ�NTͷ��λ��

																   //�ж��Ƿ���һ����Ч��PE�ļ�
	if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE || pNtHdr->Signature != IMAGE_NT_SIGNATURE)
	{

		MessageBoxW(L"������Ч��PE�ļ�");


	}
	pFileHdr = &pNtHdr->FileHeader;//�ļ�ͷ�ĵ�ַ
								   //INT A= pFileHdr->NU;
	poptHdr = &pNtHdr->OptionalHeader;//��չͷ
									  //�ж�PE�ļ���32λ����64λ��
	if (poptHdr->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		MessageBoxW(L"32λ��PE�ļ�");

	}
	else if (poptHdr->Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		MessageBoxW(L"64λ��PE�ļ�");
	}
	dwNtHeaderSize = sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + pFileHdr->SizeOfOptionalHeader;
	pScnHdr = (IMAGE_SECTION_HEADER*)((DWORD)pNtHdr + dwNtHeaderSize);//???
	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);
	//�ڶ�������
	//����Ҫ�ӽṹ��Ĵ�С
	dwNtHeaderSizea = sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + pFileHdr->SizeOfOptionalHeader + sizeof(IMAGE_SECTION_HEADER);
	pScnHdB = (IMAGE_SECTION_HEADER*)((DWORD)pNtHdr + dwNtHeaderSizea);

	pDataDirectory = poptHdr->DataDirectory;//��Դ��ַ
											// �ҵ�������
	DWORD dwExpRva = pDataDirectory[0].VirtualAddress;
	//�õ�RVA���ļ�ƫ��
	DWORD dwExpOfs = RVAToOffset(pDosHdr, dwExpRva);

	//������ַ
	pExpTab = (IMAGE_EXPORT_DIRECTORY*)(dwExpOfs + (DWORD)pDosHdr);
	//����DLL����
	dwNameOfs = RVAToOffset(pDosHdr, pExpTab->Name);

	//�õ�������
	char* pDllName = (char*)(dwNameOfs + (DWORD)pDosHdr);


}


//DOSͷ
void CPeZhu::OnBnClickedButton1()
{
	//1.����ͷ�ļ�
	CPEDosDlg dlgPE(pDosHdr);
	dlgPE.DoModal();
}


BOOL CPeZhu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	init();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//NTͷ
void CPeZhu::OnBnClickedButton2()
{
	CFileDlg dlgPE(pFileHdr);
	dlgPE.DoModal();
	
}

//��ѡͷ
void CPeZhu::OnBnClickedButton3()
{
	// ��չͷ��Ϣ
	COptlonalDlg COptDlg(poptHdr);															//���ص����,������,��Դ��...
	COptDlg.DoModal();
	
}

//��Դ��
void CPeZhu::OnBnClickedButton4()
{
	CString stda;
	// resources����
	CResouDlg resoudlg(pDosHdr, hFile);
	resoudlg.DoModal();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//���α�
void CPeZhu::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ͷ��Ϣ
	CString STDA;
	CMuluDlg resoudlg;
	INT nA = pFileHdr->NumberOfSections;// ���α�Ĵ�С
	for (int i = 0; i < nA; ++i)
	{
		STDA = CString(pScnHdr->Name);
		STDA += "\n";
		resoudlg.m_statica += STDA;
		pScnHdr++;//�ӽṹ���С

	}
	resoudlg.DoModal();
}

//�����
void CPeZhu::OnBnClickedButton7()
{
	Cdaoru daoru(pDosHdr);
	daoru.DoModal();

}

//������
void CPeZhu::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDerivedTabDlg dlg(pDosHdr);
	dlg.DoModal();
}


void CPeZhu::OnBnClickedButton8()
{
	CongDingWeiDlg DLG(pDosHdr, pBuf, pFileHdr, pScnHdr);
	DLG.DoModal();
}
