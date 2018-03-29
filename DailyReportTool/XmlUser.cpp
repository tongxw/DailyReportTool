#include "StdAfx.h"
#include "XmlUser.h"

#include "tinyxml2.h"
using namespace tinyxml2;

const char* USER_NODE_NAME = "user";
const char* NODE_TO = "to";
const char* NODE_CC = "cc";
const char* NODE_WEEKLY = "weekly";
const char* NODE_CONTENT = "content";
const char* NODE_TODAY = "today";
const char* NODE_TOMORROW = "tomorrow";
const char* NODE_BEFORE = "before";
const char* NODE_AFTER = "after";
const char* NODE_BODY = "body";
//const char* ATTR_NAME = "name";
const char* ATTR_EMAIL = "email";

const char* NODE_YEAR = "year";
const char* NODE_MONTH = "month";
const char* NODE_DAY = "day";

const char* ATTR_VALUE = "value";

#define  GOTO_CHILD(pNode, sNode, nValue) \
	pNode = pNode->FirstChildElement(sNode);\
	if (nValue != -1)\
	{\
	while(pNode != NULL)\
	{\
	if (atoi(pNode->Attribute(ATTR_VALUE)) == nValue) break;\
	pNode = pNode->NextSiblingElement();\
	}\
	if (pNode == NULL) return FALSE;\
	}
	

//#define GOTO_USER_NODE(pNode) 	\
//	pNode = m_doc.FirstChildElement();\
//	if (pNote == NULL) return FALSE;\
//	pNode = pNode->FirstChildElement(USER_NODE_NAME);\
//	if (pNode == NULL ) return FALSE;\
//	if (!m_sName.IsEmpty()) \
//	{\
//		while(pNode != NULL)\
//		{\
//			const char* sCurrentName = pNode->Attribute(ATTR_VALUE);\
//			if (strcmp(sCurrentName, m_sName) == 0)\
//			{\
//				break;\
//			}\
//		pNode = pNode->NextSiblingElement(USER_NODE_NAME);\
//	}

#define GOTO_DAY_NODE(pNode, nYear, nMonth, nDay) \
	GOTO_CHILD(pNode, NODE_YEAR, nYear); \
	GOTO_CHILD(pNode, NODE_MONTH, nMonth); \
	GOTO_CHILD(pNode, NODE_DAY, nDay);

//#define CREATE_CONTENTS(strings, pNode) \
//	int nCount = strings.GetCount();\
//	for (int i=0; i<nCount; ++i)\
//{\
//	tinyxml2::XMLElement* pNewNode = m_doc.NewElement(NODE_CONTENT);\
//	tinyxml2::XMLText* pNewText =  m_doc.NewText(strings[i]);\
//	pNewNode->InsertEndChild(pNewText);\
//	pNode->InsertEndChild(pNewNode);\
//}

CXmlUser::CXmlUser(void)
	:CUser()
{
}

CXmlUser::CXmlUser( const CString& sName )
	:CUser(sName)
{
}


CXmlUser::~CXmlUser(void)
{
}


BOOL CXmlUser::LoadData( const CString& sDBName )
{
	m_sDBName = sDBName;
	return m_doc.LoadFile(sDBName) == XML_NO_ERROR;
}

BOOL CXmlUser::CommitData()
{
	return m_doc.SaveFile(m_sDBName) == XML_NO_ERROR;
}

BOOL CXmlUser::GetAllUserNames( CStringArray& arrNames ) const
{
	const tinyxml2::XMLElement* pNode 
		= m_doc.FirstChildElement()->FirstChildElement(USER_NODE_NAME);
	while(pNode != NULL)
	{
		const char* sName = pNode->Attribute(ATTR_VALUE);
		if (sName == NULL) return FALSE;

		arrNames.Add(sName);

		pNode = pNode->NextSiblingElement(USER_NODE_NAME);
	}

	return TRUE;
}

BOOL CXmlUser::GetEMail( CString& sEMail ) const
{
	const tinyxml2::XMLElement* pNode = NULL;
	pNode = FindUser();
	//GOTO_USER_NODE(pNode);
	if (pNode == NULL) return FALSE;
	
	sEMail = pNode->Attribute(ATTR_EMAIL);

	return TRUE;
}

