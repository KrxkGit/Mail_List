#pragma once

#include"stdafx.h"
#include"RunByWindow.h"
#include"resource.h"
#include"Base.h"
#include"Cinformation.h"
#include"SendMail.h"


//�������װ
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
	CBase theBase;//�ļ���д�ĳ�ʼ������
public:
	//���ݲ���
public:
	// ���ļ�
	void OnOpen();
	// �����ļ�
	void OnSave();
	// ��������
	void OnRunbyWindows();
	// ���ڶԻ���
	void OnAbout();
};

//���ں�����
INT_PTR CALLBACK MainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//���Ի���
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//�����ڡ��Ի���
INT_PTR CALLBACK InfoDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//��Ϣ�Ի���
INT_PTR CALLBACK InfoDetailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//��ϸ��Ϣ�鿴�Ի���
INT_PTR CALLBACK HobbyDetailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//���öԻ���
INT_PTR CALLBACK RelationViewDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//��ϵ��ͼ�Ի���
INT_PTR CALLBACK SearchDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//���ҶԻ���
INT_PTR CALLBACK PartyDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//�ۻ�Ի���
INT_PTR CALLBACK PassWordDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);//����Ի���
INT_PTR CALLBACK MailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
VOID FlushInfoList(CMainDlg& mainDlg);//���ڸ��¸�����Ϣ�б�
VOID DelInfo(HWND hDlg);//ɾ����Ϣ��¼
VOID SetOrReadDataFromReg(BOOL bSet, CHAR szDomain[], CHAR szPostAddr[], CHAR szPostPW[], CHAR szTitle[], CHAR szContent[]);//�ʼ�����