#include "stdafx.h"
#include "MainDlg.h"
//扩展全局变量作用域
extern HINSTANCE hInst;
extern list<wstring> relation;//全局关系链表
extern list<wstring>::iterator ite;
extern list<CInformation*> bukket;//全局对象链表
extern list<CInformation*>::iterator spec;
extern vector<vector<bool>*>::iterator brush;
//密码相关
#define PWLEN 30//密码长度
TCHAR PassWord[PWLEN];
TCHAR PassWordKey = _T('L');//密钥
BOOL bOpenPass = TRUE;//标记状态，true为打开文件输入密码，FALSE为保存文件获取密码


//窗口函数
VOID OnMainDlgCommand(CMainDlg& mainDlg, WPARAM wParam, LPARAM lParam)//主对话框
{
	switch (LOWORD(wParam))
	{
	case IDM_EXIT:
	case IDCANCEL:
		EndDialog(mainDlg.getDlgHandle(), LOWORD(wParam));
		break;
	case IDM_OPEN:
		mainDlg.OnOpen();
		break;
	case IDM_SAVE:
		mainDlg.OnSave();
		break;
	case IDM_RUNBYWIN:
		mainDlg.OnRunbyWindows(/*参数*/);//此处需要参数
		break;
	case IDM_ABOUT: //关于对话框
		mainDlg.OnAbout();
		break;
	case IDM_ADDINFO://添加个人信息
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGINFO), mainDlg.getDlgHandle(), InfoDlg);
		FlushInfoList(mainDlg);//更新
		break;
	case IDM_DELINFO://删除个人信息
		if (bukket.size() == 0) {
			MessageBox(mainDlg.getDlgHandle(), _T("未添加任何记录，无法使用本功能！"), _T("错误提示"), MB_ICONERROR);
			break;
		}
		DelInfo(mainDlg.getDlgHandle());
		FlushInfoList(mainDlg);//更新
		break;
	case IDM_INFODET://查看信息详情
		if (bukket.size() == 0) {
			MessageBox(mainDlg.getDlgHandle(), _T("未添加任何记录，无法使用本功能！"), _T("错误提示"), MB_ICONERROR);
			break;
		}
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGINFO), mainDlg.getDlgHandle(), InfoDetailDlg);
		FlushInfoList(mainDlg);
		break;
	case IDM_HOBDET://查看爱好
		if (bukket.size() == 0) {
			MessageBox(mainDlg.getDlgHandle(), _T("未添加任何记录，无法使用本功能！"), _T("错误提示"), MB_ICONERROR);
			break;
		}
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGHOB), mainDlg.getDlgHandle(), HobbyDetailDlg);
		break;
	case IDM_RELAVIEW://关系视图
		if (bukket.size() == 0) {
			MessageBox(mainDlg.getDlgHandle(), _T("未添加任何记录，无法使用本功能！"), _T("错误提示"), MB_ICONERROR);
			break;
		}
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGRELATION), mainDlg.getDlgHandle(), RelationViewDlg);
		break;
	case IDM_SEARCH://查找
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGSEARCH), mainDlg.getDlgHandle(), SearchDlg);
		break;
	case IDM_PARTY://生成聚会名单
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGPARTY), mainDlg.getDlgHandle(), PartyDlg);
		break;
	case IDM_PASS://设置密码
		bOpenPass = FALSE;
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGPW), mainDlg.getDlgHandle(), PassWordDlg);
		break;
	}
}

INT_PTR CALLBACK PassWordDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//密码对话框
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDOK) {
			TCHAR TempPassWord[PWLEN];
			ZeroMemory(TempPassWord, sizeof(TCHAR)*PWLEN);

			if (bOpenPass) {//打开文件
				GetDlgItemText(hDlg, IDC_EDIT1, TempPassWord, _countof(TempPassWord));
				for (int i = 0; i < PWLEN-1; i++) {
					TempPassWord[i] ^= PassWordKey;//加解密(留一位作终止符）
				}
				int a = _tcscmp(TempPassWord, PassWord);
				if (_tcscmp(TempPassWord, PassWord)==0) {//密码正确
					EndDialog(hDlg, TRUE);
					return TRUE;
				}
				else {
					EndDialog(hDlg, FALSE);
					return FALSE;
				}
			}
			else {//保存文件
				ZeroMemory(PassWord, sizeof(TCHAR)*PWLEN);
				GetDlgItemText(hDlg, IDC_EDIT1, PassWord, _countof(PassWord));
				for (int i = 0; i < PWLEN-1; i++) {
					PassWord[i] ^= PassWordKey;//加解密(留一位作终止符）
				}
				EndDialog(hDlg, TRUE);
				return TRUE;
			}
		}
		break;
	}
	return (INT_PTR)FALSE;

}