BOOL CXmlUser::SetEMail( const CString& sEMail )
{
	return TRUE;
}

BOOL CXmlUser::GetToMail( CStringArray& arrayOut ) const
{
	const tinyxml2::XMLElement* pNode = NULL;
	pNode = FindUser();
	//GOTO_USER_NODE(pNode);
	if (pNode == NULL) return FALSE;

	pNode = pNode->FirstChildElement(NODE_TO);
	if (pNode == NULL) return FALSE;

	pNode = pNode->FirstChildElement(NODE_CONTENT);
	if (pNode == NULL) return FALSE;

	while(pNode != NULL)
	{
		arrayOut.Add(pNode->GetText());
		pNode = pNode->NextSiblingElement(NODE_CONTENT);
	}

	return TRUE;
}

BOOL CXmlUser::SetToMail( const CStringArray& arrTo )
{
	tinyxml2::XMLElement* pNode = NULL;
	pNode = FindUserModify();
	pNode = AddChildNode(pNode, NODE_TO, -1);
	pNode->DeleteChildren();
	AddStrings(arrTo, pNode, NODE_CONTENT);

	return TRUE;
}

BOOL CXmlUser::GetCCMail( CStringArray& arrayOut ) const
{
	const tinyxml2::XMLElement* pNode = NULL;
	pNode = FindUser();
	//GOTO_USER_NODE(pNode);
	if (pNode == NULL) return FALSE;

	pNode = pNode->FirstChildElement(NODE_CC);
	if (pNode == NULL) return FALSE;

	FillupContents(pNode, arrayOut);
	return TRUE;
}

BOOL CXmlUser::SetCCMail( const CStringArray& arrCC )
{
	tinyxml2::XMLElement* pNode = NULL;
	pNode = FindUserModify();
	//GOTO_USER_NODE(pNode);
	pNode = AddChildNode(pNode, NODE_CC, -1);
	pNode->DeleteChildren();
	AddStrings(arrCC, pNode, NODE_CONTENT);

	return TRUE;
}

BOOL CXmlUser::GetWeeklyCC( CStringArray& arrayOut ) const
{
	const tinyxml2::XMLElement* pNode = NULL;
	pNode = FindUser();
	//GOTO_USER_NODE(pNode);
	if (pNode == NULL) return FALSE;

	pNode = pNode->FirstChildElement(NODE_WEEKLY);
	if (pNode == NULL) return FALSE;

	FillupContents(pNode, arrayOut);
	return TRUE;
}

BOOL CXmlUser::SetWeeklyCC( const CStringArray& arrWeekly )
{
	tinyxml2::XMLElement* pNode = NULL;
	pNode = FindUserModify();
	pNode = AddChildNode(pNode, NODE_WEEKLY, -1);
	pNode->DeleteChildren();
	AddStrings(arrWeekly, pNode, NODE_CONTENT);
	return TRUE;
}

BOOL CXmlUser::GetReports(int nYear, int nMonth, int nDay, 
	CStringArray& arrayToday, CStringArray &arrayTomorrow) const
{
	arrayToday.RemoveAll();
	arrayTomorrow.RemoveAll();
	const tinyxml2::XMLElement* pNode = NULL;
	pNode = FindUser();
	//GOTO_USER_NODE(pNode);
	if (pNode == NULL) return FALSE;

	//GOTO_DAY_NODE(pNode, nYear, nMonth, nDay);

	pNode = pNode->FirstChildElement(NODE_YEAR);
		while(pNode != NULL)
		{
			if (atoi(pNode->Attribute(ATTR_VALUE)) == nYear) break;
			pNode = pNode->NextSiblingElement();
		}
	if (pNode == NULL) return FALSE;


	pNode = pNode->FirstChildElement(NODE_MONTH);
	while(pNode != NULL)
	{
		if (atoi(pNode->Attribute(ATTR_VALUE)) == nMonth) break;
		pNode = pNode->NextSiblingElement();
	}
	if (pNode == NULL) return FALSE;

	pNode = pNode->FirstChildElement(NODE_DAY);
	while(pNode != NULL)
	{
		if (atoi(pNode->Attribute(ATTR_VALUE)) == nDay) break;
		pNode = pNode->NextSiblingElement();
	}
	if (pNode == NULL) return FALSE;

	Fillup(pNode, NODE_TODAY, arrayToday);
	Fillup(pNode, NODE_TOMORROW, arrayTomorrow);
	return TRUE;
}


