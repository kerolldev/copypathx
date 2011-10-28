// CopyPathContextMenu.cpp : CCopyPathContextMenu の実装

#include "stdafx.h"

#include <stdio.h>

#pragma warning(disable : 4996)
#include <atlapp.h>
#include <atluser.h>
#include "CopyPathContextMenu.h"
//#include <fstream>




#ifdef _UNICODE
#define CF_TEXT_FORMAT		CF_UNICODETEXT
#define ALLOC_GLOBAL_STRING(hGlobal, strText) HGLOBAL hGlobal = ::GlobalAlloc(GMEM_SHARE, ((wcslen(strText) + 1) * 2));
#else
#define CF_TEXT_FORMAT		CF_TEXT
#define ALLOC_GLOBAL_STRING(hGlobal, strText) HGLOBAL hGlobal = ::GlobalAlloc(GMEM_SHARE, strlen(strText) + 1);
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyPathContextMenu

///////////////////////////////////////////////////////////////////
// 
STDMETHODIMP CCopyPathContextMenu::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	//writeLog(_T("QueryContextMenu"));
	
	if (uFlags & CMF_DEFAULTONLY) {
		return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 0);
	}

	m_cids = 0;
	_TCHAR		strMenuCopy[80] = {0};
	_TCHAR		strMenuText[100];
	UINT imenu = indexMenu;
	// Adds our item as ID: (idCmdFirst + ID_COPY_PATH) = idCmdFirst.
	//_stprintf(strMenuText, _T("*Copy &Path%s to Clipboard"), (m_listFileNames.size() > 1 ? _T("s") : _T("")));
//	_stprintf(strMenuText, _T("パスをコピー%s"), (m_listFileNames.size() > 1 ? _T("(複数)") : _T("")));
	m_pOption->GetString(_T("menutext"), _T("パスをコピー"), strMenuCopy,  sizeof(strMenuCopy));

	_tcscat(strMenuCopy, _T("%s"));
	_stprintf(strMenuText, strMenuCopy, (m_listFileNames.size() > 1 ? _T("[複数]") : _T("")));
	::InsertMenu(hmenu, indexMenu++, MF_STRING | MF_BYPOSITION,
        idCmdFirst + ID_COPY_PATH, strMenuText);

	m_cids = TOPCMD_MAX;

	if(IsMenuIcon()) {
		BOOL ret = ::SetMenuItemBitmaps ( hmenu, imenu, MF_BYPOSITION, m_hbmpCopy, NULL );
	}

	return ResultFromScode(MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, (USHORT)(ID_COPY_PATH + 1)));
}

///////////////////////////////////////////////////////////////////
// 
STDMETHODIMP CCopyPathContextMenu::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID)
{
	//writeLog(_T("Initialize"));

	HRESULT				hres = E_FAIL;
	FORMATETC			fmte = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	STGMEDIUM			medium;
	int					nFileCount = 0, i;
	_TCHAR				strFilePath[MAX_PATH] = {0};

	// No data object
	if (lpdobj == NULL)
		return E_FAIL;

	// Use the given IDataObject to get a list of filenames (CF_HDROP).
	hres = lpdobj->GetData(&fmte, &medium);

	if (FAILED(hres))
		return E_FAIL;

	m_listFileNames.clear();
	// Make sure HDROP contains at least one file.
	if ((nFileCount = DragQueryFile((HDROP)medium.hGlobal, (UINT)(-1), NULL, 0)) >= 1)
	{
		bool bDirAdd = IsDirectryAdd();
		// Loop through all the files that were selected.
		for (i = 0; i < nFileCount; i++) {
			DragQueryFile((HDROP)medium.hGlobal, i, strFilePath, MAX_PATH);
			// If the file name is a directory, make sure it has a backslash at the end.
			// 最後に追加するのをオプションに
			if ((::GetFileAttributes(strFilePath) & FILE_ATTRIBUTE_DIRECTORY) && bDirAdd) {
				size_t nLen = _tcslen(strFilePath);
				if (strFilePath[nLen-1] != _T('\\')) {
					_tcscat(strFilePath, _T("\\"));
				}
			}
			// Add the file name to the end of the list.
			m_listFileNames.push_back(strFilePath);
		}
		hres = S_OK;
	}
	else
		hres = E_FAIL;

	// Release the data.
	ReleaseStgMedium (&medium);

	return hres;
}

