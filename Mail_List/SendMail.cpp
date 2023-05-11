#include"stdafx.h"
#include"SendMail.h"


CSendMail::CSendMail(char Domain[],char userName[], char password[],char targetEmail[], char emailTiltle[], char content[])
{
	srvPort = 25;
	lstrcpyA(this->srvDomain, Domain);
	lstrcpyA(this->userName, userName);//自己的邮箱名称
	lstrcpyA(this->password, password);//自己邮箱的密码
	lstrcpyA(this->targetEmail, targetEmail);//要发送的邮件地址
	lstrcpyA(this->emailTitle, emailTiltle);; //邮件主题
	lstrcpyA(this->content, content);;//邮件正文
}

SOCKET CSendMail::CreateConn(char* pWebsite,int port)
{
	//为建立socket对象做准备，初始化环境
	SOCKET sockClient = socket(AF_INET,SOCK_STREAM,0);     //建立socket对象
	SOCKADDR_IN addrSrv;   
	HOSTENT* pHostent;
    pHostent = gethostbyname(pWebsite);  //得到有关于域名的信息
    
	addrSrv.sin_addr.S_un.S_addr = *((DWORD *)pHostent->h_addr_list[0]);	//得到smtp服务器的网络字节序的ip地址   
	addrSrv.sin_family=AF_INET;   
	addrSrv.sin_port=htons(port);   
	int tf = connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));   //向服务器发送请求  
	if(tf!=0)
	{
		return 0;
		//printf("链接失败\n");
	}
	return sockClient;
}

//int DoWhat是否接受数据
void CSendMail::SendAndRecvMsg(SOCKET sockClient,		//客户端的套接字
					char* pMessage,			//要发送的消息
					int Messagelen,			//消息的长度
					int DoWhat,				//造作的类型
					char* recvBuf,			//接收的缓冲区
					int recvBufLen			//缓冲区长度
					)
{
	char lpMessage[256] = {0};
	memcpy(lpMessage, pMessage, Messagelen);
	printf("\n\n%s \n", lpMessage);
	if (DoWhat == 0)
	{
		send(sockClient, lpMessage, Messagelen, 0);
		memset(recvBuf , 0, recvBufLen);
		DWORD num = recv(sockClient, recvBuf, recvBufLen, 0);     //接收数据
		printf("%s \n", recvBuf); 
		int i = 0;
		while(i != num)
		{
			printf("%02X ", recvBuf[i++]); 
			if((i)%16 == 0)
			{
				printf("\n"); 
			}
		}
		printf("\n");
		
	}
	else if (DoWhat == 1)
	{
		send(sockClient, lpMessage, Messagelen, 0);
		
	}
	else if (DoWhat == 2) //仅仅是接收数据
	{
		memset(recvBuf, 0, recvBufLen);
		DWORD num=recv(sockClient,recvBuf,recvBufLen,0);     //接收数据
		cout << recvBuf << endl;
		UINT i = 0;
		while(i < num)
		{
			printf("%02X ", (BYTE)recvBuf[i++]); 
			if((i)%16 == 0)
			{
				printf("\n"); 
			}
		}
		printf("\n");
	}
	
}

int CSendMail::GetStrLen(char* pString) //得到字符串的长度
{
	int i = 0;
	while(pString[i++] != 0);
	return i-1;
}


void CSendMail::StringToBase64(const char *src, char *dst)
{/*将字符串变为base64编码*/
    int i = 0;
    char *p = dst;
    int d= strlen(src) - 3;
	static const char Base64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //for(i=0;i<strlen(src)-3;i+=3) ;if (strlen(src)-3)<0 there is a buf
	
    for(i=0;i<=d;i+=3)
    {
        *p++ = Base64[((*(src + i)) >> 2) & 0x3f];
        *p++ = Base64[(((*(src + i)) & 0x3) << 4) + ((*(src + i + 1)) >> 4)];
        *p++ = Base64[((*(src + i + 1) & 0xf) << 2) + ((*(src + i + 2)) >> 6)];
        *p++ = Base64[(*(src + i + 2)) & 0x3f];
    }
    if((strlen(src) - i) == 1)
    {
        *p++ = Base64[((*(src + i)) >> 2) & 0x3f];
        *p++ = Base64[((*(src + i)) & 0x3) << 4];
        *p++ = '=';
        *p++ = '=';
    }
    if((strlen(src) - i) == 2)
    {
        *p++ = Base64[((*(src + i)) >> 2) & 0x3f];
        *p++ = Base64[(((*(src + i)) & 0x3) << 4) + ((*(src + i + 1)) >> 4)];
        *p++ = Base64[((*(src + i + 1) & 0xf) << 2)];
        *p++ = '=';
    }
    *p = '\0';
}