VOID InitPartyDlg(HWND hDlg)
{
	HWND hList = GetDlgItem(GetParent(hDlg), IDC_LIST1);//父窗口
	HWND hList1 = GetDlgItem(hDlg, IDC_LIST1);//本窗口
	int pos = ListBox_GetCurSel(hList);
	TCHAR szOption[MAX_PATH];
	ListBox_GetText(hList, pos, szOption);
	wstring szName = szOption;
	wstringstream stringName(szName);
	szName.empty();
	stringName >> szName;


	CInformation * p = NULL;
	list<CInformation*>::iterator iter = bukket.begin();
	for (; iter != bukket.end(); iter++) {
		if ((*iter)->get_name() == szName) {
			p = *iter;
			break;
		}
	}
	if (p == NULL) {
		MessageBox(hDlg, _T("未找到！"), _T("提示"), MB_ICONERROR);
		EndDialog(hDlg, 1);
		return;
	}

	vector<vector<CInformation*>>*pParty = have_party(p);
	if (pParty->size() == 0){
		MessageBox(hDlg, _T("未找到！"), _T("提示"), MB_ICONERROR);
		EndDialog(hDlg, 1);
		return;
	}
	vector<vector<CInformation*>>::iterator partyite = pParty->begin();
	vector<vector<CInformation*>>::iterator parTemp = partyite;
	UINT size = (*parTemp).size();
	for (; partyite != pParty->end(); partyite++) {
		if (size < (*partyite).size()) {
			size = (*partyite).size();
			parTemp = partyite;
		}
	}
	vector<CInformation*>::iterator InfoRela = parTemp->begin();
	for (; InfoRela != parTemp->end(); InfoRela++) {
		ListBox_AddString(hList1, (*InfoRela)->get_name().data());
	}
}

