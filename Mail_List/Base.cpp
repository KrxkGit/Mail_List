#include "stdafx.h"
#include "Base.h"


CLASSCOUNT_DEFINITION(CBase);
fstream CBase::rwFile;

CBase::CBase(HWND hDlg)
{
	m_CBaseCount++;
	m_hDlg = hDlg;
}
CBase::CBase()
{
	m_CBaseCount++;
}


CBase::~CBase()
{
	m_CBaseCount--;
}

void CBase::OpenFile()
{
	szFilePath[0] = '\0';
	OPENFILENAME ofn = { 0 };
	ofn.hwndOwner = m_hDlg;
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = _T("通讯录文件(*.mail)\0*.mail\0\0");//此处修改文件类型
	ofn.lpstrFile = szFilePath;
	ofn.nMaxFile = _countof(szFilePath);
	GetOpenFileName(&ofn);
	
	rwFile.open(szFilePath, ios::in| ios::binary);//读方式打开文件
	readFile();//虚函数读文件
}

void CBase::SaveFile()
{
	szFilePath[0] = '\0';
	OPENFILENAME ofn = { 0 };
	ofn.hwndOwner = m_hDlg;
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = _T("通讯录文件(*.mail)\0*.mail\0\0");//此处修改文件类型
	ofn.lpstrFile = szFilePath;
	ofn.lpstrDefExt = _T(".mail");
	ofn.nMaxFile = _countof(szFilePath);
	GetSaveFileName(&ofn);

	rwFile.open(szFilePath, ios::out | ios::binary);//写方式打开文件
	writeFile();//虚函数写文件

}

void CBase::CloseFile()
{
	rwFile.close();
}