bool CSendMail::FormatEmail(char* pFrom, char* pTo, char* pSubject, char* pMessage, char* Email)
{/*格式化要发送的内容*/
	lstrcatA(Email, "From: ");
	lstrcatA(Email, pFrom);
	lstrcatA(Email, "\r\n");
	
	lstrcatA(Email, "To: ");
	lstrcatA(Email, pTo);
	lstrcatA(Email, "\r\n");
	
	lstrcatA(Email, "Subject: ");
	lstrcatA(Email, pSubject);
	lstrcatA(Email, "\r\n");
	
	lstrcatA(Email, "MIME-Version: 1.0");
	lstrcatA(Email, "\r\n");
	lstrcatA(Email, "\r\n");
	
	lstrcatA(Email, pMessage);
	
	lstrcatA(Email, "\r\n.\r\n");
	return TRUE;
}

void CSendMail::SendMail()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD(2, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	SOCKET sockClient = CreateConn(srvDomain, srvPort); //网易smtp邮箱，25号端口

	char buff[MAXSIZE];
    memset(buff, 0, sizeof(char) * MAXSIZE);  //清零

	SendAndRecvMsg(sockClient, 0, 0, 2, buff, MAXSIZE);  //接收数据
	
	char UserNameToSendEmail[256] = {0};
	sprintf_s(UserNameToSendEmail,256, "EHLO %s", "it_is_just_a_test@163.com");
	lstrcatA(UserNameToSendEmail, "\r\n\0");
	SendAndRecvMsg(sockClient, UserNameToSendEmail, GetStrLen(UserNameToSendEmail), 0, buff, MAXSIZE); //既接收也发送

    SendAndRecvMsg(sockClient, "AUTH LOGIN\r\n", strlen("AUTH LOGIN\r\n"), 0, buff, MAXSIZE); //请求登陆
    char pUerName[256] = {0};
	//strstr函数搜索一个字符串在另一个字符串中的第一次出现，并返回第一次出现位置的指针

	DWORD p = strstr(userName,"@") - userName; 
	memcpy(pUerName, userName, p); //得到用户名,如从"13203200199@163.com"得到"13203200199"
    char base[256];
    StringToBase64(pUerName, base); //得到用户名的base64编码

	char str[MAXSIZE];
	memset(str, 0, MAXSIZE);
	sprintf_s(str, MAXSIZE,"%s\r\n", base/*"MTMyMDMyMDAxOTk="*/);
	SendAndRecvMsg(sockClient, str, lstrlenA(str), 0, buff, MAXSIZE); //发送用户名，并接收服务器的返回

	StringToBase64(password, base);
	memset(str, 0, 1024);
	sprintf_s(str,1024, "%s\r\n", base);
	SendAndRecvMsg(sockClient, str, lstrlenA(str),0, buff, MAXSIZE); //发送用户密码，并接收服务器的返回

	char MailFrom[256] = {0};
	sprintf_s(MailFrom, 256,"MAIL FROM: <%s>\r\n", userName);
	
	SendAndRecvMsg(sockClient, MailFrom, lstrlenA(MailFrom), 0, buff, MAXSIZE);
	
	char RcptTo[256] = {0};
	sprintf_s(RcptTo,256, "RCPT TO: <%s>\r\n", targetEmail);
	SendAndRecvMsg(sockClient, RcptTo, lstrlenA(RcptTo), 0, buff, MAXSIZE);

	SendAndRecvMsg(sockClient, "DATA\r\n", lstrlenA("DATA\r\n"), 0, buff, MAXSIZE);

	char Email[1024] = {0};
	FormatEmail(userName, targetEmail, emailTitle, content, Email);
	
	SendAndRecvMsg(sockClient,Email,lstrlenA(Email), 0, buff, MAXSIZE);
	
	SendAndRecvMsg(sockClient,"QUIT\r\n", lstrlenA("QUIT\r\n"), 0, buff, MAXSIZE);

    closesocket(sockClient);   
	WSACleanup();
}