INT_PTR CALLBACK PartyDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//聚会对话框
{
	UNREFERENCED_PARAMETER(lParam);
	static CMainDlg mainDlg(hInst, hDlg);

	switch (message)
	{
	case WM_INITDIALOG:
		InitPartyDlg(hDlg);
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

VOID InitSearchDlg(HWND hDlg)
{
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	HWND hList = GetDlgItem(hDlg, IDC_LIST1);
	ComboBox_AddString(hCombo, _T("姓名"));
	ComboBox_AddString(hCombo, _T("出生月份"));
	ComboBox_AddString(hCombo, _T("最近N日生日"));
	ComboBox_AddString(hCombo, _T("爱好"));
	ComboBox_AddString(hCombo, _T("电话号码"));
	ComboBox_AddString(hCombo, _T("QQ"));
	ComboBox_AddString(hCombo, _T("WeChat"));
	ComboBox_AddString(hCombo, _T("Email"));
	ComboBox_AddString(hCombo, _T("出生年份"));
	ComboBox_AddString(hCombo, _T("性别"));
}

VOID SearchInfo(HWND hDlg)
{
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	HWND hEdit = GetDlgItem(hDlg, IDC_EDIT1);
	HWND hList = GetDlgItem(hDlg, IDC_LIST1);
	ListBox_ResetContent(hList);
	int cursel = ComboBox_GetCurSel(hCombo);

	//获取参数
	TCHAR sz[MAX_PATH];
	GetWindowText(hEdit, sz, _countof(sz));
	wstring str = sz;
	int num = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, FALSE);
	int gender = -1;
	if (str == _T("男")) {
		gender = 1;
	}
	else if (str == _T("女")) {
		gender = 0;
	}
	else
	{
		gender = -1;//无效
	}

	list<wstring>namelist;

	list<CInformation*>::iterator infoite = bukket.begin();
	for (; infoite != bukket.end(); infoite++) {
		switch (cursel)
		{
		case 0://姓名
			if ((*infoite)->get_name() == str) {
				namelist.push_back(str);
			}
			break;
		case 1://出生月份
			if (num == (*infoite)->birthday.get_month()) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 2://最近N日生日
			if ((*infoite)->birthday.DateInRange(num)) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 3://爱好
			if ((*infoite)->hobby.Chobby_find(str)) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 4://电话号码
			if ((*infoite)->contact.get_telephone() == str) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 5://QQ
			if ((*infoite)->contact.get_qq() == str) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 6://微信
			if ((*infoite)->contact.get_wechat() == str) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 7://邮件
			if ((*infoite)->contact.get_email() == str) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 8://出生年份
			if ((*infoite)->birthday.get_year() == num) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 9://性别
			if ((int)(*infoite)->get_gender() == gender) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		}
	}

	//去重
	namelist.sort();
	namelist.unique();
	list<wstring>::iterator nameite = namelist.begin();
	for (; nameite != namelist.end(); nameite++) {
		ListBox_AddString(hList, (*nameite).data());
	}
}

VOID InitMailDlg(HWND hDlg)
{
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	HWND hList = GetDlgItem(GetParent(hDlg), IDC_LIST1);//父窗口

	CHAR szDomain[MAX_PATH];
	CHAR szPostAddr[MAX_PATH];
	CHAR szPostPW[MAX_PATH];
	CHAR szTitle[MAX_PATH];
	CHAR szContent[MAX_PATH];

	ComboBox_AddString(hCombo, _T("smtp.qq.com"));
	ComboBox_AddString(hCombo, _T("smtp.126.com"));
	ComboBox_AddString(hCombo, _T("smtp.163.com"));

	SetOrReadDataFromReg(TRUE, szDomain, szPostAddr, szPostPW, szTitle, szContent);
	SetWindowTextA(hCombo, szDomain);
	SetDlgItemTextA(hDlg, IDC_EDIT1, szPostAddr);
	SetDlgItemTextA(hDlg, IDC_EDIT4, szPostPW);
	SetDlgItemTextA(hDlg, IDC_EDIT6, szTitle);
	SetDlgItemTextA(hDlg, IDC_EDIT2, szContent);


	int pos = ListBox_GetCurSel(hList);
	TCHAR szOption[MAX_PATH];
	ListBox_GetText(hList, pos, szOption);
	wstring szName = szOption;
	wstringstream stringName(szName);
	szName.empty();
	stringName >> szName;

	CInformation * p = NULL;
	list<CInformation*>::iterator iter = bukket.begin();
	for (; iter != bukket.end(); iter++) {
		if ((*iter)->get_name() == szName) {
			p = *iter;
			break;
		}
	}
	if (p == NULL) {
		MessageBox(hDlg, _T("未找到！"), _T("错误"), MB_ICONERROR);
		EndDialog(hDlg, 1);
		return;
	}
	SetDlgItemText(hDlg, IDC_EDIT5, p->contact.get_email().data());
}

VOID SendMail(HWND hDlg)
{
	CHAR szDomain[MAX_PATH];
	CHAR szPostAddr[MAX_PATH];
	CHAR szPostPW[MAX_PATH];
	CHAR szToAddr[MAX_PATH];
	CHAR szTitle[MAX_PATH];
	CHAR szContent[MAX_PATH];
	GetWindowTextA(GetDlgItem(hDlg, IDC_COMBO1), szDomain, _countof(szDomain));	
	GetDlgItemTextA(hDlg, IDC_EDIT1, szPostAddr, MAX_PATH);
	GetDlgItemTextA(hDlg, IDC_EDIT4, szPostPW, MAX_PATH);
	GetDlgItemTextA(hDlg, IDC_EDIT5, szToAddr, MAX_PATH);
	GetDlgItemTextA(hDlg, IDC_EDIT6, szTitle, MAX_PATH);
	GetDlgItemTextA(hDlg, IDC_EDIT2, szContent, MAX_PATH);
	
	SetOrReadDataFromReg(FALSE, szDomain, szPostAddr, szPostPW, szTitle, szContent);//保存设置

	CSendMail sm(szDomain, szPostAddr, szPostPW, szToAddr, szTitle, szContent);
	sm.SendMail();
	MessageBox(hDlg, _T("发送完成"), _T("发送邮件"), MB_ICONINFORMATION);
}

INT_PTR CALLBACK MailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//邮件对话框
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		InitMailDlg(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			SendMail(hDlg);
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK SearchDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//查找对话框
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		InitSearchDlg(hDlg);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK://查找
			SearchInfo(hDlg);
			break;
		case IDM_INFODET://详细信息
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGINFO), hDlg, InfoDetailDlg);
			break;
		case IDC_SENDMAIL://发送邮件
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGMAIL), hDlg, MailDlg);
			break;
		case IDCANCEL:
			SendMessage(GetParent(hDlg), WM_INITDIALOG, 0, 0);//退出前刷新主窗口
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


VOID FlushRelationView(HWND hDlg)
{
	HWND hParentList = GetDlgItem(GetParent(hDlg), IDC_LIST1);//主对话框List
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);//本对话框Combo
	HWND hListI = GetDlgItem(hDlg, IDC_LIST1);//关系内部（属于关系）
	HWND hListE = GetDlgItem(hDlg, IDC_LIST2);//关系外部（不属于关系）

	ListBox_ResetContent(hListI);
	ListBox_ResetContent(hListE);

	//必须放在重设前
	int currentsel = ComboBox_GetCurSel(hCombo);
	TCHAR sz[MAX_PATH];
	ComboBox_GetText(hCombo, sz, _countof(sz));
	wstring strrel = sz;

	ComboBox_ResetContent(hCombo);

	int pos = ListBox_GetCurSel(hParentList);
	list<CInformation*>::iterator InfoIter = bukket.begin();
	for (int i = 0; i < pos; i++) {
		InfoIter++;
	}
	CInformation* pInfo = *InfoIter;

	//修改窗口标题
	wstring caption = _T("关系视图：");
	caption += pInfo->get_name();
	SetWindowText(hDlg, caption.data());

	list<wstring>InsideName;//关系内
	list<wstring>OutsideName;//关系外


	list<wstring>::iterator relation_iter = relation.begin();
	for (; relation_iter != relation.end(); relation_iter++) {
		//ComboBox
		ComboBox_AddString(hCombo, (*relation_iter).data());


		if ((*relation_iter) != strrel) {//不是所选关系，遍历下一个
			continue;
		}

		InfoIter = bukket.begin();
		for (; InfoIter != bukket.end(); InfoIter++) {
			if (*InfoIter == pInfo) {//跳过自身
				continue;
			}
			if (query_rel(pInfo, *InfoIter, (*relation_iter))) {
				InsideName.push_back((*InfoIter)->get_name());
			}
			else {
				OutsideName.push_back((*InfoIter)->get_name());
			}
		}
	}
	InsideName.sort();//先排序，否则无法去重
	InsideName.unique();
	OutsideName.sort();
	OutsideName.unique();
	list<wstring>::iterator nameiter = InsideName.begin();
	for (; nameiter != InsideName.end(); nameiter++) {
		ListBox_AddString(hListI, (*nameiter).data());
	}
	nameiter = OutsideName.begin();
	for (; nameiter != OutsideName.end(); nameiter++) {
		ListBox_AddString(hListE, (*nameiter).data());
	}

	ComboBox_SetCurSel(hCombo, currentsel);//重设上次选项
}

