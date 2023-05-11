#include "stdafx.h"
#include "Cinformation.h"

CLASSCOUNT_DEFINITION(CInformation);

//全局变量
list<wstring> relation;//全局关系链表
list<wstring>::iterator ite;
list<CInformation*> bukket;//全局对象链表
list<CInformation*>::iterator spec;
vector<vector<bool>*>::iterator brush;

/*下面开始bir的成员函数(生日)*/
unsigned bir::get_year()
{
    return this->year;
}
unsigned bir::get_month()
{
    return this->month;
}
unsigned bir::get_day()
{
    return this->day;
}

void bir::set_year(unsigned y)
{
    this->year = y;
}
void bir::set_month(unsigned m)
{
    this->month = m;
}
void bir::set_day(unsigned d)
{
    this->day = d;
}

bool bir::DateInRange(LONGLONG nDay)
{
    SYSTEMTIME this_one = {0};
	GetSystemTime(&this_one);
    this_one.wMonth = this->month;
    this_one.wDay = this->day;

    SYSTEMTIME st;
    GetSystemTime(&st);
    FILETIME ft, ftu;
    SystemTimeToFileTime(&st, &ft);
    SystemTimeToFileTime(&this_one, &ftu);

    LARGE_INTEGER lst = { ft.dwLowDateTime, ft.dwHighDateTime };
    LARGE_INTEGER lstu = { ftu.dwLowDateTime, ftu.dwHighDateTime };
    LARGE_INTEGER liDiffer, lisub;
    liDiffer.QuadPart = (LONGLONG)10000000 * 3600 * 24 * nDay;
    lisub.QuadPart = abs(lst.QuadPart - lstu.QuadPart);

    if (lisub.QuadPart <= liDiffer.QuadPart) 
    {
        return true;
    }
    else 
    {
        return false;
    }

}





/*下面开始con的成员函数(联系方式)*/

wstring con::get_qq()
{
    return this->qq;
}
wstring con::get_telephone()
{
    return this->telephone;
}
wstring con::get_wechat()
{
    return this->wechat;
}
wstring con::get_email()
{
    return this->email;
}

void con::set_qq(wstring q)
{
    this->qq = q;
}
void con::set_telephone(wstring ph)
{
    this->telephone = ph;
}
void con::set_wechat(wstring wc)
{
    this->wechat = wc;
}
void con::set_email(wstring e)
{
    this->email = e;
}

//关系链表relation相关操作--------------


void add_rel(wstring content)
{
    relation.push_back(content);
    for (spec = bukket.begin(); spec != bukket.end(); spec++)
    {
        for (brush = ((*spec)->rechart)->begin(); brush != ((*spec)->rechart)->end(); brush++)
        {
            (*brush)->push_back(false);
        }
    }
}

void del_rel(wstring content)
{
    for (spec = bukket.begin(); spec != bukket.end(); spec++)
    {
        for (brush = ((*spec)->rechart)->begin(); brush != ((*spec)->rechart)->end(); brush++)
        {
            vector<bool>::iterator del;
            del = (*brush)->begin();
            for (int i = 0; i < movement(content); i++)
            {
                del++;
            }
            (*brush)->erase(del);
        }
    }
    relation.remove(content);
}

int len_rel()
{
    return relation.size();
}

//--------------

int find_the_rank(wstring content)
{
    int count = 1;
    list<CInformation*>::iterator find;

    for (find = bukket.begin(); find != bukket.end(); find++)
    {
        if (content == (*find)->name)
        {
            return count;
        }
        else
        {
            count++;
        }
    }
    return -1;
}

int movement(wstring content)
{
    int count = 0;
    list<wstring>::iterator po;
    for (po = relation.begin(); po != relation.end(); po++)
    {
        if (*po == content)
        {
            return count;
        }
        else
        {
            count++;
        }
    }
    return -1;
}

