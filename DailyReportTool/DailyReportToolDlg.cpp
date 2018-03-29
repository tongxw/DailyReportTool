
// DailyReportToolDlg.cpp : implementation file
//


#include "stdafx.h"
#include "UserData.h"
#include "DailyReportTool.h"
#include "DailyReportToolDlg.h"
#include "afxdialogex.h"
#include "ConfigDlg.h"

#include "XmlUser.h"

//#include "BasicExcel.hpp"

#include "DataAccess.h"

#include "ExcelWrapper.h"
using namespace miniexcel;

#include "cmapi.h"

//#define SAVEPATH "c:\\temp\\a.xls"


#include "CommonDef.h"

#define COMBO_BOX_ADD_ME "<Add Me!>"
#define ATTACHMENT_NAME "dailyreport.xls"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDailyReportToolDlg dialog




CDailyReportToolDlg::CDailyReportToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDailyReportToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strToday = _T("");
	m_strTomorow = _T("");

	m_pUser = NULL;
	m_bHasAttachment = FALSE;
}

void CDailyReportToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TODAY, m_strToday);
//	DDX_MonthCalCtrl(pDX, IDC_MONTHCALENDAR, m_ctDate);
	DDX_Text(pDX, IDC_EDIT_TOMOROW, m_strTomorow);
	DDX_Control(pDX, IDC_COMBO_USER, m_cbUser);
}

BEGIN_MESSAGE_MAP(CDailyReportToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BUTTON1, &CDailyReportToolDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_MFCBUTTON1, &CDailyReportToolDlg::OnBnClickedMfcbutton1)
	//ON_BN_CLICKED(IDC_MFCBUTTON2, &CDailyReportToolDlg::OnBnClickedMfcbutton2)
	//ON_BN_CLICKED(IDC_MFCBUTTON3, &CDailyReportToolDlg::OnBnClickedMfcbutton3)
	//ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR, &CDailyReportToolDlg::OnSelchangeMonthcalendar)
	ON_CBN_SELCHANGE(IDC_COMBO_USER, &CDailyReportToolDlg::OnSelchangeComboUser)
	ON_BN_CLICKED(IDC_BTN_CFG, &CDailyReportToolDlg::OnBnClickedBtnCfg)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDailyReportToolDlg::OnOK)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SEND, &CDailyReportToolDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CDailyReportToolDlg::OnBnClickedButtonAbout)
	ON_EN_CHANGE(IDC_EDIT_TODAY, &CDailyReportToolDlg::OnEnChangeEditToday)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDailyReportToolDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CDailyReportToolDlg message handlers

BOOL CDailyReportToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// get database API from factory
	m_pUser = CDataAccess::CreateUser(CDataAccess::TYPE_XML);
	if (m_pUser == NULL)
	{
		// exit program
		AfxMessageBox("I can't load config, please check database or config file, sorry~~");
		CDialogEx::OnCancel();
	}

	// If config file does not exist, create a new one
	if (!m_pUser->LoadData(CONFIG_FILE_NAME))
	{
		AfxMessageBox("Missing config file, please put data.xml in the same location with program.");
		CDialogEx::OnCancel();
		//TODO: create default file template if not exist
		//FILE *f = fopen(CONFIG_FILE_NAME, "wb");
	}

	// Get all users' names from config file
	// and add them to combo box
	m_cbUser.AddString(COMBO_BOX_ADD_ME);
	CStringArray arrayNames;
	m_pUser->GetAllUserNames(arrayNames);
	int nCount = arrayNames.GetCount();
	for (int i=0; i<nCount; ++i)
	{
		m_cbUser.AddString(arrayNames[i]);
	}

	//// Set the first user name as default one
	CUserData *pData = CUserData::GetInstance();
	
	if (nCount > 0)
	{
		pData->UserName = arrayNames[0];
		m_cbUser.SelectString(0, arrayNames[0]);
	}

	// Load the default user's config data to memory
	LoadCfg(pData->UserName);
	LoadReports();

	//CStringArray *pToday = new CStringArray();
	//CStringArray *pTomorow = new CStringArray();
	//m_pUser->GetReports(*pToday, *pTomorow);
	//pData->TodayReports = pToday;
	//pData->TomorowReprots = pTomorow;


	//// Update view
	////UpdateData(FALSE);
	//nCount = pData->TodayReports->GetCount();
	//for (int i=0; i<nCount; ++i)
	//{
	//	m_strToday += pData->TodayReports->GetAt(i);
	//	m_strToday += "\r\n";
	//}

	//nCount = pData->TomorowReprots->GetCount();
	//for (int i=0; i<nCount; ++i)
	//{
	//	m_strTomorow += pData->TomorowReprots->GetAt(i);
	//	m_strTomorow += "\r\n";
	//}

	UpdateData(FALSE);

	//delete m_pUser;
	//m_pUser = NULL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDailyReportToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDailyReportToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDailyReportToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CDailyReportToolDlg::OnBnClickedButton1()
