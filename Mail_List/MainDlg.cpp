#include "stdafx.h"
#include "MainDlg.h"
//��չȫ�ֱ���������
extern HINSTANCE hInst;
extern list<wstring> relation;//ȫ�ֹ�ϵ����
extern list<wstring>::iterator ite;
extern list<CInformation*> bukket;//ȫ�ֶ�������
extern list<CInformation*>::iterator spec;
extern vector<vector<bool>*>::iterator brush;
//�������
#define PWLEN 30//���볤��
TCHAR PassWord[PWLEN];
TCHAR PassWordKey = _T('L');//��Կ
BOOL bOpenPass = TRUE;//���״̬��trueΪ���ļ��������룬FALSEΪ�����ļ���ȡ����


//���ں���
VOID OnMainDlgCommand(CMainDlg& mainDlg, WPARAM wParam, LPARAM lParam)//���Ի���
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
		mainDlg.OnRunbyWindows(/*����*/);//�˴���Ҫ����
		break;
	case IDM_ABOUT: //���ڶԻ���
		mainDlg.OnAbout();
		break;
	case IDM_ADDINFO://��Ӹ�����Ϣ
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGINFO), mainDlg.getDlgHandle(), InfoDlg);
		FlushInfoList(mainDlg);//����
		break;
	case IDM_DELINFO://ɾ��������Ϣ
		if (bukket.size() == 0) {
			MessageBox(mainDlg.getDlgHandle(), _T("δ����κμ�¼���޷�ʹ�ñ����ܣ�"), _T("������ʾ"), MB_ICONERROR);
			break;
		}
		DelInfo(mainDlg.getDlgHandle());
		FlushInfoList(mainDlg);//����
		break;
	case IDM_INFODET://�鿴��Ϣ����
		if (bukket.size() == 0) {
			MessageBox(mainDlg.getDlgHandle(), _T("δ����κμ�¼���޷�ʹ�ñ����ܣ�"), _T("������ʾ"), MB_ICONERROR);
			break;
		}
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGINFO), mainDlg.getDlgHandle(), InfoDetailDlg);
		FlushInfoList(mainDlg);
		break;
	case IDM_HOBDET://�鿴����
		if (bukket.size() == 0) {
			MessageBox(mainDlg.getDlgHandle(), _T("δ����κμ�¼���޷�ʹ�ñ����ܣ�"), _T("������ʾ"), MB_ICONERROR);
			break;
		}
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGHOB), mainDlg.getDlgHandle(), HobbyDetailDlg);
		break;
	case IDM_RELAVIEW://��ϵ��ͼ
		if (bukket.size() == 0) {
			MessageBox(mainDlg.getDlgHandle(), _T("δ����κμ�¼���޷�ʹ�ñ����ܣ�"), _T("������ʾ"), MB_ICONERROR);
			break;
		}
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGRELATION), mainDlg.getDlgHandle(), RelationViewDlg);
		break;
	case IDM_SEARCH://����
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGSEARCH), mainDlg.getDlgHandle(), SearchDlg);
		break;
	case IDM_PARTY://���ɾۻ�����
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGPARTY), mainDlg.getDlgHandle(), PartyDlg);
		break;
	case IDM_PASS://��������
		bOpenPass = FALSE;
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGPW), mainDlg.getDlgHandle(), PassWordDlg);
		break;
	}
}

