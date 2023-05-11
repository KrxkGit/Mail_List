// Mail_List.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Mail_List.h"
#include"RunByWindow.h"
#include"MainDlg.h"
//皮肤相关
#include"FindLoadSkin.h"
#pragma comment(lib,"SkinPPWTL.lib")

// 全局变量: 
HINSTANCE hInst;								// 当前实例


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	hInst = hInstance;
	FindLoadSkin();//加载皮肤

	//调试模式下不执行，Release版本执行
#ifndef _DEBUG
	RunByWindow(true);//默认开机启动
#endif
	BindFileType();
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGMAIN), NULL,MainDlg);

	skinppExitSkin();
	return 0;
}

VOID BindFileType()//绑定文件类型
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