//{
//	m_pUser->LoadData("data.xml");
//
//	CStringArray arrayToday;
//	CStringArray arrayTomorrow;
//	m_pUser->GetDailyReport(arrayToday, arrayTomorrow);
//
//	for (int i=0; i<arrayToday.GetCount(); ++i)
//	{
//		m_strToday += arrayToday[i];
//		m_strToday += "\r\n";
//	}
//
//	UpdateData(FALSE);
//
//	//using namespace tinyxml2;
//	//// TODO: 在此添加控件通知处理程序代码
//	//tinyxml2::XMLDocument doc;
//	//doc.LoadFile( "data.xml" );
//
//
//
//	//int v0 = 0;
//	//int v1 = 0;
//
//	//tinyxml2::XMLElement* nodeUser = doc.FirstChildElement()->FirstChildElement( "user" );
//	//const char* str = nodeUser->Attribute("name");
//
//	//int err = doc.ErrorID();
//
//}


BOOL CDailyReportToolDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	// save user data to file
	//UpdateData(TRUE);
	//SaveCfg();

	if (m_pUser != NULL)
	{
		delete m_pUser;
		m_pUser = NULL;
	}

	CUserData* pData = CUserData::GetInstance();
	pData->Destroy();

	return CDialogEx::DestroyWindow();
}


//void CDailyReportToolDlg::OnBnClickedMfcbutton1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//
//	CStringArray arrReport;
//
//	int nPos = 0;
//	CString sToken = m_strToday.Tokenize("\r\n", nPos);
//	while (sToken != "")   
//	{   
//		arrReport.Add(sToken);
//		sToken = m_strToday.Tokenize("\r\n", nPos);
//	};   
//
//	m_pUser->SetDailyReport(arrReport);
//	m_pUser->CommitData();
//}


//void CDailyReportToolDlg::OnBnClickedMfcbutton2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(FALSE);
//	CExcelWrapper excel;
//
//	excel.LoadFile("dailyreport.xls");
//	excel.WriteReport(m_strToday, m_strToday, 2012, 9, 10, 5, 10);
//	excel.Save();
//}


//void CDailyReportToolDlg::OnBnClickedMfcbutton3()
//{
//	CMapiMessage message;
//	message.m_To.Add(_T("tongxw.cn@gmail.com"));
//	message.m_CC.Add(_T("pjn@indigo.ie"));
//	message.m_BCC.Add(_T("pj.naughter@softech-telecom.com"));
//	message.m_sSubject = _T("Demo email using CMapiSession - Please ignore");
//	message.m_sBody = _T("This is the body of the message\r\nRegards,\r\nCMapiMessage");
//	message.m_Attachments.Add(_T("D:\\dailyreport.xls"));
//	//message.m_AttachmentTitles.Add(_T("my autoexec.bat"));
//
//	//Setup the mapi session
//	CMapiSession session;
//	BOOL bMapiInstalled = session.MapiInstalled();
//	if (!bMapiInstalled)
//	{
//		AfxMessageBox("mapi.dll is not installed. I can't send the email for you.");
//	}
//
//	//Logon to MAPI
//	//BOOL bLoggedOn = session.Logon(_T("PJ Naughter")); //You will need to change this to use a MAPI profile
//	//setup on your computer. If you want CMapiSession to
//	//bring up a dialog to allow a MAPI profile to be 
//	//interactively chosen, then you will need to create
//	//a Window in your app using the normal MFC mechanims 
//	//if (bLoggedOn)
//	//{
//
//	//	//Send the message
//	//	BOOL bSent = session.Send(message);
//	//}
//	BOOL bSent = session.Send(message, this->m_hWnd);
//
//
//}