BOOL CXmlUser::SetReports( int nYear, int nMonth, int nDay, 
	const CStringArray& arrayToday, const CStringArray &arrayTomorrow)
{
	tinyxml2::XMLElement* pNode = NULL;
	pNode = FindUserModify();
	//GOTO_USER_NODE(pNode);
	
	pNode = AddChildNode(pNode, NODE_YEAR, nYear);
	pNode = AddChildNode(pNode, NODE_MONTH, nMonth);
	pNode = AddChildNode(pNode, NODE_DAY, nDay);

	pNode->DeleteChildren();
	AddStrings(arrayToday, pNode, NODE_TODAY);
	AddStrings(arrayTomorrow, pNode, NODE_TOMORROW);

	return TRUE;
}


// -- help func



const tinyxml2::XMLElement* CXmlUser::GetChildNodeByAttr(
	const tinyxml2::XMLElement* pParentNode, 
	const char* sNode, 
	int nValue) const
{
	ASSERT(pParentNode != NULL);
	ASSERT(sNode != NULL);

	GOTO_CHILD(pParentNode, sNode, nValue);

	return pParentNode;
}

tinyxml2::XMLElement* CXmlUser::GetChildNodeByAttr( 
	tinyxml2::XMLElement* pParentNode, 
	const char* sNode, 
	int nValue ) const
{
	ASSERT(pParentNode != NULL);
	ASSERT(sNode != NULL);

	GOTO_CHILD(pParentNode, sNode, nValue);

	return pParentNode;
}

void CXmlUser::FillupContents(const tinyxml2::XMLElement* pNode, 
	CStringArray &arrayOut) const
{
	Fillup(pNode, NODE_CONTENT, arrayOut);
}

void CXmlUser::Fillup( const tinyxml2::XMLElement* pNode, 
	const CString& sNodeName, CStringArray &arrayOut ) const
{
	arrayOut.RemoveAll();
	pNode = pNode->FirstChildElement(sNodeName);
	while(pNode != NULL)
	{
		arrayOut.Add(pNode->GetText());
		pNode = pNode->NextSiblingElement(sNodeName);
	}
}

BOOL CXmlUser::AddUser(const CString& sName)
{
	set_name(sName);

	if (!DoesUserExist(sName))
	{
		// create new one
		tinyxml2::XMLElement* pNode = NULL;
		pNode = m_doc.FirstChildElement();
		if (pNode == NULL)
		{
			return FALSE;
		}

		tinyxml2::XMLElement* pNewNode = m_doc.NewElement(USER_NODE_NAME);
		pNewNode->SetAttribute(ATTR_VALUE, sName);
		pNode->InsertEndChild(pNewNode);
	}

	return TRUE;
}

BOOL CXmlUser::DoesUserExist( const char* sName ) const
{
	const tinyxml2::XMLElement* pNode 
		= m_doc.FirstChildElement()->FirstChildElement(USER_NODE_NAME);
	while(pNode != NULL)
	{
		const char* sCurrentName = pNode->Attribute(ATTR_VALUE);
		if (strcmp(sCurrentName, sName) == 0)
		{
			return TRUE;
		}

		pNode = pNode->NextSiblingElement(USER_NODE_NAME);
	}

	return FALSE;
}

void CXmlUser::AddStrings( 
	const CStringArray& in, 
	tinyxml2::XMLElement* pStartNode, 
	const char* sChildNode )
{
	ASSERT(pStartNode != NULL);
	ASSERT(sChildNode != NULL);

	int nCount = in.GetCount();
	for (int i=0; i<nCount; ++i)
	{
		tinyxml2::XMLElement* pNewNode = m_doc.NewElement(sChildNode);
		tinyxml2::XMLText* pNewText =  m_doc.NewText(in[i]);
		pNewNode->InsertEndChild(pNewText);
		pStartNode->InsertEndChild(pNewNode);
	}
}

