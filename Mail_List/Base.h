#pragma once
#include"stdafx.h"
#include"Mail_List.h"

//请注意：因为readFile、writeFile作为虚函数时，涉及虚函数表。而二进制文件读写可能改写虚函数表内容使其失效,整个对象文件读写时尤其注意。

class CBase
{
public:
	CBase(HWND hDlg);
	CBase::CBase();
	~CBase();
	void OpenFile();
	void SaveFile();
	void CloseFile();
	static fstream rwFile;//共用文件流对象,
protected:
	CLASSCOUNT_DECLARE(CBase);
	TCHAR szFilePath[MAX_PATH];//文件路径
	HWND m_hDlg;
	virtual void readFile() {/*nothing to do*/};
	virtual void writeFile() {/*nothing to do*/ };
};

