#pragma once

/********************************************************************
	created:	2012/09/08
	created:	8:9:2012   12:53
	filename: 	\DataAccess.h
	file path:	\Projects\DailyReportTool\DailyReportTool
	file base:	DataAccess
	file ext:	h
	author:		Xiaowei Tong@WatchGuard
	
	purpose:	get daily report data
*********************************************************************/
class CUser;

class CDataAccess
{
public:
	CDataAccess(void);
	virtual ~CDataAccess(void);

	static CUser* CreateUser(int nType);
	
	enum DATA_TYPE {TYPE_XML = 0};

private:


};

