#pragma once

#include"stdafx.h"
#include"RunByWindow.h"
#include"resource.h"
#include"Base.h"
#include"Cinformation.h"
#include"SendMail.h"


//窗口类封装
class CMainDlg
{
public:
	CMainDlg(HINSTANCE,HWND);
	VOID OnInit();
	~CMainDlg();
	HWND getDlgHandle() { return hDlg; }
private:
	HWND hDlg;
	HINSTANCE hInst;
	CBase theBase;//文件读写的初始化操作
public:
	//数据操作
public:
	// 打开文件
	void OnOpen();
	// 保存文件
	void OnSave();
	// 开机启动
	void OnRunbyWindows();
	// 关于对话框
	void OnAbout();
};

//窗口函声明
INT_PTR CALLBACK MainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//主对话框
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//“关于”对话框
INT_PTR CALLBACK InfoDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//信息对话框
INT_PTR CALLBACK InfoDetailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//详细信息查看对话框
INT_PTR CALLBACK HobbyDetailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//爱好对话框
INT_PTR CALLBACK RelationViewDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//关系视图对话框
INT_PTR CALLBACK SearchDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//查找对话框
INT_PTR CALLBACK PartyDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//聚会对话框
INT_PTR CALLBACK PassWordDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//密码对话框
INT_PTR CALLBACK MailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
VOID FlushInfoList(CMainDlg& mainDlg);//用于更新个人信息列表
VOID DelInfo(HWND hDlg);//删除信息记录
VOID SetOrReadDataFromReg(BOOL bSet, CHAR szDomain[], CHAR szPostAddr[], CHAR szPostPW[], CHAR szTitle[], CHAR szContent[]);//邮件设置