//void CDailyReportToolDlg::OnSelchangeMonthcalendar(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	//UpdateData(FALSE);
//
//	*pResult = 0;
//}


void CDailyReportToolDlg::OnSelchangeComboUser()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_cbUser.GetCurSel();
	CString strCBText;
	m_cbUser.GetLBText( nIndex, strCBText);
	if (strCBText == COMBO_BOX_ADD_ME)
	{
		CConfigDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			SaveCfg();
			AddNewUser(dlg.m_strName);
		}
	}
}


void CDailyReportToolDlg::OnBnClickedBtnCfg()
{
	CConfigDlg dlg;
	int nIndex = m_cbUser.GetCurSel();
	CString strCBText;
	if (nIndex != -1)
	{
		m_cbUser.GetLBText( nIndex, strCBText);
		dlg.m_strName = strCBText;
	}
	if (dlg.DoModal() == IDOK)
	{
		// save config files
		SaveCfg();

		//CUserData* pData = CUserData::GetInstance();
		// if user change user name
		if (dlg.m_strName != strCBText)
		{
			AddNewUser(dlg.m_strName);
		}
	}
}

void CDailyReportToolDlg::LoadCfg(const CString& strName)
{
	//if (strName.IsEmpty())
	//{
	//	return;
	//}

	m_pUser->set_name(strName);

	CUserData *pData = CUserData::GetInstance();
	
	CStringArray *pMailTo = new CStringArray();
	m_pUser->GetToMail(*pMailTo);
	CHECK_AND_DELETE(pData->MailTo);
	pData->MailTo = pMailTo;

	CStringArray *pMailCc = new CStringArray();
	m_pUser->GetCCMail(*pMailCc);
	CHECK_AND_DELETE(pData->MailCc);
	pData->MailCc = pMailCc;

	CStringArray *pMailWeekly = new CStringArray();
	m_pUser->GetWeeklyCC(*pMailWeekly);
	CHECK_AND_DELETE(pData->MailWeekly);
	pData->MailWeekly = pMailWeekly;


	CStringArray *pBefore = new CStringArray();
	m_pUser->GetContentsBeforeReports(*pBefore);
	CHECK_AND_DELETE(pData->BeforeReports);
	pData->BeforeReports = pBefore;

	CStringArray *pAfter = new CStringArray();
	m_pUser->GetContentsAfterReports(*pAfter);
	CHECK_AND_DELETE(pData->AfterReports);
	pData->AfterReports = pAfter;
	
}

void CDailyReportToolDlg::AddNewUser( const CString& m_strName )
{
	// if it's new, add this user
	m_pUser->set_name(m_strName);
	if (m_cbUser.SelectString(0, m_strName) == CB_ERR)
	{
		m_cbUser.AddString(m_strName);
		m_cbUser.SelectString(0, m_strName);

		LoadReports();

		UpdateData(FALSE);
	}


}

void CDailyReportToolDlg::LoadReports()
{
	CStringArray *pToday = new CStringArray();
	CStringArray *pTomorow = new CStringArray();
	m_pUser->GetReports(*pToday, *pTomorow);
	CUserData* pData = CUserData::GetInstance();
	CHECK_AND_DELETE(pData->TodayReports);
	CHECK_AND_DELETE(pData->TomorowReprots);

	pData->TodayReports = pToday;
	pData->TomorowReprots = pTomorow;


	// Update view
	CUserData::CombineStringWithReturn(m_strToday, *(pData->TodayReports));
	CUserData::CombineStringWithReturn(m_strTomorow, *(pData->TomorowReprots));
}


void CDailyReportToolDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	//SaveReports();
	//m_pUser->CommitData();

	PostMessage(WM_CLOSE);
	//CDialogEx::OnOK();
}


void CDailyReportToolDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nRet = MessageBox(_T("want to exit? \n(unsaved reports will be lost)"), _T("Confirm"), 
		MB_ICONQUESTION | MB_OKCANCEL);
	if(nRet == IDCANCEL)
	{
		return;
	}



	//UpdateData(TRUE);

	CDialogEx::OnClose();
}

void CDailyReportToolDlg::SaveCfg()
{
	ASSERT(m_pUser != NULL);

	CUserData *pData = CUserData::GetInstance();
	
	// save user name
	m_pUser->AddUser(pData->UserName);

	// save mailto mailcc mailweekly
	m_pUser->SetToMail(*(pData->MailTo));
	m_pUser->SetCCMail(*(pData->MailCc));
	m_pUser->SetWeeklyCC(*(pData->MailWeekly));


	// save body content
	m_pUser->SetContentsBeforeAndAfterReports(*(pData->BeforeReports), *(pData->AfterReports));
}

void CDailyReportToolDlg::SaveReports()
{
	ASSERT(m_pUser != NULL);

	CUserData *pData = CUserData::GetInstance();
	ASSERT(pData->TodayReports != NULL);
	ASSERT(pData->TomorowReprots != NULL);

	UpdateData(TRUE);

	// save report
	CUserData::SplitStringByReturn(*(pData->TodayReports), m_strToday);
	CUserData::SplitStringByReturn(*(pData->TomorowReprots), m_strTomorow);
	m_pUser->SetReports(*(pData->TodayReports), *(pData->TomorowReprots));
}


void CDailyReportToolDlg::OnOK()
{
	UpdateData(TRUE);
	SaveReports();
	m_pUser->CommitData();

	CDialogEx::OnOK();
}


void CDailyReportToolDlg::OnBnClickedBtnSend()
{
	//GenerateExcel();
	CUserData* pData = CUserData::GetInstance();
	//TODO: there is a problem, generate excel file is very slow,
	// Send() is called before the excel file is there.

	// send it using MAPI
	CTime time = CTime::GetCurrentTime();
	CString sDate;
	sDate.Format("%d, %d-%d", time.GetYear(), time.GetMonth(), time.GetDay());

	CMapiMessage message;
	ResovleEMail(message.m_To, *(pData->MailTo));
	//message.m_To.Add("tongxw@163.com");
	//message.m_To.Copy(*(pData->MailTo));
	//message.m_CC.Copy(*(pData->MailCc));
	ResovleEMail(message.m_CC, *(pData->MailCc));


	int nDayOfWeek = time.GetDayOfWeek();
	// if it's Friday or weekend, need to add more people in CC field
	if (nDayOfWeek == 6 || nDayOfWeek == 7 || nDayOfWeek == 1)
	{
		ResovleEMail(message.m_CC, *(pData->MailWeekly));
	}

	message.m_sSubject = _T("Daily Report ");
	message.m_sSubject += sDate;

	//message.m_sBody = "<html><body>";

	CString sBody;
	CUserData::CombineStringWithReturn(sBody, *(pData->BeforeReports));
	//sBody = "<p><b>" + sBody + "</b><p>";
	message.m_sBody = sBody;
	//message.m_sBody += "\r\n";
	message.m_sBody += "===========================================================================\r\n";
	message.m_sBody += pData->MailBody;
	message.m_sBody.TrimRight("\r\n");
	message.m_sBody += "\r\n";
	message.m_sBody += "===========================================================================\r\n";
	CUserData::CombineStringWithReturn(sBody, *(pData->AfterReports));
	message.m_sBody += sBody + pData->UserName;

	//message.m_sBody += "</body></html>";
	if (m_bHasAttachment)
	{
		message.m_Attachments.Add(m_strAttachment);
	}
	//message.m_AttachmentTitles.Add(_T("my autoexec.bat"));
	
	//Setup the mapi session
	CMapiSession session;
	BOOL bMapiInstalled = session.MapiInstalled();
	if (!bMapiInstalled)
	{
		AfxMessageBox("mapi.dll is not installed. I can't send the email for you.");
	}
	
	//Logon to MAPI
	//BOOL bLoggedOn = session.Logon(_T("PJ Naughter")); //You will need to change this to use a MAPI profile
	//setup on your computer. If you want CMapiSession to
	//bring up a dialog to allow a MAPI profile to be 
	//interactively chosen, then you will need to create
	//a Window in your app using the normal MFC mechanims 
	//if (bLoggedOn)
	//{
	
	//	//Send the message
	//	BOOL bSent = session.Send(message);
	//}

	BOOL bSent = session.Send(message, this->m_hWnd);
	CUserData::GetInstance()->MailBody.Empty();
}