tinyxml2::XMLElement* CXmlUser::AddChildNode( 
	tinyxml2::XMLElement* pParentMode, 
	const char* sNode, 
	int nValue )
{
	tinyxml2::XMLElement* pChild = GetChildNodeByAttr(pParentMode, sNode, nValue);
	if (pChild == NULL)
	{	
		pChild = m_doc.NewElement(sNode);
		if (nValue != -1)
		{
			CString sTemp;
			sTemp.Format("%d", nValue);
			pChild->SetAttribute(ATTR_VALUE, sTemp);
		}

		pParentMode->InsertEndChild(pChild);
	}

	return pChild;
}

BOOL CXmlUser::GetContentsBeforeReports(CStringArray& out)
{
	const tinyxml2::XMLElement* pNode = FindUser();
	if (pNode == NULL)
	{
		return FALSE;
	}

	pNode = pNode->FirstChildElement(NODE_BODY);
	if (pNode == NULL)
	{
		return FALSE;
	}

	Fillup(pNode, NODE_BEFORE, out);

	return TRUE;
}

BOOL CXmlUser::GetContentsAfterReports( CStringArray& out )
{
	const tinyxml2::XMLElement* pNode = FindUser();
	if (pNode == NULL)
	{
		return FALSE;
	}

	pNode = pNode->FirstChildElement(NODE_BODY);
	if (pNode == NULL)
	{
		return FALSE;
	}

	Fillup(pNode, NODE_AFTER, out);

	return TRUE;
}

const tinyxml2::XMLElement* CXmlUser::FindUser() const
{
	const tinyxml2::XMLElement* pNode = m_doc.FirstChildElement();
	pNode = pNode->FirstChildElement(USER_NODE_NAME);

	if (!m_sName.IsEmpty())
	{
		while(pNode != NULL)
		{
			const char* sCurrentName = pNode->Attribute(ATTR_VALUE);
			if (strcmp(sCurrentName, m_sName) == 0)
			{
				return pNode;
			}
			pNode = pNode->NextSiblingElement(USER_NODE_NAME);
		}
	}

	return NULL; // not found
}

tinyxml2::XMLElement* CXmlUser::FindUserModify()
{
	tinyxml2::XMLElement* pNode = m_doc.FirstChildElement()->FirstChildElement(USER_NODE_NAME);

	if (!m_sName.IsEmpty())
	{
		while(pNode != NULL)
		{
			const char* sCurrentName = pNode->Attribute(ATTR_VALUE);
			if (strcmp(sCurrentName, m_sName) == 0)
			{
				return pNode;
			}
			pNode = pNode->NextSiblingElement(USER_NODE_NAME);
		}
	}

	return NULL; // not found
}

//tinyxml2::XMLElement*  CXmlUser::GetChildByValue(
//	tinyxml2::XMLElement* pParentNode, 
//	const char* sNode,
//	const char* sAttr) const
//{
//	ASSERT(pParentNode != NULL);
//	ASSERT(sNode != NULL);
//
//	// if the child node with input node name does not exist, return FALSE
//	tinyxml2::XMLElement* pNode 
//		= pParentNode->FirstChildElement(sNode);
//	if (pNode == NULL)
//	{
//		return NULL;
//	}
//
//	// if attribute is not specified, node is found
//	if (sAttr == NULL)
//	{
//		return pNode;
//	}
//
//	// search the node with specified attribute
//	while(pNode != NULL)
//	{
//		const char* sCurrentAttr = pNode->Attribute(ATTR_NAME);
//		if (strcmp(sCurrentAttr, sAttr) == 0)
//		{
//			return pNode;
//		}
//
//		pNode = pNode->NextSiblingElement(USER_NODE_NAME);
//	}
//
//	return NULL;	
//}

BOOL CXmlUser::SetContentsBeforeAndAfterReports(
	const CStringArray& before, const CStringArray& after)
{
	tinyxml2::XMLElement* pNode = NULL;
	pNode = FindUserModify();
	pNode = AddChildNode(pNode, NODE_BODY, -1);
	pNode->DeleteChildren();
	AddStrings(before, pNode, NODE_BEFORE);
	AddStrings(after, pNode, NODE_AFTER);
	return TRUE;
}
