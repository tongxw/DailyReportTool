#include "StdAfx.h"
#include "UserData.h"
#include "CommonDef.h"

CUserData* CUserData::ms_pInstance = NULL;

CUserData::~CUserData(void)
{
	CHECK_AND_DELETE(MailCc);
	CHECK_AND_DELETE(MailWeekly);
	CHECK_AND_DELETE(MailContent);
	CHECK_AND_DELETE(BeforeReports);
	CHECK_AND_DELETE(AfterReports);
	CHECK_AND_DELETE(TodayReports);
	CHECK_AND_DELETE(TomorowReprots);
}

void CUserData::SplitStringByReturn( CStringArray &out, const CString& strIn )
{
	out.RemoveAll();

	int nPos = 0;
	CString sToken = strIn.Tokenize("\r\n", nPos);
	while (sToken != "")   
	{   
		out.Add(sToken);
		sToken = strIn.Tokenize("\r\n", nPos);
	};   
}

void CUserData::CombineStringWithReturn( CString& strOut, const CStringArray& in )
{
	strOut.Empty();
	int nCount = in.GetCount();
	for (int i=0; i<nCount; ++i)
	{
		strOut += in[i];
		strOut += "\r\n";
	}
}
