#pragma once
#include "user.h"
#include "tinyxml2.h"

class CXmlUser : public CUser
{
public:
	CXmlUser(void);
	CXmlUser(const CString& sName);
	virtual ~CXmlUser(void);

public:
	virtual BOOL LoadData(const CString& sDBName);
	virtual BOOL CommitData();

	virtual BOOL AddUser(const CString& sName);

	virtual BOOL GetAllUserNames(CStringArray& arrNames) const;

	virtual BOOL GetEMail(CString& sEMail) const;
	virtual BOOL SetEMail(const CString& sEMail);

	virtual BOOL GetToMail(CStringArray& arrayOut) const;
	virtual BOOL SetToMail(const CStringArray& arrIn);

	virtual BOOL GetCCMail(CStringArray& arrayOut) const;
	virtual BOOL SetCCMail(const CStringArray& arrIn);

	virtual BOOL GetWeeklyCC(CStringArray& arrayOut) const;
	virtual BOOL SetWeeklyCC(const CStringArray& arrIn);

	virtual BOOL GetReports(int nYear, int nMonth, int nDay, 
		CStringArray& arrayToday, CStringArray &arrayTomorrow) const;


	virtual BOOL SetReports(int nYear, int nMonth, int nDay, 
		const CStringArray& arrayToday, const CStringArray &arrayTomorrow);

	virtual BOOL GetContentsBeforeReports(CStringArray& out);
	virtual BOOL GetContentsAfterReports(CStringArray& out);

	virtual BOOL SetContentsBeforeAndAfterReports(
		const CStringArray& before, const CStringArray& after);



private:
	const tinyxml2::XMLElement* GetChildNodeByAttr(
		const tinyxml2::XMLElement* pParentNode, 
		const char* sNode, 
		int nValue) const;

	tinyxml2::XMLElement* GetChildNodeByAttr(
		tinyxml2::XMLElement* pParentNode, 
		const char* sNode, 
		int nValue) const;

	tinyxml2::XMLElement* AddChildNode( 
		tinyxml2::XMLElement* pParentMode, 
		const char* sNode, 
		int nValue );

	void FillupContents(
		const tinyxml2::XMLElement* pNode, CStringArray &arrayOut) const;

	void Fillup(const tinyxml2::XMLElement* pNode, const CString& sNodeName,
		CStringArray &arrayOut) const;
	BOOL DoesUserExist( const char* sName ) const;
	void AddStrings( const CStringArray& arrayToday, tinyxml2::XMLElement* pStartNode, const char* sChildNode );
	const tinyxml2::XMLElement* FindUser() const;

	//TODO: naming should be better
	tinyxml2::XMLElement* FindUserModify();
	
private:
	tinyxml2::XMLDocument m_doc;
};

