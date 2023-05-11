#pragma once

#include "resource.h"


//用于定义计数器。其中前者用于声明，后者用于定义
#define CLASSCOUNT_DECLARE(CLASSNAME) static UINT m_##CLASSNAME##Count
#define CLASSCOUNT_DEFINITION(CLASSNAME) UINT CLASSNAME::m_##CLASSNAME##Count=0

//函数声明
VOID BindFileType();//绑定文件类型