///////////////////////////////////////////////////////////////////
// 
STDMETHODIMP CCopyPathContextMenu::InvokeCommand(LPCMINVOKECOMMANDINFO lpici)
{
	//writeLog(_T("InvokeCommand"));

	_TCHAR*		pStrClipboardText = NULL, strTempFileNameBuff[MAX_PATH + 50];
	_TCHAR		*pCurrent = NULL, *pLast = NULL;
	size_t			nFileCount = 0, i;
	BOOLEAN		bMakeShortPath = ((GetKeyState(VK_SHIFT) & 0x8000) != 0);
	BOOLEAN		bMakeCStyleString = 0, bMakeSlash = 0;
	BOOLEAN		bLButton = 0;

	// [2003.06.14]
	PATHSETTING pathSet = {0};

	WTL::CMenu menu;
	POINT pt;
	int mID = 0;

	CMINVOKECOMMANDINFOEX* lpicix = (CMINVOKECOMMANDINFOEX*)lpici;
	BOOL fUnicode = lpici->cbSize >= sizeof(CMINVOKECOMMANDINFOEX) &&
                  (lpici->fMask & CMIC_MASK_UNICODE);

	HRESULT hr = ValidateCommand(fUnicode ? (UINT_PTR)lpicix->lpVerbW
                                        : (UINT_PTR)lpici->lpVerb);

	if (!SUCCEEDED(hr)) {
		return hr;
	}

	if(IsPlatformWin32NT()) {	// NT系
		// 左Ctrlキー
		bMakeCStyleString = ((GetKeyState(VK_LCONTROL) & 0x8000) != 0);
		// 右Ctrlキー スラッシュ
		bMakeSlash = ((GetKeyState(VK_RCONTROL) & 0x8000) != 0);
	}
	else {
		if((GetKeyState(VK_CONTROL) & 0x8000) != 0) {
			if(GetCtrlStyle() == 1) {
				bMakeSlash = 1;	// スラッシュ
			}
			else {
				bMakeCStyleString = 1;	// 
			}
		}
	}

	/* [2003.06.14]
	左を押しながら右クリックでメニュー選択
	*/
	pathSet.bAddBackSlash = IsDirectryAdd();
	if(bMakeCStyleString) {
		pathSet.bDelimiter = CopyPathX::D_CStyle;
	} else if(bMakeSlash) {
		pathSet.bDelimiter = CopyPathX::D_Slash;
	} else {
		pathSet.bDelimiter = CopyPathX::D_Normal;
	}
	pathSet.bMakeShortPath = bMakeShortPath;


	bLButton = ((GetKeyState(VK_LBUTTON) & 0x8000) != 0);
	if(bLButton) {
		_tcscpy(pathSet.tempFileName, m_listFileNames.front().data());

		BOOL bDrive = IsDrive(pathSet.tempFileName);
		if(!IsDirectryAdd() && bDrive) {
			_tcscat(pathSet.tempFileName, _T("\\"));
		}


		menu.CreatePopupMenu();
		
		// メニューをマウス・カーソル付近に表示
		if (!GetCursorPos( &pt)) {
			//MessageBox(hWnd, "GetCursorPos() Error", "Error", MB_OK);
			return S_OK;
		}

		size_t len1;
		size_t len2;
		size_t len3;
		size_t len4;

		if(!bDrive) {
			pathSet.bDelimiter = CopyPathX::D_Normal;
			pathSet.bFileOnly = true;
			len1 = MakePath(&pathSet, pathSet.tempFileName, strTempFileNameBuff, sizeof(strTempFileNameBuff));
			// 1
			menu.AppendMenu(MF_STRING, ID_MENU_1, strTempFileNameBuff);
		} else {
			++mID;
		}

		pathSet.bFileOnly = false;
		pathSet.bDelimiter = CopyPathX::D_Normal;
		len2 = MakePath(&pathSet, pathSet.tempFileName, strTempFileNameBuff, sizeof(strTempFileNameBuff));
		// 2
		menu.AppendMenu(MF_STRING, ID_MENU_2, strTempFileNameBuff);

		pathSet.bDelimiter = CopyPathX::D_Slash;
		len3 = MakePath(&pathSet, pathSet.tempFileName, strTempFileNameBuff, sizeof(strTempFileNameBuff));
		// 3
		menu.AppendMenu(MF_STRING, ID_MENU_3, strTempFileNameBuff);

		pathSet.bDelimiter = CopyPathX::D_CStyle;
		len4 = MakePath(&pathSet, pathSet.tempFileName, strTempFileNameBuff, sizeof(strTempFileNameBuff));
		// 4
		menu.AppendMenu(MF_STRING, ID_MENU_4, strTempFileNameBuff);


		if(!bDrive) {
			bool mFlg = false;
			menu.AppendMenu(MF_STRING | MF_SEPARATOR, ID_MENU_SEPARATOR, _T(""));

			pathSet.bMakeShortPath = true;
			pathSet.bFileOnly = true;
			if(len1 != MakePath(&pathSet, pathSet.tempFileName, strTempFileNameBuff, sizeof(strTempFileNameBuff))) {
				// 5
				menu.AppendMenu(MF_STRING, ID_MENU_5, strTempFileNameBuff);
			}

			pathSet.bFileOnly = false;


			pathSet.bDelimiter = CopyPathX::D_Normal;
			if(len2 != MakePath(&pathSet, pathSet.tempFileName, strTempFileNameBuff, sizeof(strTempFileNameBuff))) {
				// 6
				menu.AppendMenu(MF_STRING, ID_MENU_6, strTempFileNameBuff);
				mFlg = true;
			}

			pathSet.bDelimiter = CopyPathX::D_Slash;
			if(len3 != MakePath(&pathSet, pathSet.tempFileName, strTempFileNameBuff, sizeof(strTempFileNameBuff))) {
				// 7
				menu.AppendMenu(MF_STRING, ID_MENU_7, strTempFileNameBuff);
				mFlg = true;
			}
			pathSet.bDelimiter = CopyPathX::D_CStyle;
			if(len4 != MakePath(&pathSet, pathSet.tempFileName, strTempFileNameBuff, sizeof(strTempFileNameBuff))) {
				// 8
				menu.AppendMenu(MF_STRING, ID_MENU_8, strTempFileNameBuff);
				mFlg = true;
			}
			if(mFlg == false) {
				menu.DeleteMenu(ID_MENU_SEPARATOR, MF_BYCOMMAND);
			}
		}

		// メニュー表示
		UINT uID = (UINT)menu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_RETURNCMD,
			pt.x, pt.y,
			lpici->hwnd, NULL);
		::PostMessage(lpici->hwnd, WM_NULL, 0, 0);	//起動を確実にする
        
		menu.DestroyMenu();

		// フラグ再設定
		pathSet.bFileOnly = false;
		pathSet.bMakeShortPath = false;
		switch(uID) {
			case ID_MENU_1:
				pathSet.bFileOnly = true;
				pathSet.bDelimiter = CopyPathX::D_Normal;
				break;
			case ID_MENU_2:
				pathSet.bDelimiter = CopyPathX::D_Normal;
				break;
			case ID_MENU_3:
				pathSet.bDelimiter = CopyPathX::D_Slash;
				break;
			case ID_MENU_4:
				pathSet.bDelimiter = CopyPathX::D_CStyle;
				break;
			case ID_MENU_5:
				pathSet.bMakeShortPath = true;
				pathSet.bFileOnly = true;
				pathSet.bDelimiter = CopyPathX::D_Normal;
				break;
			case ID_MENU_6:
				pathSet.bMakeShortPath = true;
				pathSet.bDelimiter = CopyPathX::D_Normal;
				break;
			case ID_MENU_7:
				pathSet.bMakeShortPath = true;
				pathSet.bDelimiter = CopyPathX::D_Slash;
				break;
			case ID_MENU_8:
				pathSet.bMakeShortPath = true;
				pathSet.bDelimiter = CopyPathX::D_CStyle;
				break;
		}
	}

    switch (LOWORD(lpici->lpVerb)) {
	case ID_COPY_PATH:
		nFileCount = m_listFileNames.size();
		if (nFileCount == 0) {
			return S_OK;
		}
		
		// The '+ 50' is so that we provide ample room for double backslashes.
		pStrClipboardText = new _TCHAR[nFileCount * (MAX_PATH+50)];
		pStrClipboardText[0] = _T('\0');

		size_t clipSize =  nFileCount * (MAX_PATH+50);

		bool bDirAdd = IsDirectryAdd();
		// Loop through all the files.
		for (i = 0; i < nFileCount; i++) {
			_TCHAR strTemp[MAX_PATH + 50];
			 _tcscpy(strTemp, (const _TCHAR *)m_listFileNames.front().data());

			 // ドライブの場合は最後に\を追加しておく
			if(bDirAdd == false && IsDrive(strTemp)) {
				_tcscat(strTemp, _T("\\"));
			}

			MakePath(&pathSet, (const _TCHAR *)strTemp, strTempFileNameBuff, sizeof(strTempFileNameBuff));

			_tcscat_s(pStrClipboardText, clipSize, strTempFileNameBuff);
			// If this isn't the last file, add a line break before we add the next file name.
			if (i != (nFileCount - 1))
				_tcscat_s(pStrClipboardText, clipSize, _T("\r\n"));

			m_listFileNames.pop_front();
		}		

		// Open and empty the clipboard, allocate global memory, copy the file
		// names into the memory, and then set the clipboard.
		if (::OpenClipboard(NULL)) {
			::EmptyClipboard();
			ALLOC_GLOBAL_STRING(hText, pStrClipboardText);
			LPTSTR pText = (LPTSTR) ::GlobalLock(hText);
			_tcscpy_s(pText, strlen(pStrClipboardText) + 1, pStrClipboardText);
			::SetClipboardData(CF_TEXT_FORMAT, hText);
			::GlobalUnlock(hText);
			::CloseClipboard();
		}

		delete[] pStrClipboardText;
		break;
    }
    return S_OK;
}

