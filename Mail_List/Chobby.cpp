#include "stdafx.h"
#include "Chobby.h"

CLASSCOUNT_DEFINITION(Chobby);


void Chobby::Chobby_add_front(wstring content)
{
	hobby.push_front(content);
}

void Chobby::Chobby_add_back(wstring content)
{
	hobby.push_back(content);
}

void Chobby::Chobby_del_front()
{
	hobby.pop_front();
}

void Chobby::Chobby_del_back()
{
	hobby.pop_back();
}

void Chobby::Chobby_del_repeat()
{
	hobby.unique();
}

void Chobby::Chobby_del_spec(wstring content)
{
	hobby.remove(content);
}

void Chobby::Chobby_clear()
{
	hobby.clear();
}

void Chobby::Chobby_sort()
{
	hobby.sort();
}

void Chobby::Chobby_insert(wstring ele_find,wstring ele_insery)
{
	iter = find(hobby.begin(), hobby.end(), ele_find);
	if (iter != hobby.end())
	{
		hobby.insert(iter, ele_insery);
	}
}

bool Chobby::Chobby_find(wstring para)
{
	iter = find(hobby.begin(), hobby.end(), para);
	if (iter != hobby.end())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Chobby::writeFile()
{
	UINT hobsize = hobby.size();
	UINT strsize;
	rwFile.write((char*)&hobsize, sizeof(UINT));
	list<wstring>::iterator hobite = hobby.begin();
	for (; hobite != hobby.end(); hobite++) {
		strsize = ((*hobite).length()+1)*sizeof(wchar_t);
		rwFile.write((char*)&strsize,sizeof(UINT));
		rwFile.write((char*)((*hobite).data()), strsize);
	}
}

void Chobby::readFile()
{
	UINT hobsize,strsize;
	wstring strhob;
	rwFile.read((char*)&hobsize, sizeof(UINT));
	
	for (UINT i = 0; i < hobsize; i++) {
		rwFile.read((char*)&strsize, sizeof(UINT));
		char *p = new char[strsize];
		rwFile.read((char*)p, strsize);
		strhob = (wchar_t*)p;
		delete[]p;
		Chobby_add_back(strhob);//Ìí¼Ó°®ºÃ
	}
}

