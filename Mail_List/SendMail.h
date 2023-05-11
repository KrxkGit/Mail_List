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
	char userName[256]; //自己的邮箱名称
	char password[256]; //自己邮箱的密码
	char targetEmail[256]; //要发送的邮件地址
	char emailTitle[256]; //邮件主题
	char content[256];//邮件正文
};