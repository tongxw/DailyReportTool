#include "StdAfx.h"
#include "ExcelWrapper.h"
#include "UserData.h"

#define TODAY_WORK "Today's Work:"
#define TOMOROW_WORK "Tomorrow's Plan:"

#define COL_TODAY_WIDTH 60
#define COL_TOMOROW_WIDTH 60
#define COL_INDEX_WIDTH 10

CExcelWrapper::CExcelWrapper(void)
{
	m_pFile = NULL;
	m_nRow = 0;
}


CExcelWrapper::~CExcelWrapper(void)
{
}

BOOL CExcelWrapper::LoadFile( const CString& sFileName )
{
	//m_pFile = fopen ("dailyreport.xls", "wb");
	m_pFile = fopen (sFileName, "wb");

	if (m_pFile == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CExcelWrapper::Save( void )
{
	if (m_pFile != NULL)
	{
		m_excel.SetColumnWidth(0, COL_INDEX_WIDTH);
		m_excel.SetColumnWidth(1, COL_TODAY_WIDTH);
		m_excel.SetColumnWidth(2, COL_TOMOROW_WIDTH);

		m_excel.Write(m_pFile);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL CExcelWrapper::WriteOneWeek( const CStringArray& arrayReports, int nYear, int nMonth, int nDay )
{
	return TRUE;
}

CString CExcelWrapper::GetWeekName( int DayofWeek )
{
	switch (DayofWeek)
	{
	case 1:
		return "Sunday";
	case 2:
		return "Monday";
	case 3:
		return "Tuesday";
	case 4:
		return "Wednesday";
	case 5:
		return "Thursday";
	case 6:
		return "Friday";
	case 7:
		return "Saturday";
	default:
		return "WRONG INPUT";
	}
}

int CExcelWrapper::WriteOneDay( const CString& sReport, const CString& sPlan, int nYear, int nMonth, int nDay, int DayofWeek, int nBeginRow )
{
	return 0;
}


int CExcelWrapper::WriteOneDay( 
	const CStringArray& today, const CStringArray& tomorow, 
	int nYear, int nMonth, int nDay, 
	int DayofWeek, 
	int nBeginRow )
{
	int nRow = nBeginRow;
	
	CString& sAllReports = CUserData::GetInstance()->MailBody;

	// write date
	CString sDate;
	sDate.Format("%d-%d-%d", nYear, nMonth, nDay);
	m_excel(nRow, 0) = sDate;
	m_excel(nRow, 1) = GetWeekName(DayofWeek);
	m_excel(nRow, 2) = "";

	sAllReports += "{ " + sDate + " " + GetWeekName(DayofWeek) + " }\r\n";

	for (int i=0; i<3; ++i)
	{
		m_excel(nRow, i).setBorder(BORDER_TOP);
	}


	// write title
	++nRow;
	m_excel(nRow, 1) = TODAY_WORK;
	m_excel(nRow, 1).setAlignament(ALIGN_CENTER);
	sAllReports += "[ ";
	sAllReports += TODAY_WORK;
	sAllReports += " ]\r\n";

	m_excel(nRow, 2) = TOMOROW_WORK;
	m_excel(nRow, 2).setAlignament(ALIGN_CENTER);
	CString sTemp = "[ ";
	sTemp += TOMOROW_WORK;
	sTemp += " ]\r\n";

	for (int i=0; i<3; ++i)
	{
		m_excel(nRow, i).setBorder(BORDER_BOTTOM);
	}


	++nRow;
	int nReportPos = 0;
	int nPlanPos = 0;
	int nIndex = 1;
	int nCount = 0;

	int nTodayCount = today.GetCount();
	int nTomorowCount = tomorow.GetCount();
	BOOL bFoundSub = FALSE;
	
	while (nTodayCount > 0 || nTomorowCount > 0)   
	{   
		// write index
		CString sIndex;
		sIndex.Format("%d", nIndex);
		m_excel(nRow, 0) = sIndex;
		m_excel(nRow, 0).setBorder(BORDER_RIGHT);
		m_excel(nRow, 0).setAlignament(ALIGN_CENTER);

		// write report
		int nTodayRow = nRow;
		if (nTodayCount > 0)
		{
			CString sToday = today[nCount];
			//m_excel(nRow, 1) = sToday;

			// Check today's report begin with #.
			// if so, --nIndex, remove #. to today and add .# to index
			// TODO: fixed this code to look better. I'm confused in mind right now @Sep17
			int nFound = sToday.Find(".");
			if ((nFound == 1 || nFound == 2) && sToday.GetLength() > 2)
			{
				bFoundSub = TRUE;
				--nIndex;
				sIndex.Format("%d", nIndex);
				sIndex = sIndex + "." + sToday.Mid(0, nFound);
				sToday = sToday.Mid(nFound+1);
				m_excel(nRow, 0) = sIndex;
				//m_excel(nRow, 1) = sToday;
			}
			else
			{
				bFoundSub = FALSE;
				//m_excel(nRow, 1) = sToday;
			}

			// check string length first.
			// if it is longer than COL_TODAY_WIDTH, split it by the last word
			// and add it to an additional line, until it is shorter than width
			nTodayRow = SplitLongerString(sToday, nRow, COL_TODAY_WIDTH);
			
			//separate each main number with a break line
			if (!bFoundSub && nIndex != 1)
			{
				sAllReports += "\r\n";
			}
			sAllReports += sIndex + ". " + sToday + "\n";
		}
		else
		{
			m_excel(nRow, 1) = "";
		}

		// write tomorow
		int nTomorrowRow = nRow;
		if (nTomorowCount > 0)
		{
			m_excel(nRow, 2) = tomorow[nCount];
			sTemp += sIndex + ". " + tomorow[nCount] + "\r\n";
		}
		else
		{
			m_excel(nRow, 2) = "";
		}

		nRow = max(nTodayRow, nTomorrowRow);

		++nRow;
		++nCount;
		++nIndex;
		--nTodayCount;
		--nTomorowCount;	
	};

	sAllReports += "\r\n" + sTemp;
	sAllReports += "-----------------------------------------------\r\n";
	sAllReports += "\r\n\r\n";

	--nRow;
	m_excel(nRow, 0).setBorder(BORDER_RIGHT|BORDER_BOTTOM);
	for (int i=1; i<3; ++i)
	{
		m_excel(nRow, i).setBorder(BORDER_BOTTOM);
	}


	return ++nRow;
}

int CExcelWrapper::SplitLongerString( CString sLongeString, int nRow, int nLength )
{
	BOOL bFoundLonger = FALSE;
	while (sLongeString.GetLength() > nLength)
	{
		// Find last string before the given length
		int nStringLen = sLongeString.GetLength();
		CString sSplit = sLongeString.Left(nLength);
		int nLastSpace = max(sSplit.ReverseFind(' '), sSplit.ReverseFind('\\'));
		nLastSpace = max(nLastSpace, sSplit.ReverseFind('-'));
		nLastSpace = max(nLastSpace, sSplit.ReverseFind('_'));
		if (nLastSpace == -1)
		{
			nLastSpace = nLength;
		}

		sSplit = sLongeString.Left(nLastSpace + 1);
		sLongeString = sLongeString.Right(nStringLen-1-nLastSpace);


		// set it to excel
		// do not rewrite index col if it is the first found
		if (!bFoundLonger)
		{
			bFoundLonger = TRUE;
		}
		else
		{
			m_excel(nRow, 0) = "";
			m_excel(nRow, 0).setBorder(BORDER_RIGHT);
		}

		m_excel(nRow, 1) = sSplit;
		m_excel(nRow, 2) = "";
		++nRow;
	}

	if (bFoundLonger)
	{
		m_excel(nRow, 0) = "";
		m_excel(nRow, 0).setBorder(BORDER_RIGHT);
		m_excel(nRow, 2) = "";
	}

	m_excel(nRow, 1) = sLongeString;
	return nRow;
}

void CExcelWrapper::WriteIndex( int nIndex )
{
	CString sIndex;
	if (nIndex != 0)
	{
		sIndex.Format("%d.", nIndex);
	}

	m_excel(m_nRow, 0) = sIndex;
	m_excel(m_nRow, 0).setBorder(BORDER_RIGHT);
}

void CExcelWrapper::WriteTodayLine( const CString& sToday )
{

}

void CExcelWrapper::WriteTomorrowLine( const CString& sTomorrow )
{

}