VOID AddRelationView(HWND hDlg)//添加关系
{
	TCHAR sz[MAX_PATH];
	GetDlgItemText(hDlg, IDC_EDIT1, sz, _countof(sz));
	wstring str = sz;
	add_rel(str);
	SetDlgItemText(hDlg, IDC_EDIT1, _T(""));//清空
}

VOID DelRelationView(HWND hDlg)//删除关系
{
	TCHAR sz[MAX_PATH];
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);//本对话框Combo
	ComboBox_GetText(hCombo, sz, _countof(sz));
	wstring str = sz;
	del_rel(str);


}

VOID BindRelation(HWND hDlg)//绑定
{
	HWND hParentList = GetDlgItem(GetParent(hDlg), IDC_LIST1);//主对话框List
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);//本对话框Combo
	HWND hListE = GetDlgItem(hDlg, IDC_LIST2);//关系外部（不属于关系）

	int pos = ListBox_GetCurSel(hParentList);
	list<CInformation*>::iterator InfoIter = bukket.begin();
	for (int i = 0; i < pos; i++) {
		InfoIter++;
	}
	CInformation* pInfo = *InfoIter;

	TCHAR sz[MAX_PATH];
	ListBox_GetText(hListE, ListBox_GetCurSel(hListE), sz);
	wstring str = sz;

	ComboBox_GetText(hCombo, sz, _countof(sz));
	wstring strrel = sz;

	InfoIter = bukket.begin();
	for (; InfoIter != bukket.end(); InfoIter++) {
		if (str == (*InfoIter)->get_name()) {
			set_rel_to_true(pInfo, *InfoIter, strrel);
			break;
		}
	}
}

VOID UntieRelation(HWND hDlg)//松绑
{
	HWND hParentList = GetDlgItem(GetParent(hDlg), IDC_LIST1);//主对话框List
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);//本对话框Combo
	HWND hListI = GetDlgItem(hDlg, IDC_LIST1);//关系内部（属于关系）

	int pos = ListBox_GetCurSel(hParentList);
	list<CInformation*>::iterator InfoIter = bukket.begin();
	for (int i = 0; i < pos; i++) {
		InfoIter++;
	}
	CInformation* pInfo = *InfoIter;

	TCHAR sz[MAX_PATH];
	ListBox_GetText(hListI, ListBox_GetCurSel(hListI), sz);
	wstring str = sz;

	ComboBox_GetText(hCombo, sz, _countof(sz));
	wstring strrel = sz;

	InfoIter = bukket.begin();
	for (; InfoIter != bukket.end(); InfoIter++) {
		if (str == (*InfoIter)->get_name()) {
			set_rel_to_false(pInfo, *InfoIter, strrel);
			break;
		}
	}
}

