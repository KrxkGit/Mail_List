#pragma once
#include"stdafx.h"
#include"Mail_List.h"

//��ע�⣺��ΪreadFile��writeFile��Ϊ�麯��ʱ���漰�麯�������������ļ���д���ܸ�д�麯��������ʹ��ʧЧ,���������ļ���дʱ����ע�⡣

class CBase
{
public:
	CBase(HWND hDlg);
	CBase::CBase();
	~CBase();
	void OpenFile();
	void SaveFile();
	void CloseFile();
	static fstream rwFile;//�����ļ�������,
protected:
	CLASSCOUNT_DECLARE(CBase);
	TCHAR szFilePath[MAX_PATH];//�ļ�·��
	HWND m_hDlg;
	virtual void readFile() {/*nothing to do*/};
	virtual void writeFile() {/*nothing to do*/ };
};

