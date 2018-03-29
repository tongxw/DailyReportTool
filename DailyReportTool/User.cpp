#include "StdAfx.h"
#include "User.h"


CUser::CUser(void)
{
}

CUser::CUser( const CString& sName )
{
	m_sName = sName;
}


CUser::~CUser(void)
{
}

BOOL CUser::GetReports(CStringArray& arrayToday, CStringArray &arrayTomorrow) const
{
	CTime time = CTime::GetCurrentTime();
	return GetReports(time.GetYear(), time.GetMonth(), time.GetDay(), 
		arrayToday, arrayTomorrow);
}

BOOL CUser::SetReports(const CStringArray& arrToday, const CStringArray &arrayTomorrow)
{
	CTime time = CTime::GetCurrentTime();
	return SetReports(time.GetYear(), time.GetMonth(), time.GetDay(),
		arrToday, arrayTomorrow);
}