INT_PTR CALLBACK RelationViewDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//关系对话框
{
	switch (message)
	{
	case WM_INITDIALOG:
		FlushRelationView(hDlg);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (HIWORD(wParam) == CBN_SELCHANGE && GetDlgItem(hDlg, IDC_COMBO1) == (HWND)lParam) {//选择关系
			//必须先手动更新选项，否则无法获得选项
			HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
			int cursel = ComboBox_GetCurSel(hCombo);
			ComboBox_SetCurSel(hCombo, cursel);
			UpdateWindow(hCombo);
			FlushRelationView(hDlg);
		}
		switch (LOWORD(wParam))
		{
		case IDC_ADDRELA://添加关系
			AddRelationView(hDlg);
			FlushRelationView(hDlg);
			break;
		case IDC_DELRELA://删除关系
			DelRelationView(hDlg);
			FlushRelationView(hDlg);
			break;
		case IDC_BIND://关联
			BindRelation(hDlg);
			FlushRelationView(hDlg);
			break;
		case IDC_UNTIE://松绑
			UntieRelation(hDlg);
			FlushRelationView(hDlg);
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



VOID FlushHobbyList(HWND hDlg)
{
	HWND hWnd = GetDlgItem(GetParent(hDlg), IDC_LIST1);//主窗口List
	HWND hList = GetDlgItem(hDlg, IDC_LIST1);//本窗口List

	//修改标题
	TCHAR sz[MAX_PATH];
	ListBox_GetText(hWnd, ListBox_GetCurSel(hWnd), sz);
	wstring str = _T("爱好：");
	str += sz;
	for (int i = 0; i < (int)str.length(); i++) {
		if (str[i] == ' ') {
			str[i] = '\0';
			break;
		}
	}
	SetWindowText(hDlg, str.data());

	ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("男"));
	ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("女"));
	ListBox_ResetContent(hList);
	int pos = ListBox_GetCurSel(hWnd);

	list<CInformation*>::iterator iter = bukket.begin();
	for (int i = 0; i < pos; i++) {
		iter++;
	}
	Chobby& hobby = (*iter)->hobby;

	list<wstring>::iterator strhobbyiter = hobby.hobby.begin();
	for (; strhobbyiter != hobby.hobby.end(); strhobbyiter++) {
		ListBox_AddString(hList, (*strhobbyiter).data());
	}
}

VOID AddHobby(HWND hDlg)
{
	HWND hWnd = GetDlgItem(GetParent(hDlg), IDC_LIST1);//主窗口List
	int pos = ListBox_GetCurSel(hWnd);

	list<CInformation*>::iterator iter = bukket.begin();
	for (int i = 0; i < pos; i++) {
		iter++;
	}
	Chobby& hobby = (*iter)->hobby;

	TCHAR sz[MAX_PATH];
	GetDlgItemText(hDlg, IDC_EDIT1, sz, _countof(sz));
	wstring str = sz;
	hobby.Chobby_add_back(str);

	hobby.Chobby_del_repeat();//去重
	SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
}

VOID DelHobby(HWND hDlg)
{
	HWND hWnd = GetDlgItem(GetParent(hDlg), IDC_LIST1);//主窗口List
	HWND hList = GetDlgItem(hDlg, IDC_LIST1);//本窗口List
	int pos = ListBox_GetCurSel(hWnd);

	list<CInformation*>::iterator iter = bukket.begin();
	for (int i = 0; i < pos; i++) {
		iter++;
	}
	Chobby& hobby = (*iter)->hobby;
	TCHAR sz[MAX_PATH];
	ListBox_GetText(hList, ListBox_GetCurSel(hList), sz);
	wstring str = sz;

	hobby.Chobby_del_spec(str);
}

VOID ModifyHobby(HWND hDlg)
{
	DelHobby(hDlg);
	AddHobby(hDlg);
}

INT_PTR CALLBACK HobbyDetailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//爱好对话框
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		FlushHobbyList(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_ADDHOB:
			AddHobby(hDlg);
			FlushHobbyList(hDlg);//刷新
			break;
		case IDC_DELHOB:
			DelHobby(hDlg);
			FlushHobbyList(hDlg);//刷新
			break;
		case IDC_MODHOB:
			ModifyHobby(hDlg);
			FlushHobbyList(hDlg);//刷新
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

VOID FlushInfoList(CMainDlg& mainDlg)//用于更新个人信息列表
{
	HWND hWnd = GetDlgItem(mainDlg.getDlgHandle(), IDC_LIST1);
	ListBox_ResetContent(hWnd);

	list<CInformation*>::iterator fluiter = bukket.begin();
	for (; fluiter != bukket.end(); fluiter++) {
		wstring sz;
		sz += (*fluiter)->get_name();
		sz += _T(" ");

		if ((*fluiter)->get_gender()) {
			sz += _T("男");
		}
		else {
			sz += _T("女");
		}
		sz += _T(" ");

		bir& Bir = (*fluiter)->birthday;
		wstringstream strbir;
		strbir << Bir.get_year() << _T("-") << Bir.get_month() << _T("-") << Bir.get_day();
		sz += strbir.str();
		sz += _T(" ");

		con& Contact = (*fluiter)->contact;
		sz += Contact.get_telephone();
		sz += _T(" ");
		sz += Contact.get_qq();
		sz += _T(" ");
		sz += Contact.get_wechat();
		sz += _T(" ");
		sz += Contact.get_email();

		ListBox_AddString(hWnd, sz.data());
	}
}

VOID DelInfo(HWND hDlg)//删除信息记录
{
	HWND hWnd = GetDlgItem(hDlg, IDC_LIST1);
	int pos = ListBox_GetCurSel(hWnd);

	list<CInformation*>::iterator titer = bukket.begin();
	for (int i = 0; i < pos; i++) {
		titer++;
	}
	delete (*titer);
}

VOID InfoDetailDlgInit(HWND hDlg)
{
	HWND hList = GetDlgItem(GetParent(hDlg), IDC_LIST1);
	int pos = ListBox_GetCurSel(hList);
	TCHAR szOption[MAX_PATH];
	ListBox_GetText(hList, pos, szOption);
	wstring szName = szOption;
	wstringstream stringName(szName);
	szName.empty();
	stringName >> szName;

	ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("男"));
	ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("女"));


	CInformation * p = NULL;
	list<CInformation*>::iterator iter = bukket.begin();
	for (; iter != bukket.end(); iter++) {
		if ((*iter)->get_name() == szName) {
			p = *iter;
			break;
		}
	}
	if (p == NULL) {
		MessageBox(hDlg, _T("未找到！"), _T("错误"), MB_ICONERROR);
		EndDialog(hDlg, 1);
		return;
	}

	SetDlgItemText(hDlg, IDC_EDIT2, p->get_name().data());//姓名

	if (p->get_gender()) {
		ComboBox_SetCurSel(GetDlgItem(hDlg, IDC_COMBO2), 0);
	}
	else {
		ComboBox_SetCurSel(GetDlgItem(hDlg, IDC_COMBO2), 1);
	}

	//wstring sz;
	wstringstream sz;
	sz << p->birthday.get_year() << _T('-');
	sz << p->birthday.get_month() << _T('-');
	sz << p->birthday.get_day();
	SetDlgItemText(hDlg, IDC_EDIT3, sz.str().data());//生日

	SetDlgItemText(hDlg, IDC_EDIT4, p->contact.get_telephone().data());//电话号码

	SetDlgItemText(hDlg, IDC_EDIT5, p->contact.get_qq().data());//QQ

	SetDlgItemText(hDlg, IDC_EDIT6, p->contact.get_wechat().data());//微信

	SetDlgItemText(hDlg, IDC_EDIT7, p->contact.get_email().data());//Email

}

