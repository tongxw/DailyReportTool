#pragma once
/********************************************************************
	created:	2012/09/08
	created:	8:9:2012   13:02
	filename: 	d:\Documents\Visual Studio 2010\Projects\DailyReportTool\DailyReportTool\User.h
	file path:	d:\Documents\Visual Studio 2010\Projects\DailyReportTool\DailyReportTool
	file base:	User
	file ext:	h
	author:		Xiaowei Tong@BDC
	
	purpose:	user interface for r/w database
*********************************************************************/
class CUser
{
public:
	CUser(void);
	CUser(const CString& sName);
	virtual ~CUser(void);

public:
	virtual BOOL LoadData(const CString& sDBName) = 0;
	virtual BOOL CommitData() = 0;

	virtual BOOL AddUser(const CString& sName) = 0;

	CString name() { return m_sName; };
	void set_name(const CString& sName) { m_sName = sName; };

	virtual BOOL GetAllUserNames(CStringArray& arrNames) const = 0;

	virtual BOOL GetEMail(CString& sEMail) const = 0;
	virtual BOOL SetEMail(const CString& sEMail) = 0;

	virtual BOOL GetToMail(CStringArray& arrTo) const  = 0;
	virtual BOOL SetToMail(const CStringArray& arrTo) = 0;

	virtual BOOL GetCCMail(CStringArray& arrCC) const = 0;
	virtual BOOL SetCCMail(const CStringArray& arrCC) = 0;

	virtual BOOL GetWeeklyCC(CStringArray& arrWeekly) const = 0;
	virtual BOOL SetWeeklyCC(const CStringArray& arrWeekly) = 0;
	
	// get by current date
	virtual BOOL GetReports(CStringArray& arrayToday, CStringArray &arrayTomorrow) const;
	// set by current date
	virtual BOOL SetReports(const CStringArray& arrToday, const CStringArray &arrayTomorrow);

	virtual BOOL GetReports(int nYear, int nMonth, int nDay, 
		CStringArray& arrayToday, CStringArray &arrayTomorrow) const = 0;
	virtual BOOL SetReports(int nYear, int nMonth, int nDay, 
		const CStringArray& arrToday, const CStringArray &arrayTomorrow)= 0;

	virtual BOOL GetContentsBeforeReports(CStringArray& out) = 0;
	virtual BOOL GetContentsAfterReports(CStringArray& out) = 0;

	virtual BOOL SetContentsBeforeAndAfterReports(
		const CStringArray& before, const CStringArray& after) = 0;


protected:
	CString m_sDBName;
	CString m_sName;

};

