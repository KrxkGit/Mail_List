// Mail_List.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Mail_List.h"
#include"RunByWindow.h"
#include"MainDlg.h"
//Ƥ�����
#include"FindLoadSkin.h"
#pragma comment(lib,"SkinPPWTL.lib")

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	hInst = hInstance;
	FindLoadSkin();//����Ƥ��

	//����ģʽ�²�ִ�У�Release�汾ִ��
#ifndef _DEBUG
	RunByWindow(true);//Ĭ�Ͽ�������
#endif
	BindFileType();
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGMAIN), NULL,MainDlg);

	skinppExitSkin();
	return 0;
}

VOID BindFileType()//���ļ�����
{
	TCHAR name[] = _T("mail_file");
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, _countof(szPath));

	HKEY hKey;
	DWORD dwSize = sizeof(szPath);

	RegCreateKeyEx(HKEY_CLASSES_ROOT, _T(".mail"), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, 0, &hKey, NULL);
	RegSetValueEx(hKey, NULL, NULL, REG_SZ, (LPBYTE)name, _countof(name));
	RegCloseKey(hKey);
	
	RegCreateKeyEx(HKEY_CLASSES_ROOT, _T("mail_file\\DefaultIcon") , NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, 0, &hKey, NULL);
	RegSetValueEx(hKey, NULL, NULL, REG_SZ, (LPBYTE)szPath, _countof(szPath));
	RegCloseKey(hKey);

	RegCreateKeyEx(HKEY_CLASSES_ROOT, _T("mail_file\\shell\\open\\command"), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, 0, &hKey, NULL);
	RegSetValueEx(hKey, NULL, NULL, REG_SZ, (LPBYTE)szPath, _countof(szPath));
	RegCloseKey(hKey);
}