VOID ModifyInfo(HWND hDlg)
{
	int pos = ListBox_GetCurSel(GetDlgItem(GetParent(hDlg), IDC_LIST1));
	list<CInformation*>::iterator titer = bukket.begin();
	for (int i = 0; i < pos; i++) {
		titer++;
	}
	CInformation* p = *titer;

	wstring str;
	TCHAR sz[MAX_PATH];
	GetDlgItemText(hDlg, IDC_EDIT2, sz, _countof(sz));//姓名
	str = sz;
	p->set_name(str);

	GetDlgItemText(hDlg, IDC_COMBO2, sz, _countof(sz));//性别
	str = sz;
	if (str == _T("男")) {
		p->set_gender(1);
	}
	else {
		p->set_gender(0);
	}

	GetDlgItemText(hDlg, IDC_EDIT3, sz, _countof(sz));//生日
	str = sz;
	for (int i = 0; i < (int)str.length(); i++) {
		if (str[i] == '-') {//'-'作为日期分隔符
			str[i] = ' ';
		}
	}
	wistringstream birthdaystr(str);
	int year, month, day;
	birthdaystr >> year >> month >> day;
	p->birthday.set_year(year);
	p->birthday.set_month(month);
	p->birthday.set_day(day);

	GetDlgItemText(hDlg, IDC_EDIT4, sz, _countof(sz));//电话号码
	str = sz;
	p->contact.set_telephone(str);

	GetDlgItemText(hDlg, IDC_EDIT5, sz, _countof(sz));//QQ
	str = sz;
	p->contact.set_qq(str);

	GetDlgItemText(hDlg, IDC_EDIT6, sz, _countof(sz));//微信
	str = sz;
	p->contact.set_wechat(str);

	GetDlgItemText(hDlg, IDC_EDIT7, sz, _countof(sz));//Email
	str = sz;
	p->contact.set_email(str);


}

INT_PTR CALLBACK InfoDetailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//查看信息详情对话框
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		InfoDetailDlgInit(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK://修改
			ModifyInfo(hDlg);
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK MainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//主对话框
{
	UNREFERENCED_PARAMETER(lParam);
	static CMainDlg mainDlg(hInst, hDlg);

	switch (message)
	{
	case WM_INITDIALOG:
		mainDlg.OnInit();
		break;

	case WM_COMMAND:
		OnMainDlgCommand(mainDlg, wParam, lParam);
		break;
	}
	return (INT_PTR)FALSE;
}

VOID AddInfo(HWND hDlg)
{
	CInformation* p = new CInformation;

	wstring str;
	TCHAR sz[MAX_PATH];
	GetDlgItemText(hDlg, IDC_EDIT2, sz, _countof(sz));//姓名
	str = sz;
	p->set_name(str);

	GetDlgItemText(hDlg, IDC_COMBO2, sz, _countof(sz));//性别
	str = sz;
	if (str == _T("男")) {
		p->set_gender(1);
	}
	else {
		p->set_gender(0);
	}

	GetDlgItemText(hDlg, IDC_EDIT3, sz, _countof(sz));//生日
	str = sz;
	for (int i = 0; i < (int)str.length(); i++) {
		if (str[i] == '-') {//'-'作为日期分隔符
			str[i] = ' ';
		}
	}
	wistringstream birthdaystr(str);
	int year, month, day;
	birthdaystr >> year >> month >> day;
	p->birthday.set_year(year);
	p->birthday.set_month(month);
	p->birthday.set_day(day);

	GetDlgItemText(hDlg, IDC_EDIT4, sz, _countof(sz));//电话号码
	str = sz;
	p->contact.set_telephone(str);

	GetDlgItemText(hDlg, IDC_EDIT5, sz, _countof(sz));//QQ
	str = sz;
	p->contact.set_qq(str);

	GetDlgItemText(hDlg, IDC_EDIT6, sz, _countof(sz));//微信
	str = sz;
	p->contact.set_wechat(str);

	GetDlgItemText(hDlg, IDC_EDIT7, sz, _countof(sz));//Email
	str = sz;
	p->contact.set_email(str);
}

INT_PTR CALLBACK InfoDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//信息对话框
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("男"));
		ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("女"));
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			AddInfo(hDlg);
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



