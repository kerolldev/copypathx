#include "StdAfx.h"
#include "option.h"

///////////////////////////////////////////////////////////////////
// 
COption::COption(HMODULE hModule)
{
	m_hModule = hModule;

	// ini�t�@�C���̃t���p�X���ݒ�
	_TCHAR		buff[MAX_PATH] = {0};
	_TCHAR		*pCurrent = NULL;
	::ZeroMemory(m_strFile, sizeof(m_strFile));
	if(GetModuleFileName(m_hModule, buff,sizeof(buff)) ) {
		pCurrent = _tcsrchr(buff, _T('\\') );
		_tcsncpy_s(m_strFile, buff, pCurrent - buff + 1);
		_tcscat_s(m_strFile, _T("option.ini"));
	}
}

///////////////////////////////////////////////////////////////////
// 
COption::~COption(void){}

///////////////////////////////////////////////////////////////////
// 
void COption::Init(void){}

/*
///////////////////////////////////////////////////////////////////
// int�^�̐ݒ�擾
	_TCHAR * pKeyName	�L�[��
	int idef			�f�t�H���g�l
*/
int COption::GetInt(_TCHAR * pKeyName, int idef)
{
	return GetPrivateProfileInt(_T("option"), pKeyName, idef, m_strFile);
}

///////////////////////////////////////////////////////////////////
// �����^�̐ݒ�擾
void COption::GetString(_TCHAR * pKeyName, _TCHAR * pDefault, _TCHAR * pRetruned, int nSize)
{
	GetPrivateProfileString(_T("option"), pKeyName, pDefault, pRetruned, nSize, m_strFile);
}