///////////////////////////////////////////////////////////////////
// 
#if defined(_M_X64)
STDMETHODIMP CCopyPathContextMenu::GetCommandString(UINT_PTR idCmd, UINT  uType, UINT *pwReserved, LPSTR pszName, UINT cchMax)
#else
STDMETHODIMP CCopyPathContextMenu::GetCommandString(UINT idCmd, UINT  uType, UINT *pwReserved, LPSTR pszName, UINT cchMax)
#endif
{
//	writeLog(_T("GetCommandString"));

	HRESULT		hr = E_INVALIDARG; 
	size_t		nFileCount = 0;
	_TCHAR		strFileCount[100], strHelpTextBuff[200];
	BOOLEAN		bMakeShortPath = ((GetKeyState(VK_SHIFT) & 0x8000) != 0);
	BOOLEAN		bMakeCStyleString = 0, bMakeSlash = 0;

	hr = ValidateCommand(idCmd);
	if (FAILED(hr)) {
		if (uType == GCS_VALIDATEA || uType == GCS_VALIDATEW) {
			hr = S_FALSE;
		}
		return hr;
	}

	if(IsPlatformWin32NT()) {	// NT系
		// 左Ctrlキー
		bMakeCStyleString = ((GetKeyState(VK_LCONTROL) & 0x8000) != 0);
		// 右Ctrlキー スラッシュ
		bMakeSlash = ((GetKeyState(VK_RCONTROL) & 0x8000) != 0);
	}
	else {
		if((GetKeyState(VK_CONTROL) & 0x8000) != 0) {
			if(GetCtrlStyle() == 1) {
				bMakeSlash = 1;	// スラッシュ
			}
			else {
				bMakeCStyleString = 1;	// 
			}			
		}
	}


	USES_CONVERSION;

	switch (idCmd) {
	case ID_COPY_PATH:
		switch (uType) {
		// Requesting a help string for the status bar.
		case GCS_HELPTEXTA:
		case GCS_HELPTEXTW:
			nFileCount = m_listFileNames.size();
			// Help text looks like "Copies the [full / short] path[s] to the clipboard. [n Objects Selected]"
			_stprintf(strFileCount, _T("%d 個のオブジェクトを選択"), nFileCount);			
			_stprintf(strHelpTextBuff, _T("%s パスをクリップボードにコピー%s%s。 [%s]"),
				(bMakeShortPath ? _T("ショート") : _T("フル")),
				(bMakeCStyleString ? _T(" (C-スタイル)") : _T("")),
				(bMakeSlash ? _T(" (スラッシュ-スタイル)") : _T("")),
				strFileCount);

			if ((uType & GCS_HELPTEXTW) == GCS_HELPTEXTW) {
				wcsncpy_s((LPWSTR)pszName, cchMax, T2W(strHelpTextBuff), cchMax);
				((LPWSTR)pszName)[cchMax - 1] = OLESTR('\0');
			}
			else {
				strncpy_s((LPSTR)pszName, cchMax, T2A(strHelpTextBuff), cchMax);
				((LPSTR)pszName)[cchMax - 1] = '\0';
			}

			hr = S_OK;
			break;
			
		case GCS_VERBA:
		case GCS_VERBW:
			hr = S_OK;
			break;

		case GCS_VALIDATEA:
		case GCS_VALIDATEW:
			hr = S_OK;
			break;
		}
		
	}
	return hr;
}

