#pragma once

///////////////////////////////////////////////////////////////////
//  ê›íËÉNÉâÉX
class COption
{
public:
	COption(HMODULE hModule);
	~COption(void);
private:
	HMODULE		m_hModule;
	_TCHAR		m_strFile[MAX_PATH];
public:
	void Init(void);
	int GetInt(_TCHAR * pKeyName, int idef);
	void GetString(_TCHAR * pKeyName, _TCHAR * pDefault, _TCHAR * pRetruned, int nSize);

};
