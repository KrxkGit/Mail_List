#pragma once
#include"stdafx.h"
#include "Base.h"
#include"Mail_List.h"


class Chobby : public CBase //ÿ���˸�һ��
{
public:
	CLASSCOUNT_DECLARE(Chobby);
	void Chobby_add_front(wstring);//����ǰ������°���
	void Chobby_add_back(wstring);//�����������°���
	void Chobby_del_front();//ɾ����һ������
	void Chobby_del_back();//ɾ�����һ������
	void Chobby_del_spec(wstring);//ɾ������Ϊ����Ľڵ�
	void Chobby_del_repeat();//ɾ�����ڵ���������ͬ�ڵ㣬ֻ����һ����ȥ�أ�
	void Chobby_clear();//�б����
	void Chobby_sort();//����������
	void Chobby_insert(wstring a, wstring b);//���׸�����Ϊa�Ľڵ�֮ǰ����ڵ㲢���ڵ㸳ֵΪb
	bool Chobby_find(wstring);//Ѱ���Ƿ���������ã��з���1���޷���0
	void writeFile();
	void readFile();
public:
	list<wstring> hobby;//��������
private:
	list<wstring>::iterator iter;//�������������
};