INT_PTR CALLBACK PassWordDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//����Ի���
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

			if (bOpenPass) {//���ļ�
				GetDlgItemText(hDlg, IDC_EDIT1, TempPassWord, _countof(TempPassWord));
				for (int i = 0; i < PWLEN-1; i++) {
					TempPassWord[i] ^= PassWordKey;//�ӽ���(��һλ����ֹ����
				}
				int a = _tcscmp(TempPassWord, PassWord);
				if (_tcscmp(TempPassWord, PassWord)==0) {//������ȷ
					EndDialog(hDlg, TRUE);
					return TRUE;
				}
				else {
					EndDialog(hDlg, FALSE);
					return FALSE;
				}
			}
			else {//�����ļ�
				ZeroMemory(PassWord, sizeof(TCHAR)*PWLEN);
				GetDlgItemText(hDlg, IDC_EDIT1, PassWord, _countof(PassWord));
				for (int i = 0; i < PWLEN-1; i++) {
					PassWord[i] ^= PassWordKey;//�ӽ���(��һλ����ֹ����
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
	HWND hList = GetDlgItem(GetParent(hDlg), IDC_LIST1);//������
	HWND hList1 = GetDlgItem(hDlg, IDC_LIST1);//������
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
		MessageBox(hDlg, _T("δ�ҵ���"), _T("��ʾ"), MB_ICONERROR);
		EndDialog(hDlg, 1);
		return;
	}

	vector<vector<CInformation*>>*pParty = have_party(p);
	if (pParty->size() == 0){
		MessageBox(hDlg, _T("δ�ҵ���"), _T("��ʾ"), MB_ICONERROR);
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

INT_PTR CALLBACK PartyDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//�ۻ�Ի���
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
	ComboBox_AddString(hCombo, _T("����"));
	ComboBox_AddString(hCombo, _T("�����·�"));
	ComboBox_AddString(hCombo, _T("���N������"));
	ComboBox_AddString(hCombo, _T("����"));
	ComboBox_AddString(hCombo, _T("�绰����"));
	ComboBox_AddString(hCombo, _T("QQ"));
	ComboBox_AddString(hCombo, _T("WeChat"));
	ComboBox_AddString(hCombo, _T("Email"));
	ComboBox_AddString(hCombo, _T("�������"));
	ComboBox_AddString(hCombo, _T("�Ա�"));
}

VOID SearchInfo(HWND hDlg)
{
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	HWND hEdit = GetDlgItem(hDlg, IDC_EDIT1);
	HWND hList = GetDlgItem(hDlg, IDC_LIST1);
	ListBox_ResetContent(hList);
	int cursel = ComboBox_GetCurSel(hCombo);

	//��ȡ����
	TCHAR sz[MAX_PATH];
	GetWindowText(hEdit, sz, _countof(sz));
	wstring str = sz;
	int num = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, FALSE);
	int gender = -1;
	if (str == _T("��")) {
		gender = 1;
	}
	else if (str == _T("Ů")) {
		gender = 0;
	}
	else
	{
		gender = -1;//��Ч
	}

	list<wstring>namelist;

	list<CInformation*>::iterator infoite = bukket.begin();
	for (; infoite != bukket.end(); infoite++) {
		switch (cursel)
		{
		case 0://����
			if ((*infoite)->get_name() == str) {
				namelist.push_back(str);
			}
			break;
		case 1://�����·�
			if (num == (*infoite)->birthday.get_month()) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 2://���N������
			if ((*infoite)->birthday.DateInRange(num)) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 3://����
			if ((*infoite)->hobby.Chobby_find(str)) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 4://�绰����
			if ((*infoite)->contact.get_telephone() == str) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 5://QQ
			if ((*infoite)->contact.get_qq() == str) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 6://΢��
			if ((*infoite)->contact.get_wechat() == str) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 7://�ʼ�
			if ((*infoite)->contact.get_email() == str) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 8://�������
			if ((*infoite)->birthday.get_year() == num) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		case 9://�Ա�
			if ((int)(*infoite)->get_gender() == gender) {
				namelist.push_back((*infoite)->get_name());
			}
			break;
		}
	}

	//ȥ��
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
	HWND hList = GetDlgItem(GetParent(hDlg), IDC_LIST1);//������

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
		MessageBox(hDlg, _T("δ�ҵ���"), _T("����"), MB_ICONERROR);
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
	
	SetOrReadDataFromReg(FALSE, szDomain, szPostAddr, szPostPW, szTitle, szContent);//��������

	CSendMail sm(szDomain, szPostAddr, szPostPW, szToAddr, szTitle, szContent);
	sm.SendMail();
	MessageBox(hDlg, _T("�������"), _T("�����ʼ�"), MB_ICONINFORMATION);
}