void CDailyReportToolDlg::ResovleEMail( CStringArray& out, const CStringArray& in )
{
	int nCount = in.GetCount();
	for (int i=0; i<nCount; ++i)
	{
		out.Add(in[i] + "@watchguard.com");
	}
}


void CDailyReportToolDlg::OnBnClickedButtonAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CDailyReportToolDlg::OnEnChangeEditToday()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CDailyReportToolDlg::OnBnClickedBtnSave()
{
	// TODO: Add your control notification handler code here
	GenerateExcel();

	SaveReports();
	if (!m_pUser->CommitData())
	{
		AfxMessageBox("Ouch, I can't save the data file.");
	}
}

void CDailyReportToolDlg::GenerateExcel()
{
	CUserData *pData = CUserData::GetInstance();
	ASSERT(pData->TodayReports != NULL);
	ASSERT(pData->TomorowReprots != NULL);

	// generate reports in excel file
	UpdateData(TRUE);

	// Save the editbox content to file
	CUserData::SplitStringByReturn((*pData->TodayReports), m_strToday);
	CUserData::SplitStringByReturn((*pData->TomorowReprots), m_strTomorow);

	CTime time = CTime::GetCurrentTime();

	m_strAttachment = time.Format("%Y_Daily_Report_%b_%d_%a_by_");
	m_strAttachment += pData->UserName + ".xls";
	m_strAttachment = ATTACHMENT_NAME;
	CExcelWrapper excel;
	excel.LoadFile(m_strAttachment);
	
	//CString sDate;
	//sDate.Format("%d, %d-%d", time.GetYear(), time.GetMonth(), time.GetDay());
	int nDayOfWeek = time.GetDayOfWeek();
	//int nCountDay = nDayofWeek;

	if (nDayOfWeek == 1)
	{
		// if it is weekend, set it as Friday
		CTimeSpan ts(2,0,0,0);
		time = time - ts; 
		nDayOfWeek = 6;
	} else if (nDayOfWeek == 7)
	{
		CTimeSpan ts(1,0,0,0);
		time = time -ts;
		nDayOfWeek = 6;
	}

	int nRow = 0;
	while (nDayOfWeek > 1)
	{
		if (nRow != 0)
		{
			// if it is not today's report
			// load it from file
			// otherwise, use the contents in the edit box directly
			m_pUser->GetReports(time.GetYear(), time.GetMonth(), time.GetDay(), 
				*(pData->TodayReports), *(pData->TomorowReprots));
		}

		nRow = excel.WriteOneDay(
			*(pData->TodayReports), *(pData->TomorowReprots), 
			time.GetYear(), time.GetMonth(), time.GetDay(),
			nDayOfWeek,
			nRow);
		
		++nRow;

		CTimeSpan ts(1,0,0,0);
		time = time - ts;
		--nDayOfWeek;
	}
	m_bHasAttachment = excel.Save();
}
