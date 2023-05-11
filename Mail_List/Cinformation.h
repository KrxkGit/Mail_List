#pragma once
#include"stdafx.h"
#include "Base.h"
#include"Chobby.h"


class bir//生日
{
public:
    void set_year(unsigned);//设置年份
    void set_month(unsigned);//设置月份
    void set_day(unsigned);//设置几号
    unsigned get_year();//获得生日年份
    unsigned get_month();//获得生日月份
    unsigned get_day();//获得生日多少日
    bool DateInRange(LONGLONG nDay);//判断此人是否相差nDay生日
protected:
    unsigned year;
    unsigned month;
    unsigned day;
};

class con//联系方式
{
public:
	wstring get_telephone();
    wstring get_qq();
    wstring get_wechat();
    wstring get_email();
    void set_qq(wstring);
    void set_telephone(wstring);
    void set_wechat(wstring);
    void set_email(wstring);
protected:
    wstring qq;
    wstring telephone;
    wstring wechat;
    wstring email;
};

class CInformation : public CBase
{
public:
    CInformation();
    ~CInformation();
    CLASSCOUNT_DECLARE(CInformation);

//个人信息相关
	bir birthday;//生日
	con contact;//联系方式
	Chobby hobby;//爱好管理
//函数接口
public:
    void set_name(wstring n) { this->name = n; }
    wstring get_name() { return this->name; }
    void set_gender(bool b) { this->gender = b; }
    bool get_gender() { return this->gender; }
    void readFile();
    void writeFile();
	void writeCon(wstring& str);//辅助写联系方式
	void CInformation::readCon(wstring& str);//辅助读联系方式
public://关系相关
	friend int find_the_rank(wstring);//获取对象在链表中当前的序号,返回-1则代表未找到
	friend void add_rel(wstring);//关系链表末尾增加关系
	friend void del_rel(wstring);//关系链表删除某个关系
	friend void set_rel_to_true(CInformation *person1, CInformation *person2, wstring rel);
	friend void set_rel_to_false(CInformation* person1, CInformation* person2, wstring rel);
	friend bool query_rel(CInformation* person1, CInformation* person2, wstring rel);//1、2之间是否存在某关系
	friend bool know_each_other(CInformation* person1, CInformation* person2);//两人是否认识（至少有1个关系）
	friend vector<CInformation*>* find_all_rel(CInformation* center_person, wstring rel);//找到所有这个关系的人
	friend vector<vector<CInformation*>>* have_party(CInformation* organizer);//找、所有相互认识的人开party(有一个关系即可)（参数传入组织者）
protected:
    bool gender;//性别 0女 1男
    wstring name;//名字
    vector<vector<bool>*> *rechart;
public://读写文件辅助
	int rank;
};


int movement(wstring);//获得该关系的排序并且-1供迭代器移动,返回-1则未找到
bool full_graph(vector<CInformation*>);//是否是完全图
vector<vector<CInformation*>> subsets(vector<CInformation*>& nums);//找子集
int len_rel();//获取关系链表长度