INT_PTR CALLBACK MailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//�ʼ��Ի���
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


INT_PTR CALLBACK SearchDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//���ҶԻ���
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
		case IDOK://����
			SearchInfo(hDlg);
			break;
		case IDM_INFODET://��ϸ��Ϣ
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGINFO), hDlg, InfoDetailDlg);
			break;
		case IDC_SENDMAIL://�����ʼ�
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGMAIL), hDlg, MailDlg);
			break;
		case IDCANCEL:
			SendMessage(GetParent(hDlg), WM_INITDIALOG, 0, 0);//�˳�ǰˢ��������
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


VOID FlushRelationView(HWND hDlg)
{
	HWND hParentList = GetDlgItem(GetParent(hDlg), IDC_LIST1);//���Ի���List
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);//���Ի���Combo
	HWND hListI = GetDlgItem(hDlg, IDC_LIST1);//��ϵ�ڲ������ڹ�ϵ��
	HWND hListE = GetDlgItem(hDlg, IDC_LIST2);//��ϵ�ⲿ�������ڹ�ϵ��

	ListBox_ResetContent(hListI);
	ListBox_ResetContent(hListE);

	//�����������ǰ
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

	//�޸Ĵ��ڱ���
	wstring caption = _T("��ϵ��ͼ��");
	caption += pInfo->get_name();
	SetWindowText(hDlg, caption.data());

	list<wstring>InsideName;//��ϵ��
	list<wstring>OutsideName;//��ϵ��


	list<wstring>::iterator relation_iter = relation.begin();
	for (; relation_iter != relation.end(); relation_iter++) {
		//ComboBox
		ComboBox_AddString(hCombo, (*relation_iter).data());


		if ((*relation_iter) != strrel) {//������ѡ��ϵ��������һ��
			continue;
		}

		InfoIter = bukket.begin();
		for (; InfoIter != bukket.end(); InfoIter++) {
			if (*InfoIter == pInfo) {//��������
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
	InsideName.sort();//�����򣬷����޷�ȥ��
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

	ComboBox_SetCurSel(hCombo, currentsel);//�����ϴ�ѡ��
}

VOID AddRelationView(HWND hDlg)//��ӹ�ϵ
{
	TCHAR sz[MAX_PATH];
	GetDlgItemText(hDlg, IDC_EDIT1, sz, _countof(sz));
	wstring str = sz;
	add_rel(str);
	SetDlgItemText(hDlg, IDC_EDIT1, _T(""));//���
}

VOID DelRelationView(HWND hDlg)//ɾ����ϵ
{
	TCHAR sz[MAX_PATH];
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);//���Ի���Combo
	ComboBox_GetText(hCombo, sz, _countof(sz));
	wstring str = sz;
	del_rel(str);


}

VOID BindRelation(HWND hDlg)//��
{
	HWND hParentList = GetDlgItem(GetParent(hDlg), IDC_LIST1);//���Ի���List
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);//���Ի���Combo
	HWND hListE = GetDlgItem(hDlg, IDC_LIST2);//��ϵ�ⲿ�������ڹ�ϵ��

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

VOID UntieRelation(HWND hDlg)//�ɰ�
{
	HWND hParentList = GetDlgItem(GetParent(hDlg), IDC_LIST1);//���Ի���List
	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);//���Ի���Combo
	HWND hListI = GetDlgItem(hDlg, IDC_LIST1);//��ϵ�ڲ������ڹ�ϵ��

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

INT_PTR CALLBACK RelationViewDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//��ϵ�Ի���
{
	switch (message)
	{
	case WM_INITDIALOG:
		FlushRelationView(hDlg);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (HIWORD(wParam) == CBN_SELCHANGE && GetDlgItem(hDlg, IDC_COMBO1) == (HWND)lParam) {//ѡ���ϵ
			//�������ֶ�����ѡ������޷����ѡ��
			HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
			int cursel = ComboBox_GetCurSel(hCombo);
			ComboBox_SetCurSel(hCombo, cursel);
			UpdateWindow(hCombo);
			FlushRelationView(hDlg);
		}
		switch (LOWORD(wParam))
		{
		case IDC_ADDRELA://��ӹ�ϵ
			AddRelationView(hDlg);
			FlushRelationView(hDlg);
			break;
		case IDC_DELRELA://ɾ����ϵ
			DelRelationView(hDlg);
			FlushRelationView(hDlg);
			break;
		case IDC_BIND://����
			BindRelation(hDlg);
			FlushRelationView(hDlg);
			break;
		case IDC_UNTIE://�ɰ�
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
	HWND hWnd = GetDlgItem(GetParent(hDlg), IDC_LIST1);//������List
	HWND hList = GetDlgItem(hDlg, IDC_LIST1);//������List

	//�޸ı���
	TCHAR sz[MAX_PATH];
	ListBox_GetText(hWnd, ListBox_GetCurSel(hWnd), sz);
	wstring str = _T("���ã�");
	str += sz;
	for (int i = 0; i < (int)str.length(); i++) {
		if (str[i] == ' ') {
			str[i] = '\0';
			break;
		}
	}
	SetWindowText(hDlg, str.data());

	ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("��"));
	ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("Ů"));
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
	HWND hWnd = GetDlgItem(GetParent(hDlg), IDC_LIST1);//������List
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

	hobby.Chobby_del_repeat();//ȥ��
	SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
}

VOID DelHobby(HWND hDlg)
{
	HWND hWnd = GetDlgItem(GetParent(hDlg), IDC_LIST1);//������List
	HWND hList = GetDlgItem(hDlg, IDC_LIST1);//������List
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

INT_PTR CALLBACK HobbyDetailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//���öԻ���
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
			FlushHobbyList(hDlg);//ˢ��
			break;
		case IDC_DELHOB:
			DelHobby(hDlg);
			FlushHobbyList(hDlg);//ˢ��
			break;
		case IDC_MODHOB:
			ModifyHobby(hDlg);
			FlushHobbyList(hDlg);//ˢ��
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

VOID FlushInfoList(CMainDlg& mainDlg)//���ڸ��¸�����Ϣ�б�
{
	HWND hWnd = GetDlgItem(mainDlg.getDlgHandle(), IDC_LIST1);
	ListBox_ResetContent(hWnd);

	list<CInformation*>::iterator fluiter = bukket.begin();
	for (; fluiter != bukket.end(); fluiter++) {
		wstring sz;
		sz += (*fluiter)->get_name();
		sz += _T(" ");

		if ((*fluiter)->get_gender()) {
			sz += _T("��");
		}
		else {
			sz += _T("Ů");
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

VOID DelInfo(HWND hDlg)//ɾ����Ϣ��¼
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

	ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("��"));
	ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("Ů"));


	CInformation * p = NULL;
	list<CInformation*>::iterator iter = bukket.begin();
	for (; iter != bukket.end(); iter++) {
		if ((*iter)->get_name() == szName) {
			p = *iter;
			break;
		}
	}
	if (p == NULL) {
		MessageBox(hDlg, _T("δ�ҵ���"), _T("����"), MB_ICONERROR);
		EndDialog(hDlg, 1);
		return;
	}

	SetDlgItemText(hDlg, IDC_EDIT2, p->get_name().data());//����

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
	SetDlgItemText(hDlg, IDC_EDIT3, sz.str().data());//����

	SetDlgItemText(hDlg, IDC_EDIT4, p->contact.get_telephone().data());//�绰����

	SetDlgItemText(hDlg, IDC_EDIT5, p->contact.get_qq().data());//QQ

	SetDlgItemText(hDlg, IDC_EDIT6, p->contact.get_wechat().data());//΢��

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
	GetDlgItemText(hDlg, IDC_EDIT2, sz, _countof(sz));//����
	str = sz;
	p->set_name(str);

	GetDlgItemText(hDlg, IDC_COMBO2, sz, _countof(sz));//�Ա�
	str = sz;
	if (str == _T("��")) {
		p->set_gender(1);
	}
	else {
		p->set_gender(0);
	}

	GetDlgItemText(hDlg, IDC_EDIT3, sz, _countof(sz));//����
	str = sz;
	for (int i = 0; i < (int)str.length(); i++) {
		if (str[i] == '-') {//'-'��Ϊ���ڷָ���
			str[i] = ' ';
		}
	}
	wistringstream birthdaystr(str);
	int year, month, day;
	birthdaystr >> year >> month >> day;
	p->birthday.set_year(year);
	p->birthday.set_month(month);
	p->birthday.set_day(day);

	GetDlgItemText(hDlg, IDC_EDIT4, sz, _countof(sz));//�绰����
	str = sz;
	p->contact.set_telephone(str);

	GetDlgItemText(hDlg, IDC_EDIT5, sz, _countof(sz));//QQ
	str = sz;
	p->contact.set_qq(str);

	GetDlgItemText(hDlg, IDC_EDIT6, sz, _countof(sz));//΢��
	str = sz;
	p->contact.set_wechat(str);

	GetDlgItemText(hDlg, IDC_EDIT7, sz, _countof(sz));//Email
	str = sz;
	p->contact.set_email(str);


}

INT_PTR CALLBACK InfoDetailDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//�鿴��Ϣ����Ի���
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
		case IDOK://�޸�
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

INT_PTR CALLBACK MainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//���Ի���
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
	GetDlgItemText(hDlg, IDC_EDIT2, sz, _countof(sz));//����
	str = sz;
	p->set_name(str);

	GetDlgItemText(hDlg, IDC_COMBO2, sz, _countof(sz));//�Ա�
	str = sz;
	if (str == _T("��")) {
		p->set_gender(1);
	}
	else {
		p->set_gender(0);
	}

	GetDlgItemText(hDlg, IDC_EDIT3, sz, _countof(sz));//����
	str = sz;
	for (int i = 0; i < (int)str.length(); i++) {
		if (str[i] == '-') {//'-'��Ϊ���ڷָ���
			str[i] = ' ';
		}
	}
	wistringstream birthdaystr(str);
	int year, month, day;
	birthdaystr >> year >> month >> day;
	p->birthday.set_year(year);
	p->birthday.set_month(month);
	p->birthday.set_day(day);

	GetDlgItemText(hDlg, IDC_EDIT4, sz, _countof(sz));//�绰����
	str = sz;
	p->contact.set_telephone(str);

	GetDlgItemText(hDlg, IDC_EDIT5, sz, _countof(sz));//QQ
	str = sz;
	p->contact.set_qq(str);

	GetDlgItemText(hDlg, IDC_EDIT6, sz, _countof(sz));//΢��
	str = sz;
	p->contact.set_wechat(str);

	GetDlgItemText(hDlg, IDC_EDIT7, sz, _countof(sz));//Email
	str = sz;
	p->contact.set_email(str);
}

INT_PTR CALLBACK InfoDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//��Ϣ�Ի���
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("��"));
		ComboBox_AddString(GetDlgItem(hDlg, IDC_COMBO2), _T("Ů"));
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