//窗口类封装
CMainDlg::CMainDlg(HINSTANCE hInstance, HWND hDlg) :theBase(hDlg)
{
	this->hInst = hInstance;
	this->hDlg = hDlg;
}

VOID CMainDlg::OnInit()
{
	HICON hICON = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAIL_LIST));
	SendMessage(this->hDlg, WM_SETICON, 0, (LPARAM)hICON);

	//初始化默认密码
	ZeroMemory(PassWord, sizeof(PassWord));
	for (int i = 0; i < PWLEN - 1; i++) {
		PassWord[i] = _T('');
		PassWord[i] ^= PassWordKey;//加解密(留一位作终止符）
	}
	

	FlushInfoList(*this);
}

CMainDlg::~CMainDlg()
{
}

CInformation* FindByRank(int rank)
{
	list<CInformation*>::iterator ite = bukket.begin();
	for (; ite != bukket.end(); ite++) {
		if ((*ite)->rank == rank) {
			return *ite;
		}
	}
	return NULL;
}

// 打开文件
void CMainDlg::OnOpen()
{
	theBase.OpenFile();
	//读文件操作
	//读出密码
	bOpenPass = TRUE;
	theBase.rwFile.read((char*)PassWord, sizeof(PassWord));
	if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGPW), this->getDlgHandle(), PassWordDlg)==FALSE) {//密码错误
		MessageBox(hDlg, _T("密码错误！\n无法打开文件！"), _T("Mail_List"), MB_ICONERROR);
		return;
	}
	int tempCount = 0;//必须使用临时变量并使用前置零，否则会因对象创建而使循环中计数器发生改变

	//读Information
	theBase.rwFile.read((char*)&tempCount, sizeof(UINT));
	for (int i = 0; i < tempCount; i++) {
		CInformation* pInfo = new CInformation;
		pInfo->readFile(); //无法进行整个对象读写，否则虚函数表被修改
	}

	//读Relation
	tempCount = 0;//重新置零
	char* pTemp = NULL;
	//读出所有关系类型
	//读出关系总数
	theBase.rwFile.read((char*)&tempCount, sizeof(UINT));
	UINT relSize = tempCount;//保存关系总数

	wstring strrela;
	UINT strsize = 0;
	for (int i=0; i<tempCount; i++) {//读出每一个关系
		strsize = 0;
		theBase.rwFile.read((char*)&strsize, sizeof(UINT));
		pTemp = new char[strsize];
		theBase.rwFile.read((char*)pTemp, strsize);
		strrela = (wchar_t*)pTemp;
		delete[]pTemp;
		add_rel(strrela);
	}

	//读出每个人关系类型
	list<CInformation*>::iterator InfoIter = bukket.begin();
	strsize = 0;
	
	for (; InfoIter != bukket.end(); InfoIter++) {
		theBase.rwFile.read((char*)&(*InfoIter)->rank, sizeof(int));//读出此人rank

		for (int i = 0; i < (int)relSize; i++) {//对每一个关系，读出关系名与其他人rank
			//读出关系名
			theBase.rwFile.read((char*)&strsize, sizeof(UINT));
			pTemp = new char[strsize];
			theBase.rwFile.read((char*)pTemp, strsize);
			strrela = (wchar_t*)pTemp;
			delete[]pTemp;

			//读出关系长度
			tempCount = 0;
			theBase.rwFile.read((char*)&tempCount, sizeof(UINT));

			//读出关系内容
			int rank;
			for (int i = 0; i < tempCount; i++) {
				theBase.rwFile.read((char*)&rank, sizeof(rank));
				set_rel_to_true(*InfoIter, FindByRank(rank), strrela);
			}
		}
	}

	theBase.CloseFile();
	FlushInfoList(*this);//更新窗口
}


