
// MyRestrauntDiaryDlg.h: 헤더 파일
//

#pragma once


// CMyRestrauntDiaryDlg 대화 상자
class CMyRestrauntDiaryDlg : public CDialogEx
{
// 생성입니다.
public:
	CMyRestrauntDiaryDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	int CMyRestrauntDiaryDlg::setDataToList();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYRESTRAUNTDIARY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_nRating;
	CListCtrl m_ctrlListCtrl;
	CSliderCtrl m_sldSlider1;
	CString m_strOrigin;
	CString m_strName;
	CString m_strMenu;
	CString m_strTel;
	CString m_strAddress;
	CString m_strComment;
	afx_msg void OnBnClickedBtnadd();
	afx_msg void OnBnClickedBtnmodify();
	afx_msg void OnBnClickedBtndelete();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
};