void set_rel_to_true(CInformation *person1, CInformation *person2, wstring rel)
{
    CInformation* main_p;
    CInformation* vice_p;
    if ((person1->rechart)->size() > (person2->rechart)->size())
    {
        main_p = person1;
        vice_p = person2;
    }
    else
    {
        main_p = person2;
        vice_p = person1;
    }
    vector<vector<bool>*>::iterator vice_finder;
    vice_finder = ((*main_p).rechart)->begin();
    for (int i = 0; i < find_the_rank(vice_p->name) - 1; i++)
    {
        vice_finder++;
    }
    vector<bool>::iterator rel_finder;
    rel_finder = (*vice_finder)->begin();
    for (int i = 0; i < movement(rel); i++)
    {
        rel_finder++;
    }
    *rel_finder = true;
}

void set_rel_to_false(CInformation* person1, CInformation* person2, wstring rel)
{
    CInformation* main_p;
    CInformation* vice_p;
    if ((person1->rechart)->size() > (person2->rechart)->size())
    {
        main_p = person1;
        vice_p = person2;
    }
    else
    {
        main_p = person2;
        vice_p = person1;
    }
    vector<vector<bool>*>::iterator vice_finder;
    vice_finder = ((*main_p).rechart)->begin();
    for (int i = 0; i < find_the_rank(vice_p->name) - 1; i++)
    {
        vice_finder++;
    }
    vector<bool>::iterator rel_finder;
    rel_finder = (*vice_finder)->begin();
    for (int i = 0; i < movement(rel); i++)
    {
        rel_finder++;
    }
    *rel_finder = false;
}

bool query_rel(CInformation* person1, CInformation* person2, wstring rel)
{
    CInformation* main_p;
    CInformation* vice_p;
    if ((person1->rechart)->size() > (person2->rechart)->size())
    {
        main_p = person1;
        vice_p = person2;
    }
    else
    {
        main_p = person2;
        vice_p = person1;
    }
    vector<vector<bool>*>::iterator vice_finder;
    vice_finder = ((*main_p).rechart)->begin();
    for (int i = 0; i < find_the_rank(vice_p->name) - 1; i++)
    {
        vice_finder++;
    }
    vector<bool>::iterator rel_finder;
    rel_finder = (*vice_finder)->begin();
    for (int i = 0; i < movement(rel); i++)
    {
        rel_finder++;
    }
    return *rel_finder;
}

vector<CInformation*>* find_all_rel(CInformation* center_person, wstring rel)
{
    vector<CInformation*>* t = new vector<CInformation*>;
    for (spec = bukket.begin(); spec != bukket.end(); spec++)
    {
        if (query_rel(center_person, *spec, rel) == true)
        {
            t->push_back(*spec);
        }
    }
    return t;
}

bool know_each_other(CInformation* person1, CInformation* person2)
{
    bool know_each = false;
    for (ite = relation.begin(); ite != relation.end(); ite++)
    {
        if (query_rel(person1, person2, *ite))
        {
            know_each = true;
        }
    }
    return know_each;
}

vector<vector<CInformation*>>* have_party(CInformation* organizer)
{
    vector<vector<CInformation*>>* t = new vector<vector<CInformation*>>;

    vector<CInformation*>* relevant = new vector<CInformation*>;//和organizer有直接关系的新建容器
    relevant->push_back(organizer);
    for (spec = bukket.begin(); spec != bukket.end(); spec++)
    {
        if (know_each_other(organizer, *spec)==1)
        {
            relevant->push_back(*spec);
        }
    }

    vector<vector<CInformation*>> set = subsets(*relevant);//求该容器的子集，存入set中
    vector<vector<CInformation*>>::iterator searching;
    for (searching = set.begin(); searching != set.end(); searching++)
    {
        if (full_graph(*searching)==1)
        {
            t->push_back(*searching);
        }
    }
    delete relevant;
    relevant = NULL;

    vector<vector<CInformation*>>* final = new vector<vector<CInformation*>>;
    vector<vector<CInformation*>>::iterator final_p;
    for (final_p = t->begin(); final_p != t->end(); final_p++)
    {
        if (final_p->size() >= 3)
        {
            final->push_back(*final_p);
        }
    }

    delete t;
    t = NULL;
    return final;
}

bool full_graph(vector<CInformation*> x)
{
    vector<CInformation*>::iterator itera;
    vector<CInformation*>::iterator iterat;
    for (itera=x.begin();itera!=x.end();itera++)
    {
        for (iterat = x.begin(); iterat != x.end(); iterat++)
        {
            if (itera == iterat)
            {
                break;
            }
            else
            {
                if (know_each_other(*itera, *iterat)==false)
                {
                    return false;
                }
            }
        }
    }
    return true;
}


