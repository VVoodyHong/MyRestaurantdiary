
// MyRestrauntDiaryDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MyRestrauntDiary.h"
#include "MyRestrauntDiaryDlg.h"
#include "afxdialogex.h"
#include "mysql.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyRestrauntDiaryDlg 대화 상자



CMyRestrauntDiaryDlg::CMyRestrauntDiaryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYRESTRAUNTDIARY_DIALOG, pParent)

	, m_strOrigin(_T(""))
	, m_strName(_T(""))
	, m_strMenu(_T(""))
	, m_strTel(_T(""))
	, m_strAddress(_T(""))
	, m_nRating(0)
	, m_strComment(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyRestrauntDiaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlListCtrl);
	DDX_Text(pDX, IDC_ORIGIN, m_strOrigin);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_MENU, m_strMenu);
	DDX_Text(pDX, IDC_TEL, m_strTel);
	DDX_Text(pDX, IDC_ADDRESS, m_strAddress);
	DDX_Text(pDX, IDC_RATING, m_nRating);
	DDX_Text(pDX, IDC_COMMENT, m_strComment);
	DDX_Control(pDX, IDC_SLIDER1, m_sldSlider1);
}

BEGIN_MESSAGE_MAP(CMyRestrauntDiaryDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnAdd, &CMyRestrauntDiaryDlg::OnBnClickedBtnadd)
	ON_BN_CLICKED(IDC_BtnModify, &CMyRestrauntDiaryDlg::OnBnClickedBtnmodify)
	ON_BN_CLICKED(IDC_BtnDelete, &CMyRestrauntDiaryDlg::OnBnClickedBtndelete)
	ON_BN_CLICKED(IDCANCEL, &CMyRestrauntDiaryDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CMyRestrauntDiaryDlg::OnClickList1)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CMyRestrauntDiaryDlg::OnReleasedcaptureSlider1)
END_MESSAGE_MAP()


// CMyRestrauntDiaryDlg 메시지 처리기

