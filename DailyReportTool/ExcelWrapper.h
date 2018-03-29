#pragma once
#include "mexcel.h"

class CExcelWrapper
{
public:
	CExcelWrapper(void);
	virtual ~CExcelWrapper(void);
	
	BOOL LoadFile(const CString& sFileName);

	int WriteOneDay(const CString& sReport, const CString& sPlan,
		int nYear, int nMonth, int nDay, int DayofWeek, int nBeginRow);

	int WriteOneDay( const CStringArray& today, const CStringArray& tomorow, 
		int nYear, int nMonth, int nDay, int DayofWeek, int nBeginRow );

	BOOL WriteOneWeek(const CStringArray& arrayReports, 
		int nYear, int nMonth, int nDay);

	BOOL Save(void);
	CString GetWeekName( int DayofWeek );
	int SplitLongerString(CString sLongeString, int nRow, int nLength);

	void set_row(int nRow) {m_nRow = nRow;};
	int row(void) const {return m_nRow;};
	//CString GetAllReports(void) const {return m_sAllReports;};
	
	//enum DAY_OF_WEEK {EXCEL_MON, EXCEL_TUE, EXCEL_WED, EXCEL_THU, EXCEL_FRI};

private:
	void WriteIndex(int nIndex);
	void WriteTodayLine(const CString& sToday);
	void WriteTomorrowLine(const CString& sTomorrow);




private:
	miniexcel::CMiniExcel m_excel;
	FILE *m_pFile;
	int m_nRow;
	//CString m_sAllReports;
};