///////////////////////////////////////////////////////////////////
// 
int CCopyPathContextMenu::GetCtrlStyle()
{
	return GetOption(_T("ctrlstyle"), 0);
}

///////////////////////////////////////////////////////////////////
// ディレクトリ名の後に文字(\\ or \ or /)を追加するかどうか
bool CCopyPathContextMenu::IsDirectryAdd()
{
	return (GetOption(_T("addbackslash"), 1) == 1);
}

///////////////////////////////////////////////////////////////////
// メニューのアイコン表示/非表示
bool CCopyPathContextMenu::IsMenuIcon()
{
	return (GetOption(_T("menuicon"), 1) == 1);
}



///////////////////////////////////////////////////////////////////
// ini ファイルからオプション値の取得
int CCopyPathContextMenu::GetOption(_TCHAR * pKeyName, int idef)
{
	return m_pOption->GetInt(pKeyName, idef);
}

///////////////////////////////////////////////////////////////////
// プラットフォームがWin32NTかどうかを取得する
// true : Win32NT系
bool CCopyPathContextMenu::IsPlatformWin32NT()
{
	// OSVERSIONINFO 構造体の必要な個所を埋める
	OSVERSIONINFO osVer;
	osVer.dwOSVersionInfoSize = sizeof(osVer);

	// OS のバージョン情報の取得
	GetVersionEx(&osVer);

	// OS が Win32 NT プラットフォームかを判断
	if (osVer.dwPlatformId == VER_PLATFORM_WIN32_NT) {
		return true;
	}

	return false;
}


HRESULT CCopyPathContextMenu::ValidateCommand(UINT_PTR idCmd)
{
  if (idCmd < m_cids) {
    return S_OK;
  }

  return E_INVALIDARG;
}