// �����ڡ������Ϣ�������
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



//�������װ
CMainDlg::CMainDlg(HINSTANCE hInstance, HWND hDlg) :theBase(hDlg)
{
	this->hInst = hInstance;
	this->hDlg = hDlg;
}

VOID CMainDlg::OnInit()
{
	HICON hICON = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAIL_LIST));
	SendMessage(this->hDlg, WM_SETICON, 0, (LPARAM)hICON);

	//��ʼ��Ĭ������
	ZeroMemory(PassWord, sizeof(PassWord));
	for (int i = 0; i < PWLEN - 1; i++) {
		PassWord[i] = _T('');
		PassWord[i] ^= PassWordKey;//�ӽ���(��һλ����ֹ����
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

// ���ļ�
void CMainDlg::OnOpen()
{
	theBase.OpenFile();
	//���ļ�����
	//��������
	bOpenPass = TRUE;
	theBase.rwFile.read((char*)PassWord, sizeof(PassWord));
	if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGPW), this->getDlgHandle(), PassWordDlg)==FALSE) {//�������
		MessageBox(hDlg, _T("�������\n�޷����ļ���"), _T("Mail_List"), MB_ICONERROR);
		return;
	}
	int tempCount = 0;//����ʹ����ʱ������ʹ��ǰ���㣬���������󴴽���ʹѭ���м����������ı�

	//��Information
	theBase.rwFile.read((char*)&tempCount, sizeof(UINT));
	for (int i = 0; i < tempCount; i++) {
		CInformation* pInfo = new CInformation;
		pInfo->readFile(); //�޷��������������д�������麯�����޸�
	}

	//��Relation
	tempCount = 0;//��������
	char* pTemp = NULL;
	//�������й�ϵ����
	//������ϵ����
	theBase.rwFile.read((char*)&tempCount, sizeof(UINT));
	UINT relSize = tempCount;//�����ϵ����

	wstring strrela;
	UINT strsize = 0;
	for (int i=0; i<tempCount; i++) {//����ÿһ����ϵ
		strsize = 0;
		theBase.rwFile.read((char*)&strsize, sizeof(UINT));
		pTemp = new char[strsize];
		theBase.rwFile.read((char*)pTemp, strsize);
		strrela = (wchar_t*)pTemp;
		delete[]pTemp;
		add_rel(strrela);
	}

	//����ÿ���˹�ϵ����
	list<CInformation*>::iterator InfoIter = bukket.begin();
	strsize = 0;
	
	for (; InfoIter != bukket.end(); InfoIter++) {
		theBase.rwFile.read((char*)&(*InfoIter)->rank, sizeof(int));//��������rank

		for (int i = 0; i < (int)relSize; i++) {//��ÿһ����ϵ��������ϵ����������rank
			//������ϵ��
			theBase.rwFile.read((char*)&strsize, sizeof(UINT));
			pTemp = new char[strsize];
			theBase.rwFile.read((char*)pTemp, strsize);
			strrela = (wchar_t*)pTemp;
			delete[]pTemp;

			//������ϵ����
			tempCount = 0;
			theBase.rwFile.read((char*)&tempCount, sizeof(UINT));

			//������ϵ����
			int rank;
			for (int i = 0; i < tempCount; i++) {
				theBase.rwFile.read((char*)&rank, sizeof(rank));
				set_rel_to_true(*InfoIter, FindByRank(rank), strrela);
			}
		}
	}

	theBase.CloseFile();
	FlushInfoList(*this);//���´���
}


// �����ļ�
void CMainDlg::OnSave()
{
	theBase.SaveFile();
	//д�ļ�����
	//д������
	theBase.rwFile.write((char*)PassWord, sizeof(PassWord));
	//д��Information
	theBase.rwFile.write((char*)&CInformation::m_CInformationCount, sizeof(UINT));
	list<CInformation*>::iterator InfoIter = bukket.begin();
	int i = 0;
	for (; InfoIter != bukket.end(); InfoIter++,i++) {
		(*InfoIter)->rank = i;
		(*InfoIter)->writeFile();
	}
	//д��Relation
	//д�����й�ϵ����
	list<wstring>::iterator reite = relation.begin();
	//д���ϵ����
	UINT strsize = relation.size();
	theBase.rwFile.write((char*)&strsize, sizeof(UINT));

	for (; reite != relation.end(); reite++) {//д���ϵ
		strsize = ((*reite).length() + 1)*sizeof(wchar_t);
		theBase.rwFile.write((char*)&strsize, sizeof(UINT));
		theBase.rwFile.write((char*)(*reite).data(), strsize);
	}

	//д��ÿ���˹�ϵ����
	InfoIter = bukket.begin();
	for (; InfoIter != bukket.end(); InfoIter++) {
		theBase.rwFile.write((char*)&(*InfoIter)->rank, sizeof(int));//д�����rank

		list<wstring>::iterator relaite = relation.begin();
		for (; relaite != relation.end(); relaite++) {//��ÿһ����ϵ��д���ϵ����������rank
			//д���ϵ��
			UINT strsize = ((*relaite).length() + 1)*sizeof(wchar_t);
			theBase.rwFile.write((char*)&strsize, sizeof(UINT));
			theBase.rwFile.write((char*)(*relaite).data(), strsize);;

			vector<CInformation*>*pPerrela = find_all_rel(*InfoIter, *relaite);
			vector<CInformation*>::iterator per_rela = pPerrela->begin();
			//д���ϵ����
			UINT relSize = pPerrela->size();
			theBase.rwFile.write((char*)&relSize, sizeof(UINT));
			
			for (; per_rela != pPerrela->end(); per_rela++) {
				//д���ϵ
				theBase.rwFile.write((char*)&(*per_rela)->rank,sizeof(int));
			}
		}
		
	}


	theBase.CloseFile();
	MessageBox(hDlg, _T("��ɣ�"), _T("Mail_List"), MB_ICONINFORMATION);
}


// ��������
void CMainDlg::OnRunbyWindows()
{
	bool bStaus;
	HMENU hMenu = GetMenu(hDlg);

	if (GetMenuState(hMenu, IDM_RUNBYWIN, MF_BYCOMMAND) & MF_CHECKED) {//�رտ�������
		bStaus = false;
		CheckMenuItem(hMenu, IDM_RUNBYWIN, MF_UNCHECKED);
	}
	else {
		bStaus = true;
		CheckMenuItem(hMenu, IDM_RUNBYWIN, MF_CHECKED);
	}
	RunByWindow(bStaus);

}


// ���ڶԻ���
void CMainDlg::OnAbout()
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
}


VOID SetOrReadDataFromReg(BOOL bSet,CHAR szDomain[],CHAR szPostAddr[],CHAR szPostPW[],CHAR szTitle[],CHAR szContent[])//�ʼ�����
{
	TCHAR g_sz[] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Mail_List");
	HKEY hKey;
	DWORD dwSize = MAX_PATH*sizeof(CHAR);
	if (!bSet) {
		RegCreateKeyEx(HKEY_CURRENT_USER, g_sz, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, 0, &hKey, NULL);

		RegSetValueEx(hKey, _T("���������"), NULL, REG_BINARY, (LPBYTE)szDomain, dwSize);
		RegSetValueEx(hKey, _T("���ͷ������ַ"), NULL, REG_BINARY, (LPBYTE)szPostAddr, dwSize);
		RegSetValueEx(hKey, _T("���ͷ���������"), NULL, REG_BINARY, (LPBYTE)szPostPW, dwSize);
		RegSetValueEx(hKey, _T("�ʼ�����"), NULL, REG_BINARY, (LPBYTE)szTitle, dwSize);
		RegSetValueEx(hKey, _T("�ʼ�����"), NULL, REG_BINARY, (LPBYTE)szContent, dwSize);
	}
	else {
		RegOpenKeyEx(HKEY_CURRENT_USER, g_sz, NULL, KEY_QUERY_VALUE, &hKey);
		DWORD dwType;
		RegQueryValueEx(hKey, _T("���������"), NULL, &dwType, (LPBYTE)szDomain, &dwSize);
		
		RegQueryValueEx(hKey, _T("���ͷ������ַ"), NULL, &dwType, (LPBYTE)szPostAddr, &dwSize);
	
		RegQueryValueEx(hKey, _T("���ͷ���������"), NULL, &dwType, (LPBYTE)szPostPW, &dwSize);
		
		RegQueryValueEx(hKey, _T("�ʼ�����"), NULL, &dwType, (LPBYTE)szTitle, &dwSize);

		RegQueryValueEx(hKey, _T("�ʼ�����"), NULL, &dwType, (LPBYTE)szContent, &dwSize);
	}
	RegCloseKey(hKey);
}