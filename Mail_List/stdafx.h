// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>
#include<windowsx.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  在此处引用程序需要的其他头文件
#include<iostream>
#include<fstream>
#include <string>
#include <list>
#include<vector>
#include <algorithm>
using namespace std;
#include<commdlg.h>
#include<sstream>

//套接字
#include <WinSock2.h>
#include <string.h>
#pragma  comment(lib, "ws2_32.lib")	/*链接ws2_32.lib动态链接库*/