#ifndef __SENDFILETO_H__
#define __SENDFILETO_H__

#include <mapi.h>

class CSendFileTo
{
public:
	bool SendMail(HWND hWndParent, CString const &strAttachmentFileName, CString const &strSubject=_T(""))
	{
		if (strAttachmentFileName.IsEmpty())
			return false;

		if (!hWndParent || !::IsWindow(hWndParent))
			return false;

		HINSTANCE hMAPI = ::LoadLibraryA(_T("MAPI32.DLL"));
		if (!hMAPI)
			return false;

		ULONG (PASCAL *SendMail)(ULONG, ULONG_PTR, MapiMessage*, FLAGS, ULONG);
		(FARPROC&)SendMail = GetProcAddress(hMAPI, _T("MAPISendMail"));

		if (!SendMail)
			return false;

		TCHAR szFileName[_MAX_PATH];
		TCHAR szPath[_MAX_PATH];
		TCHAR szSubject[_MAX_PATH];
		::StrCpy(szFileName, strAttachmentFileName.GetString());
		::StrCpy(szPath, strAttachmentFileName.GetString());
		::StrCpy(szSubject, strSubject.GetString());

		MapiFileDesc fileDesc;
		::ZeroMemory(&fileDesc, sizeof(fileDesc));
		fileDesc.nPosition = (ULONG)-1;
		fileDesc.lpszPathName = szPath;
		fileDesc.lpszFileName = szFileName;

		MapiMessage message;
		::ZeroMemory(&message, sizeof(message));
		message.lpszSubject = szSubject;
		message.nFileCount = 1;
		message.lpFiles = &fileDesc;

		int nError = SendMail(0, (ULONG_PTR)hWndParent, &message, MAPI_LOGON_UI|MAPI_DIALOG, 0);

		if (nError != SUCCESS_SUCCESS && nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
			return false;

		return true;
	}
};

#endif