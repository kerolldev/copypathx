
#pragma once

///////////////////////////////////////////////////////////////////
// 設定構造体
typedef struct _PATHSETTING {
	int bDelimiter; 
	UINT bAddBackSlash; 
	UINT bMakeShortPath; 
	UINT bFileOnly;
	_TCHAR tempFileName[MAX_PATH + 50];
} PATHSETTING, *LPPATHSETTING;


///////////////////////////////////////////////////////////////////
// グローバル関数
BOOL IsDirectory(const _TCHAR * pPath);
BOOL IsDrive(const _TCHAR * pPath);
size_t MakePath(LPPATHSETTING lpPathSetting, const _TCHAR * pFileName, _TCHAR * pTempFileNameBuff, int size);


///////////////////////////////////////////////////////////////////
// 定数定義
// enum型を定義するためのクラス
class CopyPathX {
public:
	enum PathDelimiter {
		D_Normal,
		D_CStyle,
		D_Slash
	};

	CopyPathX() {
	}
};
