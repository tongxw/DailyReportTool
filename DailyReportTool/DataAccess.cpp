#include "StdAfx.h"
#include "DataAccess.h"

#include "XmlUser.h"

CDataAccess::CDataAccess(void)
{
}


CDataAccess::~CDataAccess(void)
{
}

CUser* CDataAccess::CreateUser(int nType)
{
	switch (nType)
	{
	case TYPE_XML:
		return new CXmlUser();
	default:
		return NULL;
	}
}