// 保存文件
void CMainDlg::OnSave()
{
	theBase.SaveFile();
	//写文件操作
	//写入密码
	theBase.rwFile.write((char*)PassWord, sizeof(PassWord));
	//写入Information
	theBase.rwFile.write((char*)&CInformation::m_CInformationCount, sizeof(UINT));
	list<CInformation*>::iterator InfoIter = bukket.begin();
	int i = 0;
	for (; InfoIter != bukket.end(); InfoIter++,i++) {
		(*InfoIter)->rank = i;
		(*InfoIter)->writeFile();
	}
	//写入Relation
	//写入所有关系类型
	list<wstring>::iterator reite = relation.begin();
	//写入关系总数
	UINT strsize = relation.size();
	theBase.rwFile.write((char*)&strsize, sizeof(UINT));

	for (; reite != relation.end(); reite++) {//写入关系
		strsize = ((*reite).length() + 1)*sizeof(wchar_t);
		theBase.rwFile.write((char*)&strsize, sizeof(UINT));
		theBase.rwFile.write((char*)(*reite).data(), strsize);
	}

	//写入每个人关系类型
	InfoIter = bukket.begin();
	for (; InfoIter != bukket.end(); InfoIter++) {
		theBase.rwFile.write((char*)&(*InfoIter)->rank, sizeof(int));//写入此人rank

		list<wstring>::iterator relaite = relation.begin();
		for (; relaite != relation.end(); relaite++) {//对每一个关系，写入关系名与其他人rank
			//写入关系名
			UINT strsize = ((*relaite).length() + 1)*sizeof(wchar_t);
			theBase.rwFile.write((char*)&strsize, sizeof(UINT));
			theBase.rwFile.write((char*)(*relaite).data(), strsize);;

			vector<CInformation*>*pPerrela = find_all_rel(*InfoIter, *relaite);
			vector<CInformation*>::iterator per_rela = pPerrela->begin();
			//写入关系长度
			UINT relSize = pPerrela->size();
			theBase.rwFile.write((char*)&relSize, sizeof(UINT));
			
			for (; per_rela != pPerrela->end(); per_rela++) {
				//写入关系
				theBase.rwFile.write((char*)&(*per_rela)->rank,sizeof(int));
			}
		}
		
	}


	theBase.CloseFile();
	MessageBox(hDlg, _T("完成！"), _T("Mail_List"), MB_ICONINFORMATION);
}


// 开机启动
void CMainDlg::OnRunbyWindows()
{
	bool bStaus;
	HMENU hMenu = GetMenu(hDlg);

	if (GetMenuState(hMenu, IDM_RUNBYWIN, MF_BYCOMMAND) & MF_CHECKED) {//关闭开机启动
		bStaus = false;
		CheckMenuItem(hMenu, IDM_RUNBYWIN, MF_UNCHECKED);
	}
	else {
		bStaus = true;
		CheckMenuItem(hMenu, IDM_RUNBYWIN, MF_CHECKED);
	}
	RunByWindow(bStaus);

}


// 关于对话框
void CMainDlg::OnAbout()
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
}


VOID SetOrReadDataFromReg(BOOL bSet,CHAR szDomain[],CHAR szPostAddr[],CHAR szPostPW[],CHAR szTitle[],CHAR szContent[])//邮件设置
{
	TCHAR g_sz[] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Mail_List");
	HKEY hKey;
	DWORD dwSize = MAX_PATH*sizeof(CHAR);
	if (!bSet) {
		RegCreateKeyEx(HKEY_CURRENT_USER, g_sz, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, 0, &hKey, NULL);

		RegSetValueEx(hKey, _T("邮箱服务器"), NULL, REG_BINARY, (LPBYTE)szDomain, dwSize);
		RegSetValueEx(hKey, _T("发送方邮箱地址"), NULL, REG_BINARY, (LPBYTE)szPostAddr, dwSize);
		RegSetValueEx(hKey, _T("发送方邮箱密码"), NULL, REG_BINARY, (LPBYTE)szPostPW, dwSize);
		RegSetValueEx(hKey, _T("邮件标题"), NULL, REG_BINARY, (LPBYTE)szTitle, dwSize);
		RegSetValueEx(hKey, _T("邮件正文"), NULL, REG_BINARY, (LPBYTE)szContent, dwSize);
	}
	else {
		RegOpenKeyEx(HKEY_CURRENT_USER, g_sz, NULL, KEY_QUERY_VALUE, &hKey);
		DWORD dwType;
		RegQueryValueEx(hKey, _T("邮箱服务器"), NULL, &dwType, (LPBYTE)szDomain, &dwSize);
		
		RegQueryValueEx(hKey, _T("发送方邮箱地址"), NULL, &dwType, (LPBYTE)szPostAddr, &dwSize);
	
		RegQueryValueEx(hKey, _T("发送方邮箱密码"), NULL, &dwType, (LPBYTE)szPostPW, &dwSize);
		
		RegQueryValueEx(hKey, _T("邮件标题"), NULL, &dwType, (LPBYTE)szTitle, &dwSize);

		RegQueryValueEx(hKey, _T("邮件正文"), NULL, &dwType, (LPBYTE)szContent, &dwSize);
	}
	RegCloseKey(hKey);
}