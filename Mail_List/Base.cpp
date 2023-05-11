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
	ofn.lpstrFilter = _T("ͨѶ¼�ļ�(*.mail)\0*.mail\0\0");//�˴��޸��ļ�����
	ofn.lpstrFile = szFilePath;
	ofn.nMaxFile = _countof(szFilePath);
	GetOpenFileName(&ofn);
	
	rwFile.open(szFilePath, ios::in| ios::binary);//����ʽ���ļ�
	readFile();//�麯�����ļ�
}

void CBase::SaveFile()
{
	szFilePath[0] = '\0';
	OPENFILENAME ofn = { 0 };
	ofn.hwndOwner = m_hDlg;
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = _T("ͨѶ¼�ļ�(*.mail)\0*.mail\0\0");//�˴��޸��ļ�����
	ofn.lpstrFile = szFilePath;
	ofn.lpstrDefExt = _T(".mail");
	ofn.nMaxFile = _countof(szFilePath);
	GetSaveFileName(&ofn);

	rwFile.open(szFilePath, ios::out | ios::binary);//д��ʽ���ļ�
	writeFile();//�麯��д�ļ�

}

void CBase::CloseFile()
{
	rwFile.close();
}
