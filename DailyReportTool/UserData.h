#pragma once
class CUserData
{
public:
	virtual ~CUserData(void);

public:
	CString UserName;
	CStringArray* MailTo;
	CStringArray* MailCc;
	CStringArray* MailWeekly;
	CStringArray* MailContent;
	CStringArray* BeforeReports;
	CStringArray* AfterReports;
	CStringArray* TodayReports;
	CStringArray* TomorowReprots;
	CString MailBody;

public:
	void static SplitStringByReturn(CStringArray& out, const CString& strIn);
	void static CombineStringWithReturn(CString& strOut, const CStringArray& in);

//public:
//	static CUserData& GetInstance(void)
//	{
//		static CUserData instance;
//		return instance;
//	}
//public:
//	static CUserData& GetInstance(void)
//	{
//		if (ms_apInstance->get() == NULL)
//			ms_apInstance->reset(new CUserData());
//		return *ms_apInstance;
//	}
//private:
//	static std::auto_ptr<CUserData> *ms_apInstance;

public:
	static CUserData* GetInstance(void)
	{
		if (ms_pInstance == NULL)
		{
			ms_pInstance = new CUserData();
		}
		return ms_pInstance;
	};

	static void Destroy()
	{  
		if (ms_pInstance != NULL)
		{
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}  


private:
	static CUserData* ms_pInstance;


private:
	CUserData(void)
	{
		BeforeReports = NULL;
		AfterReports = NULL;
		MailCc = NULL;
		MailContent = NULL;
		MailTo = NULL;
		MailWeekly = NULL;
		TodayReports = NULL;
		TomorowReprots = NULL;

	};
};

