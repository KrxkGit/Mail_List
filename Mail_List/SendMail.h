#pragma once
#include"stdafx.h"

const int MAXSIZE = 1024;

class CSendMail
{
public:
	CSendMail(char Domain[],char userName[],char password[], char targetEmail[], char emailTiltle[], char content[]);
	void SendMail();
protected:
	SOCKET CreateConn(char* pWebsite, int port);
	void SendAndRecvMsg(SOCKET sockClient, char* pMessage, int Messagelen, int DoWhat, char* recvBuf, int recvBufLen);
	int GetStrLen(char* pString);
	void StringToBase64(const char *src, char *dst);
	bool FormatEmail(char* pFrom, char* pTo, char* pSubject, char* pMessage, char* Email);
private:
	int srvPort = 25;
	char srvDomain[256];
	char userName[256]; //�Լ�����������
	char password[256]; //�Լ����������
	char targetEmail[256]; //Ҫ���͵��ʼ���ַ
	char emailTitle[256]; //�ʼ�����
	char content[256];//�ʼ�����
};