vector<vector<CInformation*>> subsets(vector<CInformation*>& ori)
{
    vector<vector<CInformation*>> set;
    set.push_back(vector<CInformation*>());//初始子集是空集
    for (int i = 0; i < (int)ori.size(); i++)
    {
        int size = set.size();
        for (int j = 0; j < size; ++j)
        {
            vector<CInformation*> tmp = set[j];//对于已经求出的每个子集
            tmp.push_back(ori[i]);//将第i个元素加入其中
            set.push_back(tmp);
        }
    }
    return set;
}

CInformation::CInformation()
{
	m_CInformationCount++;

	bukket.push_back(this);
	this->rechart = new vector<vector<bool>*>(bukket.size());
	vector<bool>::iterator sweep;
	for (brush = this->rechart->begin(); brush != this->rechart->end(); brush++)
	{
		*brush = new vector<bool>(len_rel());
		for (sweep = (*brush)->begin(); sweep != (*brush)->end(); sweep++)
		{
			*sweep = false;
		}
	}
}


CInformation::~CInformation()
{
	m_CInformationCount--;

	if (find_the_rank(this->name) != -1)
	{
		int move = find_the_rank(this->name) - 1;

		spec = bukket.begin();
		for (int i = 0; i < move; i++)
		{
			spec++;
		}
		list<CInformation*>::iterator tempo = spec;
		spec++;
		vector<vector<bool>*>::iterator tem;
		brush = ((*tempo)->rechart)->begin();
		brush += move;
		delete* brush;
		*brush = NULL;
		tem = brush;
		((*tempo)->rechart)->erase(tem);
		list<CInformation*>::iterator temp;
		temp = bukket.begin();
		for (int i = 0; i < move; i++)
		{
			temp++;
		}
		bukket.erase(temp);
		bukket.remove(this);
	}
	else
	{
		//error:无法找到该对象信息
		MessageBox(NULL, _T("无法找到该对象信息！\n请重启！"), _T("Mail_List"), MB_ICONERROR);
		TerminateProcess(GetCurrentProcess(), GetLastError());
	}
}

void CInformation::readCon(wstring& str)//辅助读联系方式
{
	UINT strsize;
	rwFile.read((char*)&strsize, sizeof(UINT));
	char*p = new char[strsize];
	rwFile.read((char*)p, strsize);
	str = (wchar_t*)p;
	delete[]p;
}

void CInformation::readFile()
{
	UINT namesize =0;
	wstring sz;
	char *ptemp;
	rwFile.read((char*)&namesize, sizeof(UINT));
	ptemp = new char[namesize];
	rwFile.read((char*)ptemp, namesize);
	sz += (wchar_t*)ptemp;
	set_name(sz);
	delete[]ptemp;

	rwFile.read((char*)&rank, sizeof(rank));
	rwFile.read((char*)&gender, sizeof(gender));

	rwFile.read((char*)&birthday, sizeof(bir));
	wstring con;
	readCon(con);
	contact.set_telephone(con);
	readCon(con);
	contact.set_qq(con);
	readCon(con);
	contact.set_wechat(con);
	readCon(con);
	contact.set_email(con);
	
	hobby.readFile();
}

void CInformation::writeCon(wstring& str)//辅助写联系方式
{
	UINT strsize;
	strsize = (str.length() + 1)*sizeof(wchar_t);
	rwFile.write((char*)&strsize, sizeof(UINT));
	rwFile.write((char*)str.data(), strsize);
}

void CInformation::writeFile()
{
	UINT namesize = (name.length()+1)*sizeof(wchar_t);//请注意要+1写入终止符
	rwFile.write((char*)&namesize, sizeof(UINT));
	rwFile.write((char*)name.data(), namesize);

	rwFile.write((char*)&rank, sizeof(rank));
	rwFile.write((char*)&gender, sizeof(gender));

	rwFile.write((char*)&birthday, sizeof(bir));
	//写入联系方式
	writeCon(contact.get_telephone());
	writeCon(contact.get_qq());
	writeCon(contact.get_wechat());
	writeCon(contact.get_email());
	
	hobby.writeFile();
	
}

