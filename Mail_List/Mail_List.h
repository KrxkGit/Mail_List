#pragma once

#include "resource.h"


//���ڶ��������������ǰ�������������������ڶ���
#define CLASSCOUNT_DECLARE(CLASSNAME) static UINT m_##CLASSNAME##Count
#define CLASSCOUNT_DEFINITION(CLASSNAME) UINT CLASSNAME::m_##CLASSNAME##Count=0

//��������
VOID BindFileType();//���ļ�����