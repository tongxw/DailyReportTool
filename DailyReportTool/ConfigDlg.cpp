// ConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DailyReportTool.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"

// email address by default
#define MAIL_BERNARD "Bernard.Chang"
#define MAIL_JJ "JJ.Zeng"
#define MAIL_JOEY "Joey.Yu"
#define MAIL_MGMT_PEK "eng-mgmt-pek"
#define MAIL_BALA "Bala.Viswanathan"
#define MAIL_MARK "Mark.Hughes"
#define CONTENT_BEFORE "Hi, guys,\r\nMy daily report is as follows, please check."
#define CONTENT_AFTER "The same contents are in the attachment,\r\nBest regards,"

#include "User.h"
#include "XmlUser.h"
#include "DataAccess.h"

#include "UserData.h"


// CConfigDlg 对话框

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigDlg::IDD, pParent)
{

	m_strName = _T("");
	m_strAfterReports = _T("");
	m_strBeforeReports = _T("");
}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCVSLISTBOX_TO, m_lbTo);
	DDX_Control(pDX, IDC_MFCVSLISTBOX_CC, m_lbCC);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Control(pDX, IDC_MFCVSLISTBOX_WEEKLY, m_lbWeekly);
	//	DDX_Control(pDX, IDC_EDIT_NAME, m_edName);
	DDX_Text(pDX, IDC_EDIT_AFTER, m_strAfterReports);
	DDX_Text(pDX, IDC_EDIT_BEFORE, m_strBeforeReports);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigDlg 消息处理程序


BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_strName.IsEmpty())
	{
		// use default params for new user
		m_lbTo.AddItem(MAIL_BERNARD);
		m_lbTo.AddItem(MAIL_JJ);
		m_lbTo.AddItem(MAIL_JOEY);

		m_lbCC.AddItem(MAIL_MGMT_PEK);

		m_lbWeekly.AddItem(MAIL_BALA);
		m_lbWeekly.AddItem(MAIL_MARK);

		m_strBeforeReports = CONTENT_BEFORE;
		m_strAfterReports = CONTENT_AFTER;
	}
	else
	{
		// load user data to view
		CUserData *pData = CUserData::GetInstance();
		ASSERT(pData->MailTo);
		ASSERT(pData->MailCc);
		ASSERT(pData->MailWeekly);
		ASSERT(pData->BeforeReports);
		ASSERT(pData->AfterReports);

		SetListCtrlByStrings(m_lbTo, *(pData->MailTo));
		SetListCtrlByStrings(m_lbCC, *(pData->MailCc));
		SetListCtrlByStrings(m_lbWeekly, *(pData->MailWeekly));
		CUserData::CombineStringWithReturn(m_strBeforeReports, *(pData->BeforeReports));
		CUserData::CombineStringWithReturn(m_strAfterReports, *(pData->AfterReports));
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// save all the items to DB when click "OK"
void CConfigDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty())
	{
		AfxMessageBox("Hey, tell me who you are.");
		return;
	}

	// if I got here, the CUserData must be initialized in main dialog init;
	// so I don't check pointers validity.

	// save data to memory
	CUserData* pData = CUserData::GetInstance();
	ASSERT(pData->MailTo);
	ASSERT(pData->MailCc);
	ASSERT(pData->MailWeekly);
	ASSERT(pData->BeforeReports);
	ASSERT(pData->AfterReports);

	// set user name
	pData->UserName = m_strName;

	// set "to"
	GetStringsFromListCtrl(*(pData->MailTo), m_lbTo);

	// set "cc"
	GetStringsFromListCtrl(*(pData->MailCc), m_lbCC);

	// set "weekly"
	GetStringsFromListCtrl(*(pData->MailWeekly), m_lbWeekly);

	//GetStringsFromEditCtrl(*(pData->mail))
	CUserData::SplitStringByReturn(*(pData->BeforeReports), m_strBeforeReports);
	CUserData::SplitStringByReturn(*(pData->AfterReports), m_strAfterReports);
	

	CDialogEx::OnOK();
}

void CConfigDlg::GetStringsFromListCtrl(CStringArray& arrayOut, const CVSListBox& lb) const
{
	int nCount = lb.GetCount();
	for (int i=0; i<nCount; ++i)
	{
		arrayOut.Add(lb.GetItemText(i));
	}
}

void CConfigDlg::SetListCtrlByStrings( CVSListBox& lb, const CStringArray& arrayOut )
{
	int nCount = arrayOut.GetCount();
	for (int i=0; i<nCount; ++i)
	{
		lb.AddItem(arrayOut[i]);
	}

}
