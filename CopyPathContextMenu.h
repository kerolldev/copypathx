// CopyPathContextMenu.h : CCopyPathContextMenu の宣言
// [***]VC7.0 IContextMenuのエラーはPlatform SDKのインクルードディレクトリの優先順位を上げる

#pragma once

#include "resource.h"       // メイン シンボル
#include <shlguid.h>
#include <shlobj.h>
#include <comdef.h>
#include <list>
#include <string>
#include <fstream>

#include "resource.h"
#include "global.h"
#include "option.h"

#define ID_COPY_PATH	0

#define ID_MENU_1	1
#define ID_MENU_2	2
#define ID_MENU_3	4
#define ID_MENU_4	8
#define ID_MENU_5	16
#define ID_MENU_6	32
#define ID_MENU_7	64
#define ID_MENU_8	128
#define ID_MENU_SEPARATOR	9999

#define TOPCMD_MAX      1

// ICopyPathContextMenu
[
	object,
	uuid("D96F8104-CE33-4DA1-BA9E-E38A9F815CAF"),
	dual,	helpstring("ICopyPathContextMenu インターフェイス"),
	pointer_default(unique)
]
__interface ICopyPathContextMenu : IDispatch
{
};


// CCopyPathContextMenu
[
	coclass,
	threading(apartment),
	vi_progid("CopyPathX.CopyPathContextMenu"),
	progid("CopyPathX.CopyPathContextMenu.1"),
	version(1.0),
	uuid("BE21F5FB-D683-43B1-959B-DB6A83B218CB"),
	helpstring("CopyPathContextMenu Class")
]
class ATL_NO_VTABLE CCopyPathContextMenu : 
//	public ICopyPathContextMenu,
//	public IContextMenu3,
	public IContextMenu,
	public IShellExtInit
{
public:
	CCopyPathContextMenu()
	{
		m_hbmpCopy = ::LoadBitmap(_AtlBaseModule.GetModuleInstance(),
						MAKEINTRESOURCE(IDB_COPY));

		m_pOption = new COption(_AtlBaseModule.GetModuleInstance());
		m_cids = 0;
	}
	~CCopyPathContextMenu()
	{
		if(m_hbmpCopy) {
			::DeleteObject(m_hbmpCopy);
		}
		if(m_pOption) {
			delete m_pOption;
		}

	}

	/*
	void writeLog(_TCHAR * msg) {
	  // ファイルを開く
	  std::ofstream fout( "c:/copypathx.log", std::ios::app ); //追記モードで開く
	  // 開けなかった場合は終了
	  if( fout.fail() )
	  {
		return ;
	  }
	  // 出力
	  fout << msg << std::endl ;
	  // ファイルを閉じる
	  fout.close();
	}
	*/

	// IContextMenu interface
    STDMETHOD(QueryContextMenu)(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpici);
	#if defined(_M_X64)
    STDMETHOD(GetCommandString)(UINT_PTR idCmd, UINT  uType, UINT *pwReserved, LPSTR pszName, UINT cchMax);   
	#else
    STDMETHOD(GetCommandString)(UINT idCmd, UINT  uType, UINT *pwReserved, LPSTR pszName, UINT cchMax);   
	#endif

    
	// IContextMenu2 interface
	//STDMETHOD(HandleMenuMsg)(UINT uMsg, WPARAM wParam, LPARAM lParam);    
    
	// IContextMenu3 interface
	//STDMETHOD(HandleMenuMsg2)(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
    
	// IShellExtInit interface
	STDMETHOD(Initialize)(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID);

	//DECLARE_REGISTRY_RESOURCEID(IDR_COPYPATHX)

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CCopyPathContextMenu)
		//COM_INTERFACE_ENTRY(IContextMenu3)
		//COM_INTERFACE_ENTRY(IContextMenu2)
		COM_INTERFACE_ENTRY(IContextMenu)
		COM_INTERFACE_ENTRY(IShellExtInit)
	END_COM_MAP()


	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

private:
	bool IsDirectryAdd();
	int GetOption(_TCHAR * pKeyName, int idef);
	bool IsPlatformWin32NT();
	int GetCtrlStyle();
	bool IsMenuIcon();
	HRESULT ValidateCommand(UINT_PTR idCmd);

private:
	typedef std::basic_string<_TCHAR>		string;	
	std::list<string>						m_listFileNames;

	HBITMAP m_hbmpCopy;

	COption * m_pOption;

	UINT  m_cids;
	
	//void MakePath(LPPATHSETTING lpPathSetting, _TCHAR * pPath, _TCHAR * pTemp);
};