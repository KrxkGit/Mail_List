#pragma once
#include"stdafx.h"
#include "Base.h"
#include"Mail_List.h"


class Chobby : public CBase //每个人各一个
{
public:
	CLASSCOUNT_DECLARE(Chobby);
	void Chobby_add_front(wstring);//链表前面加入新爱好
	void Chobby_add_back(wstring);//链表后面加入新爱好
	void Chobby_del_front();//删除第一个爱好
	void Chobby_del_back();//删除最后一个爱好
	void Chobby_del_spec(wstring);//删除内容为这个的节点
	void Chobby_del_repeat();//删除相邻但是内容相同节点，只保留一个（去重）
	void Chobby_clear();//列表清空
	void Chobby_sort();//按名称排序
	void Chobby_insert(wstring a, wstring b);//在首个内容为a的节点之前插入节点并给节点赋值为b
	bool Chobby_find(wstring);//寻找是否有这个爱好，有返回1，无返回0
	void writeFile();
	void readFile();
public:
	list<wstring> hobby;//爱好链表
private:
	list<wstring>::iterator iter;//定义遍历迭代器
};






