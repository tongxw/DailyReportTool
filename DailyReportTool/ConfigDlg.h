#pragma once

// CConfigDlg �Ի���
//#include <afxvslistbox.h>
//class CUser;
class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfigDlg();

// �Ի�������
	enum { IDD = IDD_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CVSListBox m_lbTo;
	CVSListBox m_lbCC;
	CString m_strName;
	virtual BOOL OnInitDialog();
	CVSListBox m_lbWeekly;
	afx_msg void OnBnClickedOk();

private:
	void GetStringsFromListCtrl(CStringArray& arrayOut, const CVSListBox& lb ) const;
	void SetListCtrlByStrings( CVSListBox& lb, const CStringArray& arrayOut );
//private:
//	CUser* m_pUser;
public:
	CString m_strAfterReports;
	CString m_strBeforeReports;
};