BOOL CMyRestrauntDiaryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//-----------------------------------------------------------------------------
	// ListCtrl Title Setting
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_FMT;

	m_ctrlListCtrl.SetExtendedStyle(m_ctrlListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CString titlearr[8] = { _T("업종"), _T("식당명"), _T("전화번호"), _T("주소"), _T("메뉴"), _T("평점"), _T("평가"), _T("입력일") };
	UINT titlewdt[8] = { 60, 105, 110, 120, 100, 50, 140, 95 };
	UINT titlefmt[8] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT };

	for (int i = 0; i < 8; i++)
	{
		lvcolumn.pszText = (LPTSTR)(LPCTSTR)titlearr[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = titlewdt[i];
		lvcolumn.fmt = titlefmt[i];

		m_ctrlListCtrl.InsertColumn(i, &lvcolumn);
	}
	//-----------------------------------------------------------------------------
	// Initiating Mysql
	CMyRestrauntDiaryDlg::setDataToList();

	// slider control 설정
	m_sldSlider1.SetRange(0, 5);
	m_sldSlider1.SetPos(0);
	m_sldSlider1.SetLineSize(1);
	
	// edit box 설정
	m_nRating = m_sldSlider1.GetPos();
	
	UpdateData(FALSE);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMyRestrauntDiaryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMyRestrauntDiaryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyRestrauntDiaryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//sql 데이터를 가져옴
int CMyRestrauntDiaryDlg::setDataToList()
{
	MYSQL mysql; // sql 인스턴스 생성
	
	// sql 시작
	if (!mysql_init(&mysql))
	{
		AfxMessageBox(_T("fail to mysql_init()"));
		return -1;
	}

	// sql 연결
	if (mysql_real_connect(&mysql, "164.125.121.194", "woody", "password", "myrestaurantsdiary", 0, NULL, 0) == FALSE)
	{
		AfxMessageBox(mysql_error(&mysql));
		return -1;
	}

	// char type 설정
	mysql_query(&mysql, "set names euckr");

	// sql에 쿼리
	mysql_query(&mysql, "select type_name, res_name,res_tel, res_address, menu, rating, comments, update_date from restaurantcontacts, memo, origin where restaurantcontacts.res_id=memo.res_id and restaurantcontacts.type_id=origin.type_id");

	// 쿼리 결과를 result 객체에 저장
	MYSQL_RES* result = mysql_store_result(&mysql);

	// 제대로된 데이터가 오지 않았을 시 경고 후 종료
	if (!result)
	{
		AfxMessageBox(mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}

	int idx = 0;				// 행
	CString valstr;				// 행의 인덱스
	MYSQL_ROW row;				// 행의 데이터를 담기 위한 인스턴스
	
	while (row = mysql_fetch_row(result)) // row에 데이터 저장
	{
		valstr.Format(_T("%d"), idx + 1); // vastr에 idx +1 입력
		m_ctrlListCtrl.InsertItem(idx, valstr); // idx번째 줄에 varstr 데이터 입력하여 행 생성 (행생성을 위함)

		for (int i = 0; i < 8; i++)
			m_ctrlListCtrl.SetItemText(idx, i, row[i]); // idx번째 줄 i 열에 row[i]의 데이터 입력
		idx++; // 행 증가
	}
	mysql_free_result(result);
	mysql_close(&mysql);
	UpdateData(FALSE);

	return idx;
}

void CMyRestrauntDiaryDlg::OnBnClickedBtnadd()
{
	UpdateData(TRUE);

	// 프라이머리키인 Origin이 입력되지 않았을 시 경고 후 리턴
	if (m_strOrigin == "")
	{
		AfxMessageBox("Enter the item to add");
		return;
	}

	// 입력한 Origin과 mysql 연동을 위해 type_id로 받음
	int type_id;
	if (m_strOrigin == "한식") type_id = 101;
	else if (m_strOrigin == "중식") type_id = 102;
	else if (m_strOrigin == "일식") type_id = 103;
	else if (m_strOrigin == "양식") type_id = 104;
	else if (m_strOrigin == "기타") type_id = 105;
	else
	{
		AfxMessageBox("insert correct Origin.");
		return;
	}

	//에디트 컨트롤에 입력된 데이터들을 배열로 만듦
	CString str;
	str.Format("%d", m_nRating);
	CString column[7] = { m_strOrigin, m_strName, m_strTel, m_strAddress, m_strMenu, str, m_strComment };

	//리스트컨트롤의 행 수를 count로 선언
	int count = m_ctrlListCtrl.GetItemCount();

	// 고유번호인 Tel과 해당 Menu를 통해 중복 체크
	for (int i = 0; i < count; i++)
	{
		CString Tel = m_ctrlListCtrl.GetItemText(i, 2);
		CString Menu = m_ctrlListCtrl.GetItemText(i, 4);

		if (m_strTel == Tel && m_strMenu == Menu)
		{
			AfxMessageBox("This item already exists");
			return;
		}
	}

	// count번째 줄에 cloumn[0] 추가하여 행 생성
	m_ctrlListCtrl.InsertItem(count, column[0]);
	//각 열에 데이터 입력
	for (int i = 1; i < 7; i++)
		m_ctrlListCtrl.SetItemText(count, i, column[i]);

	MYSQL mysql;

	if (!mysql_init(&mysql))
	{
		AfxMessageBox("fail to mysql_init()");
		return;
	}

	if (mysql_real_connect(&mysql, "164.125.121.194", "woody", "password", "myrestaurantsdiary", 0, NULL, 0) == FALSE)
	{
		AfxMessageBox(mysql_error(&mysql));
		return;
	}

	mysql_query(&mysql, "set names euckr");

	// 쿼리 입력을 위한 변수 설정
	CString  insContacts_query, insMemo_query, res_query;

	// 추가를 위해 입력한 가게 이름의 res_id를 데이터베이스에서 받아옴
	res_query.Format("select res_id from restaurantcontacts where res_name = '%s'", m_strName);
	// 쿼리 실행
	mysql_query(&mysql, res_query);

	// 쿼리 실행 결과를 res 구조체에 담음
	MYSQL_RES* res = mysql_store_result(&mysql);
	if (!res)
	{
		AfxMessageBox(mysql_error(&mysql));
		mysql_close(&mysql);
		return;
	}

	// 쿼리 결과를 MYSQL_ROW 타입에 담음
	MYSQL_ROW resRow = mysql_fetch_row(res);

	// resRow 결과가 없을 시 <데이터베이스에 데이터가 없을 시>
	if (!resRow)
	{
		// contacts 테이블에 추가
		insContacts_query.Format("insert into restaurantcontacts(res_name, res_address, res_tel, type_id) values('%s', '%s', '%s',  %d)", m_strName, m_strAddress, m_strTel, type_id);
		mysql_query(&mysql, insContacts_query);
		// 추가한 필드의 id를 받는 쿼리
		res_query.Format("select res_id from restaurantcontacts where res_name = '%s'", m_strName);
		mysql_query(&mysql, res_query);

		MYSQL_RES* resNew = mysql_store_result(&mysql);
		if (!resNew)
		{
			mysql_free_result(res);
			AfxMessageBox(mysql_error(&mysql));
			mysql_close(&mysql);
			return;
		}

		MYSQL_ROW row = mysql_fetch_row(resNew);

		// id와 함께 메모 테이블에 추가
		insMemo_query.Format("insert into memo(menu, res_id, comments, rating) values('%s', %s, '%s', '%d')", m_strMenu, row[0], m_strComment, m_nRating);
		mysql_query(&mysql, insMemo_query);
		mysql_free_result(resNew);
	}
	// resRow 결과가 존재할 시 <데이터베이스에 데이터가 잇을 시>
	else
	{
		// 이미 존재하는 필드의 id를 받아와서 메모 테이블에 추가
		insMemo_query.Format("insert into memo(menu, res_id, comments, rating) values('%s', %s, '%s', '%d')", m_strMenu, resRow[0], m_strComment, m_nRating);
		mysql_query(&mysql, insMemo_query);
	}


	// 에디트 컨트롤 초기화
	m_strOrigin = m_strName = m_strTel = m_strAddress = m_strMenu = m_strComment = "";
	m_nRating = NULL;
	mysql_free_result(res);
	mysql_close(&mysql);
	UpdateData(FALSE);
}


void CMyRestrauntDiaryDlg::OnBnClickedBtnmodify()
{
	CString Menu = m_strMenu;

	UpdateData(TRUE);

	// 수정할 항목 선택하지 않을 시 경고 후 리턴
	if (m_strOrigin == "")
	{
		AfxMessageBox("Select an item to modify");
		return;
	}

	MYSQL mysql;

	if (!mysql_init(&mysql))
	{
		AfxMessageBox("fail to mysql_init()");
		return;
	}

	if (mysql_real_connect(&mysql, "164.125.121.194", "woody", "password", "myrestaurantsdiary", 0, NULL, 0) == FALSE)
	{
		AfxMessageBox(mysql_error(&mysql));
		return;
	}

	mysql_query(&mysql, "set names euckr");

	CString  res_query, modify_query;

	res_query.Format("select m.res_id, m.memo_id from memo as m, restaurantcontacts as rs where rs.res_name = '%s' and m.menu ='%s'", m_strName, Menu);


	mysql_query(&mysql, res_query);

	MYSQL_RES* res = mysql_store_result(&mysql);
	if (!res)
	{
		AfxMessageBox(mysql_error(&mysql));
		mysql_close(&mysql);
		return;
	}

	MYSQL_ROW resRow;
	resRow = mysql_fetch_row(res);

	modify_query.Format("update restaurantcontacts rc, memo m set rc.res_tel = '%s', rc.res_address = '%s', m.menu = '%s', m.rating = %d, m.comments = '%s' where rc.res_id = %s and m.memo_id = %s", m_strTel, m_strAddress, m_strMenu, m_nRating, m_strComment, resRow[0], resRow[1]);
	mysql_query(&mysql, modify_query);

	m_strOrigin = m_strName = m_strTel = m_strAddress = m_strMenu = m_strComment = "";
	m_nRating = NULL;

	mysql_free_result(res);
	mysql_close(&mysql);
	UpdateData(FALSE);

	m_ctrlListCtrl.DeleteAllItems();
	setDataToList();

}

void CMyRestrauntDiaryDlg::OnBnClickedBtndelete()
{
	UpdateData(TRUE);

	if (m_strOrigin == "")
	{
		AfxMessageBox("Select an item to delete");
		return;
	}

	MYSQL mysql;

	if (!mysql_init(&mysql))
	{
		AfxMessageBox("fail to mysql_init()");
		return;
	}

	if (mysql_real_connect(&mysql, "164.125.121.170", "woody", "password", "myrestaurantsdiary", 0, NULL, 0) == FALSE)
	{
		AfxMessageBox(mysql_error(&mysql));
		return;
	}

	mysql_query(&mysql, "set names euckr");

	CString  delContacts_query, delMemo_query, chkMemo_query, res_id_query;

	res_id_query.Format("select res_id from restaurantcontacts where res_name = '%s'", m_strName);
	
	mysql_query(&mysql, res_id_query);

	MYSQL_RES* res_id = mysql_store_result(&mysql);
	if (!res_id)
	{
		AfxMessageBox(mysql_error(&mysql));
		mysql_close(&mysql);
		return;
	}

	MYSQL_ROW row;
	row = mysql_fetch_row(res_id);

	delMemo_query.Format("delete from memo where res_id = %s and Menu = '%s'", row[0], m_strMenu);

	mysql_query(&mysql, delMemo_query);

	int chk = 0;
	int count = m_ctrlListCtrl.GetItemCount();

	for (int i = 0; i < count; i++)
	{
		CString res_name = m_ctrlListCtrl.GetItemText(i, 1);
		if (m_strName == res_name)
		{
			chk++;
		}
	}
	if (chk == 1)
	{
		delContacts_query.Format("delete from restaurantcontacts where res_id = %s", row[0]);
		mysql_query(&mysql, delContacts_query);
	}

	if (count == 1 && chk == 1)
	{
		CString reset_id_query1, reset_id_query2;
		reset_id_query1.Format("alter table memo auto_increment = 1;");
		reset_id_query2.Format("alter table restaurantcontacts auto_increment = 1;");
		mysql_query(&mysql, reset_id_query1);
		mysql_query(&mysql, reset_id_query2);
	}
	

	m_strOrigin = m_strName = m_strTel = m_strAddress = m_strMenu = m_strComment = "";
	m_nRating = NULL;
	mysql_free_result(res_id);
	mysql_close(&mysql);

	POSITION pos;
	pos = m_ctrlListCtrl.GetFirstSelectedItemPosition();
	int cur = m_ctrlListCtrl.GetNextSelectedItem(pos);
	m_ctrlListCtrl.DeleteItem(cur);

	UpdateData(FALSE);
}

void CMyRestrauntDiaryDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

// 선택한 항목을 에디트컨트롤에 받아옴
void CMyRestrauntDiaryDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMView = (NM_LISTVIEW*)pNMHDR;

	int nSel = pNMView->iItem;

	m_strOrigin = m_ctrlListCtrl.GetItemText(nSel, 0);
	m_strName = m_ctrlListCtrl.GetItemText(nSel, 1);
	m_strTel = m_ctrlListCtrl.GetItemText(nSel, 2);
	m_strAddress = m_ctrlListCtrl.GetItemText(nSel, 3);
	m_strMenu = m_ctrlListCtrl.GetItemText(nSel, 4);
	m_nRating = atoi(m_ctrlListCtrl.GetItemText(nSel, 5));
	m_strComment = m_ctrlListCtrl.GetItemText(nSel, 6);


	UpdateData(FALSE);
	*pResult = 0;
}



void CMyRestrauntDiaryDlg::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_nRating = m_sldSlider1.GetPos();
	UpdateData(FALSE);
	UpdateData(TRUE);
	*pResult = 0;
}
