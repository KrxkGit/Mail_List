#pragma once
#include"stdafx.h"
#include "Base.h"
#include"Chobby.h"


class bir//����
{
public:
    void set_year(unsigned);//�������
    void set_month(unsigned);//�����·�
    void set_day(unsigned);//���ü���
    unsigned get_year();//����������
    unsigned get_month();//��������·�
    unsigned get_day();//������ն�����
    bool DateInRange(LONGLONG nDay);//�жϴ����Ƿ����nDay����
protected:
    unsigned year;
    unsigned month;
    unsigned day;
};

class con//��ϵ��ʽ
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

//������Ϣ���
	bir birthday;//����
	con contact;//��ϵ��ʽ
	Chobby hobby;//���ù���
//�����ӿ�
public:
    void set_name(wstring n) { this->name = n; }
    wstring get_name() { return this->name; }
    void set_gender(bool b) { this->gender = b; }
    bool get_gender() { return this->gender; }
    void readFile();
    void writeFile();
	void writeCon(wstring& str);//����д��ϵ��ʽ
	void CInformation::readCon(wstring& str);//��������ϵ��ʽ
public://��ϵ���
	friend int find_the_rank(wstring);//��ȡ�����������е�ǰ�����,����-1�����δ�ҵ�
	friend void add_rel(wstring);//��ϵ����ĩβ���ӹ�ϵ
	friend void del_rel(wstring);//��ϵ����ɾ��ĳ����ϵ
	friend void set_rel_to_true(CInformation *person1, CInformation *person2, wstring rel);
	friend void set_rel_to_false(CInformation* person1, CInformation* person2, wstring rel);
	friend bool query_rel(CInformation* person1, CInformation* person2, wstring rel);//1��2֮���Ƿ����ĳ��ϵ
	friend bool know_each_other(CInformation* person1, CInformation* person2);//�����Ƿ���ʶ��������1����ϵ��
	friend vector<CInformation*>* find_all_rel(CInformation* center_person, wstring rel);//�ҵ����������ϵ����
	friend vector<vector<CInformation*>>* have_party(CInformation* organizer);//�ҡ������໥��ʶ���˿�party(��һ����ϵ����)������������֯�ߣ�
protected:
    bool gender;//�Ա� 0Ů 1��
    wstring name;//����
    vector<vector<bool>*> *rechart;
public://��д�ļ�����
	int rank;
};


int movement(wstring);//��øù�ϵ��������-1���������ƶ�,����-1��δ�ҵ�
bool full_graph(vector<CInformation*>);//�Ƿ�����ȫͼ
vector<vector<CInformation*>> subsets(vector<CInformation*>& nums);//���Ӽ�
int len_rel();//��ȡ��ϵ������













