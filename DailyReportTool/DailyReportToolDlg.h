
// DailyReportToolDlg.h : header file
//

#pragma once

class CUser;
// CDailyReportToolDlg dialog
class CDailyReportToolDlg : public CDialogEx
{
// Construction
public:
	CDailyReportToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DAILYREPORTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

public:
	virtual BOOL DestroyWindow();
	CString m_strToday;
	afx_msg void OnBnClickedMfcbutton1();
	afx_msg void OnBnClickedMfcbutton2();
	afx_msg void OnBnClickedMfcbutton3();
	//afx_msg void OnSelchangeMonthcalendar(NMHDR *pNMHDR, LRESULT *pResult);
	CTime m_ctDate;
	CString m_strTomorow;
	CComboBox m_cbUser;
	afx_msg void OnSelchangeComboUser();
	afx_msg void OnBnClickedBtnCfg();

private:
	void LoadCfg(const CString& strCBText);
	void AddNewUser( const CString& m_strName );
	void LoadReports();
private:
	CUser* m_pUser;
	CString m_strAttachment;
	BOOL m_bHasAttachment;
public:
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnClose();
	void SaveCfg();
	void SaveReports();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnSend();

	void GenerateExcel();

	void ResovleEMail( CStringArray& out, const CStringArray& in );
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnEnChangeEditToday();
	afx_msg void OnBnClickedBtnSave();
};
