#include "stdafx.h"
#include "resource.h"
#include "global.h"

//#pragma warning(disable : 4996)

///////////////////////////////////////////////////////////////////
// ディレクトリかどうか
BOOL IsDirectory(const _TCHAR * pPath)
{
	return ((::GetFileAttributes(pPath) & FILE_ATTRIBUTE_DIRECTORY) ? TRUE : FALSE);
}

///////////////////////////////////////////////////////////////////
// ドライブかどうか
BOOL IsDrive(const _TCHAR * pPath)
{
	_TCHAR drive[_MAX_DRIVE];
	_TCHAR dir[_MAX_DIR];
	_TCHAR fname[_MAX_FNAME];
	_TCHAR ext[_MAX_EXT];

	if(_tcslen(pPath) > 3) {
		return FALSE;
	}

	if(!IsDirectory(pPath)) {
		return FALSE;
	}

	// 分割
	_tsplitpath_s(pPath, drive, sizeof(drive),dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
	if((dir[0] == _T('\\') || dir[0] == NULL) && fname[0] == NULL) {
		return TRUE;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////
// パス作成
size_t MakePath(LPPATHSETTING lpPathSetting, const _TCHAR * pFileName, _TCHAR * pTempFileNameBuff, int size)
{
	_TCHAR		strTemp[MAX_PATH + 50];
	_TCHAR		*pCurrent = NULL, *pLast = NULL;

	ZeroMemory(pTempFileNameBuff, size);

	// ショートパス
	if(lpPathSetting->bMakeShortPath) {
		::GetShortPathName(pFileName, strTemp, MAX_PATH + 50);
	}
	else {
		_tcscpy(strTemp, pFileName);
	}

//	if(lpPathSetting->bFileOnly && (IsDirectory(strTemp) == FALSE)) {
	if(lpPathSetting->bFileOnly) {
		_TCHAR filename[_MAX_FNAME + _MAX_EXT];
		_TCHAR drive[_MAX_DRIVE];
		_TCHAR dir[_MAX_DIR];
		_TCHAR fname[_MAX_FNAME];
		_TCHAR ext[_MAX_EXT];

		// Drive
		if(IsDrive(strTemp)) {
			return 0;
		}

		// 最後に\で終わるディレクトリの場合
		_TCHAR * lPos = _tcsrchr(strTemp, '\\');
		size_t len = _tcslen(strTemp);
		if(len < 1) { return 0; }

		if(strTemp[len - 1] == *lPos) {
			strTemp[len - 1] = NULL;
		}

		// 分割
		_tsplitpath_s(strTemp, drive, sizeof(drive),dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
		// 連結
		_tcscpy(filename, fname);
		_tcscat(filename, ext);
		_tcscpy_s(pTempFileNameBuff, size, filename);
	} else {
		pLast = strTemp;

		switch(lpPathSetting->bDelimiter)
		{
		case CopyPathX::D_Normal:
			_tcscpy_s(pTempFileNameBuff, size, pLast);
			break;
		case CopyPathX::D_CStyle:
			while ((pCurrent = _tcschr(pLast, _T('\\'))) != NULL) {
				_tcsncat_s(pTempFileNameBuff, size, pLast, pCurrent - pLast + 1);
				pLast = pCurrent + 1;
				_tcscat_s(pTempFileNameBuff, size, _T("\\"));
			}
			_tcscat_s(pTempFileNameBuff, size, pLast);
			break;
		case CopyPathX::D_Slash:
			while ((pCurrent = _tcschr(pLast, _T('\\'))) != NULL) {
				_tcsncat_s(pTempFileNameBuff, size, pLast, pCurrent - pLast);
				pLast = pCurrent + 1;
				_tcscat_s(pTempFileNameBuff, size, _T("/"));
			}
			_tcscat_s(pTempFileNameBuff, size, pLast);
			break;
		}

		if((IsDirectory(pFileName))) {
			size_t len = _tcslen(pTempFileNameBuff);
			if(lpPathSetting->bAddBackSlash) {
				switch(lpPathSetting->bDelimiter)
				{
				case CopyPathX::D_Normal:
					if(pTempFileNameBuff[len -1 ] != _T('\\')) {
						_tcscat_s(pTempFileNameBuff, size, _T("\\"));
					}
					break;
				case CopyPathX::D_CStyle:
					//OutputDebugStr(&m_pPathSetting->tempFileName[_tcslen(m_pPathSetting->tempFileName)]);
					//OutputDebugStr(&m_pPathSetting->tempFileName[_tcslen(m_pPathSetting->tempFileName) -1 ]);
					//ATLTRACE2( atlTraceGeneral, 0, "len= %d\n", _tcslen(m_pPathSetting->tempFileName) );

					if(pTempFileNameBuff[len -1 ] != _T('\\')) {
						_tcscat_s(pTempFileNameBuff, size, _T("\\\\"));
					}
					break;
				case CopyPathX::D_Slash:
					if(pTempFileNameBuff[len -1 ] != _T('/')) {
						_tcscat_s(pTempFileNameBuff, size, _T("/"));
					}
					break;
				}
			} else {
				switch(lpPathSetting->bDelimiter)
				{
				case CopyPathX::D_Normal:
					if(pTempFileNameBuff[len -1 ] == _T('\\')) {
						pTempFileNameBuff[len - 1] = NULL;
					}
					break;
				case CopyPathX::D_CStyle:
					if(pTempFileNameBuff[len -1 ] == _T('\\')) {
						pTempFileNameBuff[len - 1] = NULL;
						pTempFileNameBuff[len - 2] = NULL;
					}
					break;
				case CopyPathX::D_Slash:
					if(pTempFileNameBuff[len -1 ] == _T('/')) {
						pTempFileNameBuff[len - 1] = NULL;
					}
					break;
				}
			} // end if
		} // end if
	} // end if
	
	return _tcslen(pTempFileNameBuff);
}
