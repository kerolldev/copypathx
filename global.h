
#pragma once

///////////////////////////////////////////////////////////////////
// �ݒ�\����
typedef struct _PATHSETTING {
	int bDelimiter; 
	UINT bAddBackSlash; 
	UINT bMakeShortPath; 
	UINT bFileOnly;
	_TCHAR tempFileName[MAX_PATH + 50];
} PATHSETTING, *LPPATHSETTING;


///////////////////////////////////////////////////////////////////
// �O���[�o���֐�
BOOL IsDirectory(const _TCHAR * pPath);
BOOL IsDrive(const _TCHAR * pPath);
size_t MakePath(LPPATHSETTING lpPathSetting, const _TCHAR * pFileName, _TCHAR * pTempFileNameBuff, int size);


///////////////////////////////////////////////////////////////////
// �萔��`
// enum�^���`���邽�߂̃N���X
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
